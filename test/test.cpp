/*----------------------------------------------------------------------------------------
  30/12/2021
  Author: R WILSON
  Platforms: ESP32 ONLY - ESP8266 not supported
  Version: 4.0.0 - 17 Mar 2022
  Language: C/C++/Arduino
  New - large panel size version - FLEX PCB VERSION
  ----------------------------------------------------------------------------------------
  Description:
  ESP32 connected to NeoPixel WS2812B LED matrix display (64x16 - 32x8 x 4 Panels)
  RTC DS3231 I2C - SDA(21)gray/orange and SCL(22)purple/yellow
  TEMP SENSOR BUILD INTO DS3231
  ----------------------------------------------------------------------------------------
  Libraries:
  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
  Adafruit RTC Lib: https://github.com/adafruit/RTClib              (lib version 2.0.2)
  ArduinoJson Benoit Blanchon: https://arduinojson.org              (lib version 6.18.5)
  me-no-dev: https://github.com/me-no-dev/AsyncTCP
  me-no-dev: https://github.com/me-no-dev/ESPAsyncWebServer
  Rui Santos: https://RandomNerdTutorials.com - information resource
  
  FastLED Daniel Garcia: https://fastled.io/                        (lib version
  LEDText A Liddiment https://github.com/AaronLiddiment/LEDText     (lib version 3.4.0)
  LEDMatrix A Liddiment https://github.com/AaronLiddiment/LEDMatrix (lib version )
    modified: J Skrotzky https://github.com/Jorgen-VikingGod/LEDMatrix Dec'21)
   
  LOAD TO SPIFFS THESE EXTERNAL FILES:
    >> index.html notfound.html settings.html time.html timepicker.html 
  Connect to ESP32MessageBoard WIFI AP created by ESP32  
  Open browser to http://192.168.4.1/
  Open browser to http://1.2.3.4/ 
  Password: 12345678 or password
  Enter message to be displayed on the NeoMatrix scrolling display
----------------------------------------------------------------------------------------*/

#include <Arduino.h>

#include <WiFi.h>
#include "SPIFFS.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include "EEPROMHandler.h"                  // Storing message into permanent memory

#include <FastLED.h>
#include <LEDMatrix.h>
#include <LEDText.h>
#include <Font12x16RW.h>                    // "FontRobert.h" 5x7 font for 2 lines display

#include <Wire.h>
#include "RTClib.h"

#define BUF_SIZE    400                     // 400 out of 512 used
#define PASS_BSIZE  9                       // 8 digit password
#define PASS_EXIST  405                     // password $ address
#define PASS_BEGIN  410                     // password value stored
#define P_CHAR      '`'
#define BRT_BEGIN   425                     // Brightness value stored (int = 4Bytes)

#define BUZZER_PIN  23                      // Buzzer pin

#define LED_PIN     27                      // NeoPixel pin 1/2 display
#define LED2_PIN    13                      // NeoPixel pin other 1/2 display
#define LED_BUILTIN 5                       // lolin buildin led on 5

#define VOLTS       5
#define MAX_MA      500                     // !change to 3000

#define MATRIX_TYPE          VERTICAL_ZIGZAG_MATRIX
#define MATRIX_TILE_WIDTH   -64             // width of EACH NEOPIXEL MATRIX (not total display)
#define MATRIX_TILE_HEIGHT   8              // height of each matrix
#define MATRIX_TILE_H        1              // number of matrices horizontally (neg for reverse order)
#define MATRIX_TILE_V        2              // number of matrices vertically (neg for reverse order)
#define MATRIX_SIZE          (MATRIX_WIDTH*MATRIX_HEIGHT)
#define MATRIX_PANEL         (MATRIX_WIDTH*MATRIX_HEIGHT)
#define MATRIX_WIDTH         (MATRIX_TILE_WIDTH*MATRIX_TILE_H)
#define MATRIX_HEIGHT        (MATRIX_TILE_HEIGHT*MATRIX_TILE_V)
#define NUM_LEDS             (MATRIX_WIDTH*MATRIX_HEIGHT)

