/*----------------------------------------------------------------------------------------
  15/03/2023
  Author: R WILSON
  Platforms: ESP32 ONLY - ESP8266 not supported
  Version: 5.0.0 - 15 Mar 2023
  Language: C/C++/Arduino
  v5.0.0 - 8 panel xlarge panel size version - FLEX PCB VERSION
  v4.0.0 - large panel size version - FLEX PCB VERSION (4 panel)
  v4.0.0 - validity check on message text
  ----------------------------------------------------------------------------------------
  Description:
  ESP32 connected to NeoPixel WS2812B LED matrix display (64x16x2 - 32x8 x 8 Panels)
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

   LED PINS 25 26 27 14
   ________ ________                                 ________ ________  ________ ________
  |________'________| (27) < 8x64x1  8x64x2 > 25,26 |________'________||________'________|
  |________'________| (13) < 8x64x1  8x64x2 > 27,14 |________'________||________'________|
      MATRIX_2x2                                                  MATRIX_4x2                                     
----------------------------------------------------------------------------------------*/

#include <Arduino.h>

#include <WiFi.h>
#include "SPIFFS.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <ArduinoJson.h>

#include "EEPROMHandler.h"                  // Storing message into permanent memory

#include <FastLED.h>
#include <LEDMatrix.h>
#include <LEDText.h>
#include <FontP12x16RW.h>                   // Custom (modified) font

#include <Wire.h>
#include "RTClib.h"

#define BUF_SIZE    400                     // 400 out of 512 used
#define PASS_BSIZE  9                       // 8 digit password
#define PASS_EXIST  405                     // password $ address
#define PASS_BEGIN  410                     // password value stored
#define P_CHAR      '`'
#define BRT_ADDR    425                     // Brightness value stored (int = 4Bytes)

#define TDUR_ADDR   426                     // Settings byte data EEPROM 23 bytes
#define MSPEED_ADDR 427
#define MONLY_ADDR  428
#define TIMER_ADDR  429
#define TIMEG_ADDR  430
#define TIMEB_ADDR  431
#define TEMPR_ADDR  432
#define TEMPG_ADDR  433
#define TEMPB_ADDR  434
#define DOWR_ADDR   435
#define DOWG_ADDR   436
#define DOWB_ADDR   437
#define DATER_ADDR  438
#define DATEG_ADDR  439
#define DATEB_ADDR  440
#define MSGR_ADDR   441
#define MSGG_ADDR   442
#define MSGB_ADDR   443
#define tiRGB_ADDR  444
#define tpRGB_ADDR  445
#define dowRGB_ADDR 446
#define datRGB_ADDR 447
#define msgRGB_ADDR 448

#define BUZZER_PIN           23             // TODO 19 for lolin board 23 38p-board Buzzer pin
#define LED_PIN              25             // NeoPixel pin TOP LEFT  1/4 display
#define LED2_PIN             26             // NeoPixel pin TOP RIGHT 1/4 display
#define LED3_PIN             27             // NeoPixel pin BOT LEFT  1/4 display
#define LED4_PIN             14             // NeoPixel pin BOT RIGHT 1/4 display
#define LED_BUILTIN          5              // lolin buildin led on 5
#define VOLTS                5
#define MAX_MA               1000            // !change to 3000
#define MATRIX_TYPE          VERTICAL_ZIGZAG_MATRIX
#define MATRIX_TILE_WIDTH   -64             // width of EACH NEOPIXEL MATRIX (not total display)
#define MATRIX_TILE_HEIGHT   8              // height of each matrix
#define MATRIX_TILE_H        2 //1          // number of matrices horizontally (neg for reverse order)
#define MATRIX_TILE_V        2              // number of matrices vertically (neg for reverse order)

#define MATRIX_SIZE          (MATRIX_WIDTH*MATRIX_HEIGHT)
#define MATRIX_PANEL         (MATRIX_WIDTH*MATRIX_HEIGHT) 
#define MATRIX_TILE          abs(MATRIX_TILE_WIDTH*MATRIX_TILE_HEIGHT) // Number os LEDS in one tile
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

#define RW_RGB               0xd0           // R WILSON add to lib
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

byte BRIGHTNESS = 15;

int rc;                                     // custom return char for ledMatrix lib

