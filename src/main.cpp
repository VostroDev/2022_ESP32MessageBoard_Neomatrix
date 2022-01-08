/*----------------------------------------------------------------------------------------
  30/12/2021
  Author: R WILSON
  Platforms: ESP32
  Version: 1.0.1 - 30 DEC 2021
  Language: C/C++/Arduino
  Working
  ----------------------------------------------------------------------------------------
  Description:
  ESP32 connected to NeoPixel WS2812B LED matrix display (32x8 - 4 Panels)
  MATRIX_TYPE      VERTICAL_MATRIX
  RTC DS3231 I2C - SDA(21)gray    and SCL(22)purple
  TEMP SENSOR BUILD INTO DS3231
  ----------------------------------------------------------------------------------------
  Libraries:
  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
  Adafruit RTC Lib: https://github.com/adafruit/RTClib              (lib version 2.0.2)
  ArduinoJson Benoit Blanchon: https://arduinojson.org              (lib version 6.18.5)
  FastLED Daniel Garcia: https://fastled.io/                        (lib version
  LEDText A Liddiment https://github.com/AaronLiddiment/LEDText     (lib version 3.4.0)
  LEDMatrix A Liddiment https://github.com/AaronLiddiment/LEDMatrix (lib version )
    modified: J Skrotzky https://github.com/Jorgen-VikingGod/LEDMatrix Dec'21)
  MTR Datepicker: http://mtrdesign.github.io/mtr-datepicker/
  
  Connect to ESP32MessageBoard WIFI AP created by ESP32  
  Open browser to http://192.168.4.1/ or www.message.com
  password 12345678 or password
  Enter message to be displayed on the NeoMatrix scrolling display
----------------------------------------------------------------------------------------*/

#include <Arduino.h>
#include <SPI.h>
#include "EEPROMHandler.h"                  // Storing message into permanent memory
#include "index.h"                          // HTML message  webpage with javascript
#include "settings.h"                       // HTML settings webpage with javascript
#include "timeset.h"                        // HTML settings webpage with javascript
#include "timepicker.h"                     // HTML settings webpage with javascript
#include "notfound.h"                       // HTML error 404 not found catch
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ArduinoJson.h>

#include <FastLED.h>
#include <LEDMatrix.h>
#include <LEDText.h>
#include "FontRobert.h"                     // 5x7 font use <FontMatriseRW.h>

#include <Wire.h>
#include "RTClib.h"

#define BUF_SIZE   400                      // 400 out of 512 used
#define PASS_BSIZE 40                       // 40 out of 512 used
#define PASS_EXIST 450                      // password $ address
#define PASS_BEGIN 460                      // password value stored
#define P_CHAR     '`'

#define BRT_BEGIN  505                      // Brightness value stored

#define LED_BUILTIN 26

#define LED_PIN 27
#define VOLTS   5
#define MAX_MA  400
//#define BRIGHTNESS 30

#define MATRIX_WIDTH  32
#define MATRIX_HEIGHT 8
#define MATRIX_TYPE VERTICAL_MATRIX

#define  EFF_CHAR_UP          0xd8          // for sprintf change EFFECT_CHAR_UP to EFF_CHAR_UP in loop
#define  EFF_CHAR_DOWN        0xd9
#define  EFF_CHAR_LEFT        0xda
#define  EFF_CHAR_RIGHT       0xdb

#define  EFF_SCROLL_LEFT      0xdc
#define  EFF_SCROLL_RIGHT     0xdd
#define  EFF_SCROLL_UP        0xde
#define  EFF_SCROLL_DOWN      0xdf

#define  EFF_RGB              0xe0
#define  EFF_HSV              0xe1
#define  EFF_RGB_CV           0xe2
#define  EFF_HSV_CV           0xe3
#define  EFF_RGB_AV           0xe6
#define  EFF_HSV_AV           0xe7
#define  EFF_RGB_CH           0xea
#define  EFF_HSV_CH           0xeb
#define  EFF_RGB_AH           0xee
#define  EFF_HSV_AH           0xef
#define  EFF_COLR_EMPTY       0xf0
#define  EFF_COLR_DIMMING     0xf1

#define  EFF_BACKGND_ERASE    0xf4
#define  EFF_BACKGND_LEAVE    0xf5
#define  EFF_BACKGND_DIMMING  0xf6