#define EFF_CHAR_UP          0xd8           // in sprintf change 
#define EFF_CHAR_DOWN        0xd9           // EFFECT_CHAR_UP to EFF_CHAR_UP in loop
#define EFF_CHAR_LEFT        0xda
#define EFF_CHAR_RIGHT       0xdb

#define EFF_SCROLL_LEFT      0xdc
#define EFF_SCROLL_RIGHT     0xdd
#define EFF_SCROLL_UP        0xde
#define EFF_SCROLL_DOWN      0xdf

#define EFF_RGB              0xe0
#define EFF_HSV              0xe1
#define EFF_RGB_CV           0xe2
#define EFF_HSV_CV           0xe3
#define EFF_RGB_AV           0xe6
#define EFF_HSV_AV           0xe7
#define EFF_RGB_CH           0xea
#define EFF_HSV_CH           0xeb
#define EFF_RGB_AH           0xee
#define EFF_HSV_AH           0xef
#define EFF_COLR_EMPTY       0xf0
#define EFF_COLR_DIMMING     0xf1

#define EFF_BACKGND_ERASE    0xf4
#define EFF_BACKGND_LEAVE    0xf5
#define EFF_BACKGND_DIMMING  0xf6

#define EFF_FRAME_RATE       0xf8
#define EFF_DELAY_FRAMES     0xf9
#define EFF_CUSTOM_RC        0xfa

TaskHandle_t TaskHandle_1;

int BRIGHTNESS = 30;

int rc;                                     // custom return char for ledMatrix lib

char ssid[] = "LolinMessageBoard";          // Change to your name
char password[PASS_BSIZE] = "password";     // dont change password here, change using web app

RTC_DS3231 RTC;
uint16_t h = 0;
uint16_t m = 0;
uint16_t dow = 0;
byte lastAlarm = 254;

IPAddress ip(1, 2, 3, 4);
IPAddress subnet(255, 255, 255, 0);
AsyncWebServer server(80);