byte timeDur = 10;                          // 23 bytes
byte msgSpeed = 3;
byte msgOnly = 0;
byte timeR = 0,   timeG = 255, timeB = 255;
byte tempR = 255, tempG = 0,   tempB = 0;
byte dowR  = 0,   dowG  = 255, dowB  = 0;
byte dateR = 0,   dateG = 0,   dateB = 255;
byte msgR  = 0,   msgG  = 255, msgB  = 255;
byte timeRGBHSV = EFF_HSV_AH;
byte tempRGBHSV = RW_RGB;                   // EFF_RGB,0x00,0xc8,0x64
byte dowRGBHSV  = RW_RGB;                   // EFF_RGB,0xd3,0x54,0x00
byte dateRGBHSV = RW_RGB;                   // EFF_RGB,0x00,0x80,0x80
byte msgRGBHSV  = EFF_HSV_AH;

char ssid[] = "DoitMessageBoard";          // Change to your name
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
char newTime[17] = "09.05.2022 12:00";
bool newMessageAvailable = true;
bool newTimeAvailable = false;

cLEDMatrix<MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, MATRIX_TYPE, MATRIX_TILE_H, MATRIX_TILE_V, VERTICAL_BLOCKS> leds;
cLEDText ScrollingMsg, StaticgMsg, RTCErrorMessage;

char txtDateA[] = { EFFECT_HSV_AH "\x00\xff\xff\xff\xff\xff" "09|45 " EFFECT_HSV_AH "\x00\xff\xff\xff\xff\xff" "13^ " };
char txtDateB[] = { EFFECT_HSV_AH "\x00\xff\xff\xff\xff\xff" "09:45 " EFFECT_HSV_AH "\x00\xff\xff\xff\xff\xff" "13^ " };
char szMesg[BUF_SIZE] = { EFFECT_FRAME_RATE "\x00" EFFECT_HSV_AH "\x00\xff\xff\xff\xff\xff" EFFECT_SCROLL_LEFT "           RW           "  EFFECT_CUSTOM_RC "\x01" };
//char szMesg[BUF_SIZE] = { EFFECT_FRAME_RATE "\x00" EFFECT_HSV_AH "\x00\xff\xff\xff\xff\xff" EFFECT_SCROLL_LEFT "           WELCOME TO NORTHLINK COLLEGE           "  EFFECT_CUSTOM_RC "\x01" };

String decToHexa(int n) {
  char hexaDeciNum[2];            // char array to store hexadecimal number
  int i = 0;                      // counter for hexadecimal number array
    while (n != 0) {
      int temp = 0;               // temporary variable to store remainder
      temp = n % 16;              // storing remainder in temp variable.
      if (temp < 10) {            // check if temp < 10
         hexaDeciNum[i] = temp + 48;
         i++;
      }
      else {
        hexaDeciNum[i] = temp + 55;
        i++;
      }
      hexaDeciNum[i]='\0';
      n = n / 16;
    }
     //for (int j = i - 1; j >= 0; j--) // printing hexadecimal number array in reverse order
     //cout << hexaDeciNum[j];
  return hexaDeciNum;
}

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

  if (error){                                               // Test if parsing succeeds.
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return "deserializeJson error";
  }

  BRIGHTNESS = int(doc["brightness"]);
  strcpy(newMessage, doc["message"]);

  Serial.print("message and brightness handle: ");
  Serial.println(json);
  
  newMessageAvailable = true;

  //return json;
  return "{\"status\" : \"ok\", \"data\" : \"" + json + "\"}";
}