#define  EFF_FRAME_RATE       0xf8
#define  EFF_DELAY_FRAMES     0xf9
#define  EFF_CUSTOM_RC        0xfa

int BRIGHTNESS = 30;

int rc;                                      // custom return char for ledMatrix lib

char ssid[] = "ESP32MessageBoard";           // Change to your name
char password[PASS_BSIZE] = "12345678";      // dont change password here, change using web app

uint16_t h = 0;
uint16_t m = 0;
RTC_DS3231 RTC;

const byte DNS_PORT = 53;

IPAddress ip(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

DNSServer dnsServer;
WebServer server(80);

DateTime now;                               // Decalre global variable for time
char szTime[6];                             // hh:mm\0
char daysOfTheWeek[7][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

char curMessage[BUF_SIZE] = "Vostro Message Board ";
char newMessage[BUF_SIZE] = "Vostro Message Board";
char newTime[6] = "00:00";
bool newMessageAvailable = true;
bool newTimeAvailable = false;

cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> leds;

cLEDText ScrollingMsg, StaticgMsg, RTCErrorMessage;

CRGB fleds[256]; //!

char txtDateA[] = { EFFECT_HSV_AH "\x00\xff\xff\xff\xff\xff" "12|30" };
char txtDateB[] = { EFFECT_HSV_AH "\x00\xff\xff\xff\xff\xff" "12:30" };
char szMesg[BUF_SIZE] = { EFFECT_FRAME_RATE "\x00" EFFECT_HSV_AH "\x00\xff\xff\xff\xff\xff" EFFECT_SCROLL_LEFT "     ESP32 MESSAGE BOARD BY R WILSON    "  EFFECT_CUSTOM_RC "\x01" };

void handleTimeUpdate(){
  Serial.println("handleTimeUpdate");
  String data = server.arg("plain");
  server.send(200, "text/json", "{\"status\" : \"ok\", \"time\" : \"" + data + "\"}"); // log to console
  delay(100);

  data.toCharArray(newTime, data.length() + 1);     // read new time
  newTimeAvailable = (strlen(newTime) != 0);        // if not a blank field

  Serial.print("new time: ");
  Serial.println(data);
}

void handleMessageUpdate(){
  Serial.println("handleMessageUpdate");
  String json = server.arg("plain");
  const size_t bufferSize = JSON_OBJECT_SIZE(2) + 350;      // Max message size buffer
  
  DynamicJsonDocument doc(bufferSize);
  DeserializationError error = deserializeJson(doc, json);  // Deserialize the JSON document

  if (error)                                                // Test if parsing succeeds.
  { 
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  BRIGHTNESS = int(doc["brightness"]);
  const char *xmessage = doc["message"];

  server.send(200, "text/plain", "brightness:" + String(BRIGHTNESS) + " message:" + String(xmessage)); // log OK to console
  delay(100);
  Serial.print("message and brightness handle: ");
  Serial.println(json);
  
  sprintf(newMessage, xmessage);
  newMessageAvailable = true;
}

void handleSettingsUpdate(){
  Serial.println("handleSettingsUpdate");
  String json = server.arg("plain");
  const size_t bufferSize = JSON_OBJECT_SIZE(3) + 130;      // 130 XLarge buffer twice needed

  DynamicJsonDocument doc(bufferSize);
  DeserializationError error = deserializeJson(doc, json);  // Deserialize the JSON document

  if (error)                                                // Test if parsing succeeds.
  { 
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  const char *oldpassword = doc["oldpassword"];
  const char *newpassword = doc["newpassword"];
  const char *renewpassword = doc["renewpassword"];

  if (String(oldpassword) == String(password) && String(newpassword) == String(renewpassword))
  {
    server.send(200, "text/plain", "password:" + String(password) + " newpassword:" + String(newpassword) + " renewpassword:" + String(renewpassword)); // log OK to console
    delay(100);
    Serial.print("password handle: ");
    Serial.println(json);
    eepromWriteChar(BUF_SIZE, '\0');        // wall so message doesnt display password at max buffer pos
    eepromWriteChar(PASS_EXIST, P_CHAR);    // user password now exists
    eepromWriteString(PASS_BEGIN, String(newpassword));
    Serial.println("new password saved");
    Serial.println(eepromReadChar(PASS_BEGIN));
    WiFi.softAPdisconnect();
    delay(8000);
    ESP.restart();
  }
  else
  {
    server.send(200, "text/plain", "error, passwords don't match"); // log error to console
    delay(100);
    Serial.print("password handle error: ");
    Serial.println(json);
    Serial.println("\nerror, passwords don't match\n");
    delay(1000);
  }
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
  char txtRTCError[BUF_SIZE] = { EFFECT_FRAME_RATE "\x00" EFFECT_HSV_AH "\x00\xff\xff\xff\xff\xff" EFFECT_SCROLL_LEFT "     RTC NOT FOUND     "  EFFECT_CUSTOM_RC "\x99" };
  RTCErrorMessage.SetFont(RobertFontData);
  RTCErrorMessage.Init(&leds, leds.Width(), RTCErrorMessage.FontHeight() + 1, 0, 0); //? change to +2 for 5x7 font
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

void fxSinlon() //!
{
  FastLED.addLeds<WS2812B,LED_PIN,GRB>(fleds, 250).setCorrection(TypicalLEDStrip);  //std fastled for effects
  FastLED.clear(true);
  FastLED.setBrightness(150);
  int gHue = 0, NUM_LEDS = 250, FRAMES_PER_SECOND = 120;
  while(gHue <201){
    fadeToBlackBy( fleds, NUM_LEDS, 20); // a colored dot sweeping back and forth, with fading trails
    int pos = beatsin16( 13, 0, NUM_LEDS-1 );
    fleds[pos] += CHSV( gHue, 255, 192);
    FastLED.show();  
    FastLED.delay(1000/FRAMES_PER_SECOND); 
    
    EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  }
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds[0], leds.Size()).setCorrection(TypicalLEDStrip); // back to Matrixled
  FastLED.setBrightness(BRIGHTNESS);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("\n\nScrolling display from your Internet Browser");

  //  EEPROM
  EEPROM.begin(512);
  Serial.println("\n\nEEPROM STARTED");
  curMessage[0] = newMessage[0] = '\0';
  eepromReadString(0,BUF_SIZE).toCharArray(curMessage,BUF_SIZE);  // Read stored msg from EEPROM address 0
  newMessageAvailable = false;
  Serial.print("Message: ");
  Serial.println(curMessage);

  BRIGHTNESS = eepromReadInt(BRT_BEGIN);                          // read Neomatrix brightness value
  if(BRIGHTNESS > 254) { BRIGHTNESS = 30;}
  Serial.print("NeoMatrix Brightness set to ");                   // done in line 318
  Serial.println(BRIGHTNESS);
  
  
  //  START DISPLAY
  Serial.println("\nNEOMATRIX DIPLAY STARTED");
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_MA);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds[0], leds.Size()).setCorrection(TypicalLEDStrip); //TypicalSMD5050
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear(true);

  ScrollingMsg.SetFont(RobertFontData);
  ScrollingMsg.Init(&leds, leds.Width(), ScrollingMsg.FontHeight() + 1, 0, 0); //? change to +2 for 5x7 font
  ScrollingMsg.SetText((unsigned char *)szMesg, sizeof(szMesg) - 1);
  ScrollingMsg.SetTextColrOptions(COLR_RGB | COLR_SINGLE, 0x00, 0x00, 0xff);

  StaticgMsg.SetFont(RobertFontData);
  StaticgMsg.Init(&leds, leds.Width(), ScrollingMsg.FontHeight() + 1, 1, 0); // >> 1 pixel //? change to +2 for 5x7 font
  StaticgMsg.SetText((unsigned char *)txtDateA, sizeof(txtDateA) - 1);
  StaticgMsg.SetTextColrOptions(COLR_RGB | COLR_SINGLE, 0x00, 0x00, 0xff);

  //  RTC
  Wire.begin();                             // DS3231 RTC I2C - SDA(21) and SCL(22)
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

  //  WIFI
  Serial.print("\nWIFI >> Connecting to ");
  Serial.println(ssid);  

  updateDefaultAPPassword();                // get Wifi password from EEPROM

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, subnet);
  WiFi.softAP(ssid, password);

  server.on("/",[](){server.send_P(200,"text/html", indexpage);});
  server.on("/message", HTTP_POST, handleMessageUpdate);

  server.on("/settings",[](){server.send_P(200,"text/html", settingspage);});
  server.on("/settings/send", HTTP_POST, handleSettingsUpdate);

  server.on("/time",[](){server.send_P(200,"text/html", timesetpage);});
  server.on("/time/send", HTTP_POST, handleTimeUpdate);

  server.on("/timepicker",[](){server.send_P(200,"text/html", timepickerpage);});
  server.on("/timepicker/send", HTTP_POST, handleTimeUpdate);

  server.onNotFound([]() {server.send_P(200,"text/html", notfoundpage);});

  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(DNS_PORT, "www.message.com", ip);
  Serial.println("DNS PORT: www.message.com");
  
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  server.begin();
  Serial.println("SERVER STARTED");

  //  DISPLAY WELCOME MESSAGE
  fxSinlon();                                 //! Display special startup effect
  while(ScrollingMsg.UpdateText() != 1)
  {
    FastLED.show();
    delay(30);
  }
  ScrollingMsg.SetText((unsigned char *)szMesg, sizeof(szMesg) - 1);   // reset to start of string
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

    updatetemp ++;                            // Update temperature every 10 sec, visual glitch
    if(updatetemp > 10){
      t = RTC.getTemperature();               // +or- from this for calibration
      updatetemp = 0;
    }
  }

  dnsServer.processNextRequest();             // dns server
  server.handleClient();                      // WIFI

  if (newMessageAvailable){
    strcpy(curMessage, newMessage);           // Copy new message to display
    eepromWriteString(0, newMessage);         // Write String to EEPROM Address 0
    newMessageAvailable = false;
    Serial.println("new message received, updated EEPROM\n");
    delay(100);
    eepromWriteInt(BRT_BEGIN, BRIGHTNESS);    // White new brightness value
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

  //txtDateA[7] = '2';// HRS//txtDateA[8] = '3';// HRS//txtDateA[10] = '5';// MIN//txtDateA[11] = '9';// MIN

  sprintf(txtDateA, "%c%c%c%c%c%c%c%02d%c%02d", EFF_HSV_AH,0x00,0xff,0xff,0xff,0xff,0xff,h,'|',m);
  sprintf(txtDateB, "%c%c%c%c%c%c%c%02d%c%02d", EFF_HSV_AH,0x00,0xff,0xff,0xff,0xff,0xff,h,':',m);

  sprintf(szMesg, "%c%c%c%c%c%c%c%c%c%c%s%02d%c%02d%c%c%c%c%c%c%c%c%c%c%s%02d%c%c%c%c%c%c%c%c%c%c%s%s%c%c%c%c%c%c%c%c%c%s%02d%c%02d%c%c%c%c%s%c%c%c%c%c%c%c%c%c%c%s%s%s%c%c%c%c",
                                  EFF_FRAME_RATE,0x00,EFF_HSV_AH,0x00,0xff,0xff,0xff,0xff,0xff,
                                  EFF_SCROLL_LEFT,"     ",h,':',m,EFF_DELAY_FRAMES,0x00,0x3c,EFF_CUSTOM_RC,0x02,
                                  EFF_RGB,0x00,0xc8,0x64,EFF_SCROLL_LEFT,"      ",t,'^',' ',EFF_DELAY_FRAMES,0x01,0x00,
                                  EFF_RGB,0xd3,0x54,0x00,EFF_SCROLL_LEFT,"      ",daysOfTheWeek[now.dayOfTheWeek()],' ',EFF_DELAY_FRAMES,0x01,0x00,
                                  EFF_RGB,0x00,0x80,0x80,EFF_SCROLL_LEFT,"     ",now.day(),'-',now.month(),EFF_DELAY_FRAMES,0x01,0x00,
                                  EFF_SCROLL_LEFT,"     ",
                                  EFF_HSV_AH,0x00,0xff,0xff,0xff,0xff,0xff,EFF_SCROLL_LEFT,EFF_FRAME_RATE,0x02,"    ",curMessage,"     ",EFF_FRAME_RATE,0x00,
                                  EFF_CUSTOM_RC,0x01);


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
        FastLED.show();
      }
      else{
        StaticgMsg.SetText((unsigned char *)txtDateB, sizeof(txtDateB) - 1);
        StaticgMsg.UpdateText();
        FastLED.show();
      }
    
      for (size_t jj = 0; jj < 100; jj++)
      {
        dnsServer.processNextRequest();             // delay 1000ms total
        server.handleClient();                      // WIFI and dns server request
        delay(10);                                  // put this sequence in loop to reduce wifi waiting time
      }
    }
  }
  else
  {
    FastLED.show();
  }
  delay(10);
}