DateTime now;                               // Decalre global variable for time
char szTime[6];                             // hh:mm\0
char daysOfTheWeek[7][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
char curMessage[BUF_SIZE] = "Welcome";
char newMessage[BUF_SIZE] = "Welcome";
char newTime[17] = "01.01.2022 12:00";
bool newMessageAvailable = true;
bool newTimeAvailable = false;

cLEDMatrix<MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, MATRIX_TYPE, MATRIX_TILE_H, MATRIX_TILE_V, VERTICAL_BLOCKS> leds;
cLEDText ScrollingMsg, StaticgMsg, RTCErrorMessage;

CRGB fleds[512];

char txtDateA[] = { EFFECT_HSV_AH "\x00\xff\xff\xff\xff\xff" "12|30" };
char txtDateB[] = { EFFECT_HSV_AH "\x00\xff\xff\xff\xff\xff" "12:30" };
char szMesg[BUF_SIZE] = { EFFECT_FRAME_RATE "\x00" EFFECT_HSV_AH "\x00\xff\xff\xff\xff\xff" EFFECT_SCROLL_LEFT "     WELCOME TO NORTHLINK COLLEGE     "  EFFECT_CUSTOM_RC "\x01" };

String handleTimeUpdate(uint8_t *data, size_t len){
  data[len] = '\0';
  String json = (char*)data;

  for (int i = 0; i < len; i++) {
    newTime[i] = data[i];
  }
  
  if (data[0] == '\0'){ newTimeAvailable = false;}          // if not a blank field
  else {newTimeAvailable = true;}

  Serial.print("new time: ");
  Serial.println(json);

  return "{\"status\" : \"ok\", \"time\" : \"" + json + "\"}";
}

String handleMessageUpdate(uint8_t *data, size_t len){
  data[len] = '\0';
  String json = (char*)data;
  
  DynamicJsonDocument doc(JSON_OBJECT_SIZE(2) + 350);
  DeserializationError error = deserializeJson(doc, json);  // Deserialize the JSON document

  if (error){                                                // Test if parsing succeeds.
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return "deserializeJson error";
  }

  BRIGHTNESS = int(doc["brightness"]);
  strcpy(newMessage, doc["message"]);

  Serial.print("message and brightness handle: ");
  Serial.println(json);
  
  newMessageAvailable = true;

  return json;
}

String handleSettingsUpdate(uint8_t *data, size_t len){
  String returnstring;

  data[len] = '\0';
  String json = (char*)data;
  
  DynamicJsonDocument doc(JSON_OBJECT_SIZE(3) + 130);
  DeserializationError error = deserializeJson(doc, json);   // Deserialize the JSON document

  if (error){                                                // Test if parsing succeeds.
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return "deserializeJson error";
  }

  const char *oldpassword = doc["oldpassword"];
  const char *newpassword = doc["newpassword"];
  const char *renewpassword = doc["renewpassword"];

  if (String(oldpassword) == String(password) && String(newpassword) == String(renewpassword))
  {
    returnstring = "password:" + String(password) + " newpassword:" + String(newpassword) + " renewpassword:" + String(renewpassword);
    Serial.print("password handle: ");
    Serial.println(json);
    eepromWriteChar(BUF_SIZE, '\0');        // wall so message doesnt display password at max buffer pos
    eepromWriteChar(PASS_EXIST, P_CHAR);    // user password now exists
    eepromWriteString(PASS_BEGIN, String(newpassword));
    Serial.println("new password saved");
    Serial.println(eepromReadChar(PASS_BEGIN));
    Serial.println("\n\nRESTARTING in 8 Seconds...\n");
    WiFi.softAPdisconnect();
    delay(8000);
    ESP.restart();
  }
  else
  {
    returnstring = "error, passwords don't match";
    Serial.print("password handle error: ");
    Serial.println(json);
    Serial.println("\nerror, passwords don't match\n");
    delay(1000);
  }

  return returnstring;
}

void updateDefaultAPPassword(){
  Serial.println("updateDefaultAPPassword");
  if (eepromReadChar(PASS_EXIST) == P_CHAR)
  {
    delay(200);
    Serial.print("user pwd found: \"");
  }
  else
  {
    delay(200);
    Serial.println("\nuser pwd is not found\nrestoring default pwd to ");

    eepromWriteChar(BUF_SIZE, '\0');           // wall so message doesnt display password at max buffer pos
    eepromWriteChar(PASS_EXIST, P_CHAR);       // user password now exists
    eepromWriteString(PASS_BEGIN, "password"); // default password is "password"
  }

  String eeString = eepromReadString(PASS_BEGIN, PASS_BSIZE);
  eeString.toCharArray(password, eeString.length() + 1);

  Serial.print(eeString);
  Serial.print("\" from EEPROM.\n\"");
  Serial.print(password);
  Serial.println("\" is used as the WIFI pwd");
  delay(1000);
}

void rtcErrorHandler(){
  char txtRTCError[] = { EFFECT_FRAME_RATE "\x00" EFFECT_HSV_AH "\x00\xff\xff\xff\xff\xff" EFFECT_SCROLL_LEFT "     RTC NOT FOUND     "  EFFECT_CUSTOM_RC "\x99" }; //!char txtRTCError[BUF_SIZE]
  RTCErrorMessage.SetFont(Font12x16Data);
  RTCErrorMessage.Init(&leds, leds.Width(), RTCErrorMessage.FontHeight() + 1, 0, 0);
  RTCErrorMessage.SetText((unsigned char *)txtRTCError, sizeof(txtRTCError) - 1);
  RTCErrorMessage.SetTextColrOptions(COLR_RGB | COLR_SINGLE, 0x00, 0x00, 0xff);

  while(1) {
    if(RTCErrorMessage.UpdateText() != 0x99) {
      FastLED.show();
      delay(30);
    }
    else {
      RTCErrorMessage.SetText((unsigned char *)txtRTCError, sizeof(txtRTCError) - 1);
      RTCErrorMessage.UpdateText();
      FastLED.show();
    }
  }
}

void fxSinlon() //* Startup effects
{ 
  int gHue = 0, FRAMES_PER_SECOND = 120, FX_NUM_LEDS = 512;
  FastLED.addLeds<WS2812B,LED_PIN,GRB>(fleds, 512).setCorrection(TypicalLEDStrip);  //std fastled for effects
  FastLED.addLeds<WS2812B,LED2_PIN,GRB>(fleds, 512).setCorrection(TypicalLEDStrip);  //std fastled for effects
  FastLED.clear(true);
  FastLED.setBrightness(255);
  
  while(gHue <201){
    fadeToBlackBy(fleds, FX_NUM_LEDS, 20); // a colored dot sweeping back and forth, with fading trails
    int pos = beatsin16( 13, 0, FX_NUM_LEDS-1 );
    fleds[pos] += CHSV( gHue, 255, 192);
    FastLED.show();  
    FastLED.delay(1000/FRAMES_PER_SECOND); 
    
    EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
    EVERY_N_MILLISECONDS( 1000 ) {digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));}
  }
  // back to Matrixled
  FastLED.addLeds<WS2812B,  LED_PIN, GRB>(leds[0], 0,             leds.Size()/2).setCorrection(TypicalLEDStrip);//TypicalSMD5050 //! new
  FastLED.addLeds<WS2812B, LED2_PIN, GRB>(leds[0], leds.Size()/2, leds.Size()/2).setCorrection(TypicalLEDStrip); //! new
  FastLED.setBrightness(BRIGHTNESS);
}