String handleSettingsUpdate(uint8_t *data, size_t len){
  String returnstring;

  data[len] = '\0';
  String json = (char*)data;
  
  DynamicJsonDocument doc(JSON_OBJECT_SIZE(3) + 130);
  DeserializationError error = deserializeJson(doc, json);  // Deserialize the JSON document

  if (error){                                               // Test if parsing succeeds.
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

String handleAdvancedUpdate(uint8_t *data, size_t len){
  data[len] = '\0';
  String json = (char*)data;
  DynamicJsonDocument doc(JSON_OBJECT_SIZE(2) + 350);
  DeserializationError error = deserializeJson(doc, (char*)data);   // Deserialize the JSON document

  if (error){                                                // Test if parsing succeeds.
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return "deserializeJson error";
  }

  timeDur  = doc["timedur"];
  msgSpeed = doc["msgspeed"];
  msgOnly  = doc["msgonly"];
  timeR    = doc["timecolor"][0];
  timeG    = doc["timecolor"][1];
  timeB    = doc["timecolor"][2];
  tempR    = doc["tempcolor"][0];
  tempG    = doc["tempcolor"][1];
  tempB    = doc["tempcolor"][2];
  dowR     = doc["dowcolor"][0];
  dowG     = doc["dowcolor"][1];
  dowB     = doc["dowcolor"][2];
  dateR    = doc["datecolor"][0];
  dateG    = doc["datecolor"][1];
  dateB    = doc["datecolor"][2];
  msgR     = doc["msgcolor"][0];
  msgG     = doc["msgcolor"][1];
  msgB     = doc["msgcolor"][2];

  if(timeR==0 && timeG==0 && timeB==0) {
    timeRGBHSV = EFF_HSV_AH;
    timeG = 255; timeB = 255;
  }
  else {
    timeRGBHSV = RW_RGB;
  }

  if(tempR==0 && tempG==0 && tempB==0) {
    tempRGBHSV = EFF_HSV_AH;
    tempG = 255; tempB = 255;
  }
  else {
    tempRGBHSV = RW_RGB;
  }

  if(dowR==0 && dowG==0 && dowB==0) {
    dowRGBHSV = EFF_HSV_AH;
    dowG = 255; dowB = 255;
  }
  else {
    dowRGBHSV = RW_RGB;
  }

  if(dateR==0 && dateG==0 && dateB==0) {
    dateRGBHSV = EFF_HSV_AH;
    dateG = 255; dateB = 255;
  }
  else {
    dateRGBHSV = RW_RGB;
  }

  if(msgR==0 && msgG==0 && msgB==0) {
    msgRGBHSV = EFF_HSV_AH;
    msgR = 0;
    msgG = 255;
    msgB = 255;
  }
  else {
    msgRGBHSV = RW_RGB;
  }

  eepromWriteByte(TDUR_ADDR, timeDur);
  eepromWriteByte(MSPEED_ADDR, msgSpeed);
  eepromWriteByte(MONLY_ADDR, msgOnly);
  eepromWriteByte(TIMER_ADDR, timeR);
  eepromWriteByte(TIMEG_ADDR, timeG);
  eepromWriteByte(TIMEB_ADDR, timeB);
  eepromWriteByte(TEMPR_ADDR, tempR);
  eepromWriteByte(TEMPG_ADDR, tempG);
  eepromWriteByte(TEMPB_ADDR, tempB);
  eepromWriteByte(DOWR_ADDR, dowR);
  eepromWriteByte(DOWG_ADDR, dowG);
  eepromWriteByte(DOWB_ADDR, dowB);
  eepromWriteByte(DATER_ADDR, dateR);
  eepromWriteByte(DATEG_ADDR, dateG);
  eepromWriteByte(DATEB_ADDR, dateB);
  eepromWriteByte(MSGR_ADDR, msgR);
  eepromWriteByte(MSGG_ADDR, msgG);
  eepromWriteByte(MSGB_ADDR, msgB);
  eepromWriteByte(tiRGB_ADDR, timeRGBHSV);
  eepromWriteByte(tpRGB_ADDR, tempRGBHSV);
  eepromWriteByte(dowRGB_ADDR, dowRGBHSV);
  eepromWriteByte(datRGB_ADDR, dateRGBHSV);
  eepromWriteByte(msgRGB_ADDR, msgRGBHSV);

  Serial.print("Settings Received: ");
  Serial.println(json);

  return "{\"status\" : \"ok\", \"data\" : \"" + json + "\"}";
}

void getSettings() {
  timeDur    = eepromReadByte(TDUR_ADDR);
  msgSpeed   = eepromReadByte(MSPEED_ADDR);
  msgOnly    = eepromReadByte(MONLY_ADDR);
  timeR      = eepromReadByte(TIMER_ADDR);
  timeG      = eepromReadByte(TIMEG_ADDR);
  timeB      = eepromReadByte(TIMEB_ADDR);
  tempR      = eepromReadByte(TEMPR_ADDR);
  tempG      = eepromReadByte(TEMPG_ADDR);
  tempB      = eepromReadByte(TEMPB_ADDR);
  dowR       = eepromReadByte(DOWR_ADDR);
  dowG       = eepromReadByte(DOWG_ADDR);
  dowB       = eepromReadByte(DOWB_ADDR);
  dateR      = eepromReadByte(DATER_ADDR);
  dateG      = eepromReadByte(DATEG_ADDR);
  dateB      = eepromReadByte(DATEB_ADDR);
  msgR       = eepromReadByte(MSGR_ADDR);
  msgG       = eepromReadByte(MSGG_ADDR);
  msgB       = eepromReadByte(MSGB_ADDR);
  timeRGBHSV = eepromReadByte(tiRGB_ADDR);
  tempRGBHSV = eepromReadByte(tpRGB_ADDR);
  dowRGBHSV  = eepromReadByte(dowRGB_ADDR);
  dateRGBHSV = eepromReadByte(datRGB_ADDR);
  msgRGBHSV  = eepromReadByte(msgRGB_ADDR);
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
  char txtRTCError[] = { EFFECT_FRAME_RATE "\x00" EFFECT_HSV_AH "\x00\xff\xff\xff\xff\xff" EFFECT_SCROLL_LEFT "         RTC NOT FOUND         "  EFFECT_CUSTOM_RC "\x99" }; //!char txtRTCError[BUF_SIZE]
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
  int gHue = 0, FRAMES_PER_SECOND = 60, FX_NUM_LEDS = 512;
  CRGB fleds[FX_NUM_LEDS];

  FastLED.addLeds<WS2812B,LED_PIN,GRB>(fleds, FX_NUM_LEDS).setCorrection(TypicalLEDStrip);   //std fastled for effects
  FastLED.addLeds<WS2812B,LED2_PIN,GRB>(fleds, FX_NUM_LEDS).setCorrection(TypicalLEDStrip);  //std fastled for effects
  FastLED.addLeds<WS2812B,LED3_PIN,GRB>(fleds, FX_NUM_LEDS).setCorrection(TypicalLEDStrip);  //std fastled for effects
  FastLED.addLeds<WS2812B,LED4_PIN,GRB>(fleds, FX_NUM_LEDS).setCorrection(TypicalLEDStrip);  //std fastled for effects

  FastLED.clear(true);
  FastLED.setBrightness(50);
  
  while(gHue <201){
    fadeToBlackBy(fleds, FX_NUM_LEDS, 20); // a colored dot sweeping back and forth, with fading trails
    int pos = beatsin16( 6, 0, FX_NUM_LEDS-1 );
    fleds[pos] += CHSV( gHue, 255, 192);
    FastLED.show();
    FastLED.delay(1000/FRAMES_PER_SECOND);
    
    EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
    EVERY_N_MILLISECONDS( 1000 ) {digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));}
  }
  // back to Matrixled
  FastLED.addLeds<WS2812B,  LED_PIN, GRB>(leds[0], 0,             MATRIX_TILE).setCorrection(TypicalLEDStrip);//TypicalSMD5050
  FastLED.addLeds<WS2812B, LED2_PIN, GRB>(leds[0], 1*MATRIX_TILE, MATRIX_TILE).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, LED3_PIN, GRB>(leds[0], 2*MATRIX_TILE, MATRIX_TILE).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, LED4_PIN, GRB>(leds[0], 3*MATRIX_TILE, MATRIX_TILE).setCorrection(TypicalLEDStrip);

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

void alarmCheck(){
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
  else if (dow == 5){            // Fri
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
    if(h == 11 && m == 45 && lastAlarm != 103){
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

void setup(){
  delay(300);
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

  BRIGHTNESS = eepromReadInt(BRT_ADDR);                           // read Neomatrix brightness value
  Serial.print("NeoMatrix Brightness set to ");
  Serial.println(BRIGHTNESS);
  
  //  GET ADVANCED SETTINGS FROM EEPROM
  getSettings();                                                  

  //  START DISPLAY
  Serial.println("\nNEOMATRIX DIPLAY STARTED");

  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_MA);         //! 
  FastLED.addLeds<WS2812B,  LED_PIN, GRB>(leds[0], 0,             MATRIX_TILE).setCorrection(TypicalLEDStrip);//TypicalSMD5050
  FastLED.addLeds<WS2812B, LED2_PIN, GRB>(leds[0], 1*MATRIX_TILE, MATRIX_TILE).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, LED3_PIN, GRB>(leds[0], 2*MATRIX_TILE, MATRIX_TILE).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, LED4_PIN, GRB>(leds[0], 3*MATRIX_TILE, MATRIX_TILE).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear(true);

  ScrollingMsg.SetFont(Font12x16Data);
  ScrollingMsg.Init(&leds, leds.Width(), ScrollingMsg.FontHeight() + 1, 0, 0);
  ScrollingMsg.SetText((unsigned char *)szMesg, sizeof(szMesg) - 1);
  ScrollingMsg.SetTextColrOptions(COLR_RGB | COLR_SINGLE, 0x00, 0x00, 0xff);

  StaticgMsg.SetFont(Font12x16Data);
  StaticgMsg.Init(&leds, leds.Width(), ScrollingMsg.FontHeight() + 1, 2, 0); // >> 2 pixels
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

  server.on("/advanced", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/advanced.html", "text/html");
  });
  server.on("/advanced/send", HTTP_POST, [](AsyncWebServerRequest * request){},NULL, 
      [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
          String myResponse = handleAdvancedUpdate(data,len);
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
  
  AsyncElegantOTA.begin(&server);             //* OTA - Start ElegantOTA
  server.begin();
  Serial.println("SERVER STARTED");

  //  DISPLAY WELCOME MESSAGE
  fxSinlon();                                 // Display special startup effect
  while(ScrollingMsg.UpdateText() != 1)
  {
    FastLED.show();
    delay(30);
    EVERY_N_MILLISECONDS( 1000 ) {digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));}
  }
  ScrollingMsg.SetText((unsigned char *)szMesg, sizeof(szMesg) - 1);   // reset to start of string

  Serial.println("SETUP COMPLETE");
}

void loop(){
  static uint32_t timeLast = 0;               // Heartbeat
  static int t = 0;                           // temperature
  static int updatetemp = 9999;               // so updates temp at startup
  
  if (millis() - timeLast >= 1000 && msgOnly == 0){
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    timeLast = millis();

    now = RTC.now();                          // Update the global var with current time 
    m = now.minute();  
    h = now.hour();
    dow = now.dayOfTheWeek();

    if(++updatetemp > 10){                    // Update temperature every 10 sec, visual glitch
      t = RTC.getTemperature();               // +or- from this for calibration
      updatetemp = 0;
    }

    //txtDateA[7] = '2';// HRS//txtDateA[8] = '3';// HRS//txtDateA[10] = '5';// MIN//txtDateA[11] = '9';// MIN
    sprintf(txtDateA, "%c%c%c%c%c%c%c%02d|%02d %c%c%c%c%c%c%c%02d^ ", timeRGBHSV,timeR,timeG,timeB,0xff,0xff,0xff,h,m, tempRGBHSV,tempR,tempG,tempB,0xff,0xff,0xff,t);
    sprintf(txtDateB, "%c%c%c%c%c%c%c%02d:%02d %c%c%c%c%c%c%c%02d^ ", timeRGBHSV,timeR,timeG,timeB,0xff,0xff,0xff,h,m, tempRGBHSV,tempR,tempG,tempB,0xff,0xff,0xff,t);

    // char 128 is 9 wide space, char 128 is 9 wide space - alignment reasons
    if(curMessage[0] == '\0'){
      sprintf(szMesg,
               "%c%c          %c%c%c%c%c%c%c%02d:%02d %c%c%c%c%c%c%c%02d^%c%c%c%c%c%c\
               %c%c%c%c%c%c%c%s %c%c%c%c%c%c%c%02d-%02d%c%c%c%c\
               %c%c", 
        EFF_FRAME_RATE,0x00,timeRGBHSV,timeR,timeG,timeB,0xff,0xff,0xff,h, m, tempRGBHSV,tempR,tempG,tempB,0xff,0xff,0xff,t, 128, EFF_DELAY_FRAMES,0x00,0x2c, EFF_CUSTOM_RC,0x02, 
        dowRGBHSV,dowR,dowG,dowB,0xff,0xff,0xff,daysOfTheWeek[dow],dateRGBHSV,dateR,dateG,dateB,0xff,0xff,0xff,now.day(),now.month(),129, EFF_DELAY_FRAMES,0x00,0xee,
        EFF_CUSTOM_RC,0x01);
    }
    else{
     sprintf(szMesg,
               "%c%c          %c%c%c%c%c%c%c%02d:%02d %c%c%c%c%c%c%c%02d^%c%c%c%c%c%c\
               %c%c%c%c%c%c%c%s %c%c%c%c%c%c%c%02d-%02d%c%c%c%c\
               %c%c%c%c%c%c%c%c%c%s     %c%c\
               %c%c", 
        EFF_FRAME_RATE,0x00,timeRGBHSV,timeR,timeG,timeB,0xff,0xff,0xff,h, m, tempRGBHSV,tempR,tempG,tempB,0xff,0xff,0xff,t, 128, EFF_DELAY_FRAMES,0x00,0x2c, EFF_CUSTOM_RC,0x02, 
        dowRGBHSV,dowR,dowG,dowB,0xff,0xff,0xff,daysOfTheWeek[dow],dateRGBHSV,dateR,dateG,dateB,0xff,0xff,0xff,now.day(),now.month(),129, EFF_DELAY_FRAMES,0x00,0xee,
        msgRGBHSV,msgR,msgG,msgB,0xff,0xff,0xff,EFF_FRAME_RATE,msgSpeed,curMessage,EFF_FRAME_RATE,0x00,
        EFF_CUSTOM_RC,0x01);
    }

    alarmCheck();
  }
  else if (msgOnly == 1) { // Message only display
    sprintf(szMesg, "%c%c%c%c%c%c%c%c%c%c%s%s%s%c%c",EFF_FRAME_RATE,msgSpeed,msgRGBHSV,msgR,msgG,msgB,0xff,0xff,0xff,EFF_SCROLL_LEFT,"            ",curMessage,"       ",EFF_CUSTOM_RC,0x01);
  }

  if (newMessageAvailable){
    strcpy(curMessage, newMessage);           // Copy new message to display
    eepromWriteString(0, newMessage);         // Write String to EEPROM Address 0
    newMessageAvailable = false;
    Serial.println("new message received, updated EEPROM\n");
    delay(100);
    eepromWriteInt(BRT_ADDR, BRIGHTNESS);    // Write new brightness value
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
    for (int j = 2; j < 10; j++)                //timeDur  // want to start on even number to run drawline
    {
      if(j % 2 == 0){ //even
      FastLED.clear();
         StaticgMsg.SetText((unsigned char *)txtDateA, sizeof(txtDateA) - 1);
         StaticgMsg.UpdateText();
         leds.DrawLine(0, 0, 0, 7, CRGB(0, 0, 0)); // blank column 1, due to visual glitch text shifting << 1 pixel
      }
      else{
        FastLED.clear();
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

/*
LEDText.cpp in library needs to be edited to include this:
This adds RW_RGB(R,G,B,0,0,0) last 3 bytes are ignored

line31
    #define  RW_RGB                0xd0    //!RWILSON

line227
    case RW_RGB:                           //!R WILSON
    *opt = (*opt & (~COLR_MASK)) | ((((uint16_t)m_pText[*tp] & 0x0f) << 6) & COLR_MASK);
    col1[0] = m_pText[*tp + 1];
    col1[1] = m_pText[*tp + 2];
    col1[2] = m_pText[*tp + 3];
    *tp += 6;                                
    break;


0xd1 209 Ñ // free to be used
0xd2 210 Ò
0xd3 211 Ó
0xd4 212 Ô
0xd5 213 Õ
0xd6 214 Ö
0xd7 215 ×

sprintf(szMesg, "%c%c%c%c%c%c%c%c%c%c     %02d:%02d%c%c%c%c%c%c%c%c%c%c%c%c      %02d^ %c%c%c%c%c%c%c%c%c%c      %s %c%c%c%c%c%c%c%c%c%c      %02d-%02d%c%c%c      %c%c%c%c%c%c%c%c%c%s     %c%c%c%c",
                        EFF_FRAME_RATE,0x00,timeRGBHSV,timeR,timeG,timeB,0xff,0xff,0xff,                                //  %c%c%c%c%c%c%c%c%c
                        EFF_SCROLL_LEFT,h,m,EFF_DELAY_FRAMES,0x00,0x2c,EFF_CUSTOM_RC,0x02,                              //  %c     %02d:%02d%c%c%c%c%c
                        tempRGBHSV,tempR,tempG,tempB,0xff,0xff,0xff,t,EFF_DELAY_FRAMES,0x00,0xee,                       //  %c%c%c%c%c%c%c      %02d^ %c%c%c
                        dowRGBHSV,dowR,dowG,dowB,0xff,0xff,0xff,daysOfTheWeek[dow],EFF_DELAY_FRAMES,0x00,0xee,          //  %c%c%c%c%c%c%c      %s %c%c%c
                        dateRGBHSV,dateR,dateG,dateB,0xff,0xff,0xff,now.day(),now.month(),EFF_DELAY_FRAMES,0x00,0xee,   //  %c%c%c%c%c%c%c      %02d-%02d%c%c%c      
                        msgRGBHSV,msgR,msgG,msgB,0xff,0xff,0xff,EFF_FRAME_RATE,msgSpeed,curMessage,EFF_FRAME_RATE,0x00, //  %c%c%c%c%c%c%c%c%c%s     %c%c
                        EFF_CUSTOM_RC,0x01);  

*/