static void MultiCoreTask1(void* pvParameters)
{
  for (int i = 0; i < 6; i++)
  {
    digitalWrite(BUZZER_PIN, HIGH);
    vTaskDelay(200/portTICK_PERIOD_MS);
    digitalWrite(BUZZER_PIN, LOW);
    vTaskDelay(200/portTICK_PERIOD_MS);
  }
  vTaskDelete(TaskHandle_1);    // Delete the task using the TaskHandle_1
}

void alarmCheck()
{
  if(dow > 0 && dow < 5)        // Mon -Thu
  {
    if(h == 8 && m == 0 && lastAlarm != 0){
      /* Task function, name, stackSize, parameter, priority, handler, core 0 */   
      xTaskCreatePinnedToCore(MultiCoreTask1,"Task1",10000,NULL,1,&TaskHandle_1,0);  
      lastAlarm = 0;
      return;
    }
    if(h == 10 && m == 0 && lastAlarm != 1){
      xTaskCreatePinnedToCore(MultiCoreTask1,"Task1",10000,NULL,1,&TaskHandle_1,0);  
      lastAlarm = 1;
      return;
    }
    if(h == 10 && m == 20 && lastAlarm != 2){
      xTaskCreatePinnedToCore(MultiCoreTask1,"Task1",10000,NULL,1,&TaskHandle_1,0);  
      lastAlarm = 2;
      return;
    }
    if(h == 12 && m == 20 && lastAlarm != 3){
      xTaskCreatePinnedToCore(MultiCoreTask1,"Task1",10000,NULL,1,&TaskHandle_1,0);  
      lastAlarm = 3;
      return;
    }
    if(h == 13 && m == 0 && lastAlarm != 4){
      xTaskCreatePinnedToCore(MultiCoreTask1,"Task1",10000,NULL,1,&TaskHandle_1,0);  
      lastAlarm = 4;
      return;
    }
    if(h == 14 && m == 20 && lastAlarm != 5){
      xTaskCreatePinnedToCore(MultiCoreTask1,"Task1",10000,NULL,1,&TaskHandle_1,0);  
      lastAlarm = 5;
      return;
    }
    if(h == 14 && m == 40 && lastAlarm != 6){
      xTaskCreatePinnedToCore(MultiCoreTask1,"Task1",10000,NULL,1,&TaskHandle_1,0);  
      lastAlarm = 6;
      return;
    }
    if(h == 15 && m == 00 && lastAlarm != 7){
      xTaskCreatePinnedToCore(MultiCoreTask1,"Task1",10000,NULL,1,&TaskHandle_1,0);  
      lastAlarm = 7;
      return;
    }
    if(h == 15 && m == 30 && lastAlarm != 8){
      xTaskCreatePinnedToCore(MultiCoreTask1,"Task1",10000,NULL,1,&TaskHandle_1,0);  
      lastAlarm = 8;
      return;
    }
    if(h == 15 && m == 23 && lastAlarm != 9){
      xTaskCreatePinnedToCore(MultiCoreTask1,"Task1",10000,NULL,1,&TaskHandle_1,0);  
      lastAlarm = 9;
      return;
    }
  }
  else if (dow == 5)            // Fri
  {
    if(h == 8 && m == 0 && lastAlarm != 100){
      xTaskCreatePinnedToCore(MultiCoreTask1,"Task1",10000,NULL,1,&TaskHandle_1,0);  
      lastAlarm = 100;
      return;
    }
    if(h == 9 && m == 40 && lastAlarm != 101){
      xTaskCreatePinnedToCore(MultiCoreTask1,"Task1",10000,NULL,1,&TaskHandle_1,0);  
      lastAlarm = 101;
      return;
    }
    if(h == 10 && m == 0 && lastAlarm != 102){
      xTaskCreatePinnedToCore(MultiCoreTask1,"Task1",10000,NULL,1,&TaskHandle_1,0);  
      lastAlarm = 102;
      return;
    }
    if(h == 12 && m == 0 && lastAlarm != 103){
      xTaskCreatePinnedToCore(MultiCoreTask1,"Task1",10000,NULL,1,&TaskHandle_1,0);  
      lastAlarm = 103;
      return;
    }
    if(h == 12 && m == 20 && lastAlarm != 104){
      xTaskCreatePinnedToCore(MultiCoreTask1,"Task1",10000,NULL,1,&TaskHandle_1,0);  
      lastAlarm = 104;
      return;
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("\n\nScrolling display from your Internet Browser");

  // STARTUP BEEPS
  xTaskCreatePinnedToCore(MultiCoreTask1,"Task1",10000,NULL,1,&TaskHandle_1,0);
  
  //  EEPROM
  EEPROM.begin(512);
  Serial.println("\n\nEEPROM STARTED");
  curMessage[0] = newMessage[0] = '\0';
  eepromReadString(0,BUF_SIZE).toCharArray(curMessage,BUF_SIZE);  // Read stored msg from EEPROM address 0
  newMessageAvailable = false;
  Serial.print("Message: ");
  Serial.println(curMessage);

  BRIGHTNESS = eepromReadInt(BRT_BEGIN);                          // read Neomatrix brightness value
  if(BRIGHTNESS > 255) { BRIGHTNESS = 255;}
  Serial.print("NeoMatrix Brightness set to ");                   // done in line 318
  Serial.println(BRIGHTNESS);
  
  
  //  START DISPLAY
  Serial.println("\nNEOMATRIX DIPLAY STARTED");
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_MA);         //! 
  FastLED.addLeds<WS2812B,  LED_PIN, GRB>(leds[0], 0,             leds.Size()/2).setCorrection(TypicalLEDStrip);//TypicalSMD5050 //! new
  FastLED.addLeds<WS2812B, LED2_PIN, GRB>(leds[0], leds.Size()/2, leds.Size()/2).setCorrection(TypicalLEDStrip); //! new

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear(true);

  ScrollingMsg.SetFont(Font12x16Data);
  ScrollingMsg.Init(&leds, leds.Width(), ScrollingMsg.FontHeight() + 1, 0, 0);
  ScrollingMsg.SetText((unsigned char *)szMesg, sizeof(szMesg) - 1);
  ScrollingMsg.SetTextColrOptions(COLR_RGB | COLR_SINGLE, 0x00, 0x00, 0xff);

  StaticgMsg.SetFont(Font12x16Data);
  StaticgMsg.Init(&leds, leds.Width(), ScrollingMsg.FontHeight() + 1, -2, 0); // >> 1 pixel
  StaticgMsg.SetText((unsigned char *)txtDateA, sizeof(txtDateA) - 1);
  StaticgMsg.SetTextColrOptions(COLR_RGB | COLR_SINGLE, 0x00, 0x00, 0xff);
  
  //  RTC  
  Wire.begin();                           // DS3231 RTC I2C - SDA(21) and SCL(22)

  Serial.print("\nRTC STARTING >>> ");                                  
  if (! RTC.begin()) {
    Serial.println("RTC NOT FOUND");
    rtcErrorHandler();                      // Blocking call, dont move on
  }
  Serial.println("RTC STARTED\n");

  now = RTC.now(); 
  sprintf(szTime, "Time: %02d:%02d", now.hour(), now.minute());
  Serial.println(szTime);

  pinMode(LED_BUILTIN, OUTPUT);             // Heartbeat
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(BUZZER_PIN, OUTPUT);              // Buzzer
  digitalWrite(BUZZER_PIN, LOW);

  //  SPIFFS
  if(!SPIFFS.begin(true)){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
  }

  //  WIFI 2
  Serial.print("\nWIFI >> Connecting to ");
  Serial.println(ssid);  

  updateDefaultAPPassword();                // get Wifi password from EEPROM

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, subnet);
  WiFi.softAP(ssid, password);


  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  server.on("/message", HTTP_POST, [](AsyncWebServerRequest * request){},NULL, 
      [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
          String myResponse = handleMessageUpdate(data,len);
          request->send(200, "text/plain", myResponse);
  });

  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/settings.html", "text/html");
  });
  server.on("/settings/send", HTTP_POST, [](AsyncWebServerRequest * request){},NULL, 
      [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
          String myResponse = handleSettingsUpdate(data,len);
          request->send(200, "text/plain", myResponse);
  });

  server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/time.html", "text/html");
  });
  server.on("/time/send", HTTP_POST, [](AsyncWebServerRequest * request){},NULL, 
      [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
          String myResponse = handleTimeUpdate(data,len);
          request->send(200, "text/plain", myResponse);
  });

  server.on("/timepicker", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/timepicker.html", "text/html");
  });
  server.on("/timepicker/send", HTTP_POST, [](AsyncWebServerRequest * request){},NULL, 
      [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
          String myResponse = handleTimeUpdate(data,len);
          request->send(200, "text/plain", myResponse);
  });

  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/notfound.html", "text/html");
  });

  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/favicon.png", "image/png");
  });
 
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  server.begin();
  Serial.println("SERVER STARTED");

  //  DISPLAY WELCOME MESSAGE
  fxSinlon();                                 //* Display special startup effect
  while(ScrollingMsg.UpdateText() != 1)
  {
    FastLED.show();
    delay(30);
    EVERY_N_MILLISECONDS( 1000 ) {digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));}
  }
  ScrollingMsg.SetText((unsigned char *)szMesg, sizeof(szMesg) - 1);   // reset to start of string

  //Serial.println(TIMER_BASE_CLK);
}

void loop()
{
  static uint32_t timeLast = 0;               // Heartbeat
  static uint8_t t = 0;                       // temperature
  static uint8_t updatetemp = 11;             // so updates temp at startup
  
  if (millis() - timeLast >= 1000){
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    timeLast = millis();

    now = RTC.now();                          // Update the global var with current time 
    m = now.minute();  
    h = now.hour();
    dow = now.dayOfTheWeek();

    //txtDateA[7] = '2';// HRS//txtDateA[8] = '3';// HRS//txtDateA[10] = '5';// MIN//txtDateA[11] = '9';// MIN
    sprintf(txtDateA, "%c%c%c%c%c%c%c%02d%c%02d", EFF_HSV_AH,0x00,0xff,0xff,0xff,0xff,0xff,h,'|',m);
    sprintf(txtDateB, "%c%c%c%c%c%c%c%02d%c%02d", EFF_HSV_AH,0x00,0xff,0xff,0xff,0xff,0xff,h,':',m);

    if(curMessage[0] == '\0'){  // when no message
      sprintf(szMesg, "%c%c%c%c%c%c%c%c%c%c%s%02d%c%02d%c%c%c%c%c%c%c%c%c%s%02d%c%c%c%c%c%c%c%c%c%s%s%c%c%c%c%c%c%c%c%s%02d%c%02d%c%c%c%s%c%c",
                        EFF_FRAME_RATE,0x00,EFF_HSV_AH,0x00,0xff,0xff,0xff,0xff,0xff,
                        EFF_SCROLL_LEFT,"     ",h,':',m,EFF_DELAY_FRAMES,0x00,0x2c,EFF_CUSTOM_RC,0x02,
                        EFF_RGB,0x00,0xc8,0x64,"      ",t,'^',' ',EFF_DELAY_FRAMES,0x00,0xee,
                        EFF_RGB,0xd3,0x54,0x00,"      ",daysOfTheWeek[dow],' ',EFF_DELAY_FRAMES,0x00,0xee,
                        EFF_RGB,0x00,0x80,0x80,"      ",now.day(),'-',now.month(),EFF_DELAY_FRAMES,0x00,0xee,"      ",
                        EFF_CUSTOM_RC,0x01);
    }
    else{
      sprintf(szMesg, "%c%c%c%c%c%c%c%c%c%c%s%02d%c%02d%c%c%c%c%c%c%c%c%c%s%02d%c%c%c%c%c%c%c%c%c%s%s%c%c%c%c%c%c%c%c%s%02d%c%02d%c%c%c%s%c%c%c%c%c%c%c%c%c%s%s%c%c%c%c",
                        EFF_FRAME_RATE,0x00,EFF_HSV_AH,0x00,0xff,0xff,0xff,0xff,0xff,
                        EFF_SCROLL_LEFT,"     ",h,':',m,EFF_DELAY_FRAMES,0x00,0x2c,EFF_CUSTOM_RC,0x02,
                        EFF_RGB,0x00,0xc8,0x64,"      ",t,'^',' ',EFF_DELAY_FRAMES,0x00,0xee,
                        EFF_RGB,0xd3,0x54,0x00,"      ",daysOfTheWeek[dow],' ',EFF_DELAY_FRAMES,0x00,0xee,
                        EFF_RGB,0x00,0x80,0x80,"      ",now.day(),'-',now.month(),EFF_DELAY_FRAMES,0x00,0xee,
                        "      ",EFF_HSV_AH,0x00,0xff,0xff,0xff,0xff,0xff,EFF_FRAME_RATE,0x03,curMessage,"     ",EFF_FRAME_RATE,0x00,
                        EFF_CUSTOM_RC,0x01);
    }
                  
    if(++updatetemp > 10){                    // Update temperature every 10 sec, visual glitch
      t = RTC.getTemperature();               // +or- from this for calibration
      updatetemp = 0;
    }

    alarmCheck();
  }

  if (newMessageAvailable){
    strcpy(curMessage, newMessage);           // Copy new message to display
    eepromWriteString(0, newMessage);         // Write String to EEPROM Address 0
    newMessageAvailable = false;
    Serial.println("new message received, updated EEPROM\n");
    delay(100);
    eepromWriteInt(BRT_BEGIN, BRIGHTNESS);    // Write new brightness value
    FastLED.setBrightness(BRIGHTNESS);
    Serial.print("NeoMatrix Brightness set to ");
    Serial.println(BRIGHTNESS);
  }

  if (newTimeAvailable){
    int tY, tM, tD, th, tm;
    sscanf(newTime, "%2d.%2d.%4d %2d:%2d", &tD, &tM, &tY, &th, &tm); // extract received dd.mm.yyy hh:ss
    RTC.adjust(DateTime(tY, tM, tD, th, tm, 30));                    // rtc.adjust(DateTime(yyyy, m, d, h, m, s));
    newTimeAvailable = false;
    Serial.println(newTime);
    Serial.println("new time received, updated RTC");
  }

  rc = ScrollingMsg.UpdateText();
  if (rc == -1 || rc == 1)  // -1 means end of char array, 1 means end of msg because custom rc is received
  {
    ScrollingMsg.SetText((unsigned char *)szMesg, sizeof(szMesg) - 1);
  }
  else if (rc == 2)                               // EFFECT_CUSTOM_RC "\x02"
  {
    for (int j = 2; j < 10; j++)                  // want to start on even number to run drawline
    {
      if(j % 2 == 0){ //even
        StaticgMsg.SetText((unsigned char *)txtDateA, sizeof(txtDateA) - 1);
        StaticgMsg.UpdateText();
        leds.DrawLine(0, 0, 0, 7, CRGB(0, 0, 0)); // blank column 1, due to visual glitch text shifting << 1 pixel
      }
      else{
        StaticgMsg.SetText((unsigned char *)txtDateB, sizeof(txtDateB) - 1);
        StaticgMsg.UpdateText();
      }
      FastLED.show();
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(1000);
    }
  }
  else
  {
    FastLED.show();
  }
  //!delay(5);
}