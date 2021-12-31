/*----------------------------------------------------------------------------------------
  30/12/2021
  Author: R WILSON
  Platforms: ESP32
  Version: 1.0.0 - 30 DEC 2021
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

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ArduinoJson.h>

#include <FastLED.h>
#include <LEDMatrix.h>
#include <LEDText.h>
#include <FontMatrise.h>

#include <Wire.h>
#include "RTClib.h"

#define BUF_SIZE   400                      // 400 out of 512 used
#define PASS_BSIZE 40                       // 40 out of 512 used
#define PASS_EXIST 450                      // password $ address
#define PASS_BEGIN 460                      // password value stored
#define P_CHAR     '`'

#define LED_BUILTIN 26

#define LED_PIN 27
#define VOLTS   5
#define MAX_MA  400

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

cLEDText ScrollingMsg, StaticgMsg;

char txtDateA[] = { EFFECT_HSV_AH "\x00\xff\xff\xff\xff\xff" "12|30" };
char txtDateB[] = { EFFECT_HSV_AH "\x00\xff\xff\xff\xff\xff" "12:30" };
char szMesg[BUF_SIZE] = { EFFECT_FRAME_RATE "\x00" EFFECT_HSV_AH "\x00\xff\xff\xff\xff\xff" EFFECT_SCROLL_LEFT "     ESP32 MESSAGE BOARD BY R WILSON     "  EFFECT_CUSTOM_RC "\x01" };

void handleTimeUpdate(){
  String data = server.arg("plain");
  server.send(200, "text/json", "{\"status\" : \"ok\", \"time\" : \"" + data + "\"}"); // log to console
  delay(100);

  data.toCharArray(newTime, data.length() + 1);     // read new time
  newTimeAvailable = (strlen(newTime) != 0);        // if not a blank field

  Serial.print("new time: ");
  Serial.println(data);
}

void handleMessageUpdate(){
  String data = server.arg("plain");
  server.send(200, "text/json", "{\"status\" : \"ok\", \"message\" : \"" + data + "\"}"); // log to console
  delay(100);
  data.toCharArray(newMessage, data.length() + 1);  // read new message
  newMessageAvailable = true;

  Serial.print("new message: ");
  Serial.println(data);
}

void handleSettingsUpdate(){
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

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("\n\nScrolling display from your Internet Browser");

  //  RTC
  Wire.begin();                             // DS3231 RTC I2C - SDA(21) and SCL(22)
  Serial.print("\nRTC STARTING >>> ");                                  
  if (! RTC.begin()) {
    Serial.println("RTC NOT FOUND");
    while (1);
  }
  Serial.println("RTC STARTED");

  now = RTC.now(); 
  sprintf(szTime, "Time: %02d:%02d", now.hour(), now.minute());
  Serial.println(szTime);

  pinMode(LED_BUILTIN, OUTPUT);             // Heartbeat
  digitalWrite(LED_BUILTIN, LOW);

  //  EEPROM
  EEPROM.begin(512);

  Serial.println("\n\nEEPROM STARTED");

  curMessage[0] = newMessage[0] = '\0';

  eepromReadString(0,BUF_SIZE).toCharArray(curMessage,BUF_SIZE);  // Read stored msg from EEPROM address 0
  newMessageAvailable = false;
  Serial.print("Message: ");
  Serial.println(curMessage);
  
  //  START DISPLAY
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_MA);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds[0], leds.Size()).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(30);
  FastLED.clear(true);

  ScrollingMsg.SetFont(MatriseFontData);
  ScrollingMsg.Init(&leds, leds.Width(), ScrollingMsg.FontHeight() + 2, 0, 0);
  ScrollingMsg.SetText((unsigned char *)szMesg, sizeof(szMesg) - 1);
  ScrollingMsg.SetTextColrOptions(COLR_RGB | COLR_SINGLE, 0x00, 0x00, 0xff);

  StaticgMsg.SetFont(MatriseFontData);
  StaticgMsg.Init(&leds, leds.Width(), ScrollingMsg.FontHeight() + 2, 1, 0);
  StaticgMsg.SetText((unsigned char *)txtDateA, sizeof(txtDateA) - 1);
  StaticgMsg.SetTextColrOptions(COLR_RGB | COLR_SINGLE, 0x00, 0x00, 0xff);

  //  WIFI
  Serial.print("Connecting to ");
  Serial.println(ssid);  

  updateDefaultAPPassword();                  // get Wifi password from EEPROM

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

  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(DNS_PORT, "www.message.com", ip);
  Serial.println("DNS PORT: www.message.com");
  
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  server.begin();
  Serial.println("Server started");

  //  DISPLAY WELCOME MESSAGE
  while(ScrollingMsg.UpdateText() != 1)
  {
    FastLED.show();
    delay(20);
  }
  ScrollingMsg.SetText((unsigned char *)szMesg, sizeof(szMesg) - 1);   // rest to start of string
}

void loop()
{
  static uint32_t timeLast = 0;               // Heartbeat
  static uint8_t t = 0;                       // temperature
  
  if (millis() - timeLast >= 1000){
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    timeLast = millis();

    now = RTC.now();                          // Update the global var with current time 
    m = now.minute();  
    h = now.hour();
  }

  dnsServer.processNextRequest();             // dns server
  server.handleClient();                      // WIFI

  if (newMessageAvailable){
    strcpy(curMessage, newMessage);           // Copy new message to display
    eepromWriteString(0, newMessage);         // Write String to EEPROM Address 0
    newMessageAvailable = false;
    Serial.println("new message received, updated EEPROM");
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
      
  t = RTC.getTemperature() - 1;   // -1 is for calibration complard to fluke meter

  sprintf(txtDateA, "%c%c%c%c%c%c%c%02d%c%02d", EFF_HSV_AH,0x00,0xff,0xff,0xff,0xff,0xff,h,'|',m);
  sprintf(txtDateB, "%c%c%c%c%c%c%c%02d%c%02d", EFF_HSV_AH,0x00,0xff,0xff,0xff,0xff,0xff,h,':',m);

  sprintf(szMesg, "%c%c%c%c%c%c%c%c%c%c%s%02d%c%02d%c%c%c%c%c%c%c%c%c%c%s%02d%c%c%c%c%c%c%c%c%c%c%s%s%c%c%c%c%c%c%c%c%c%s%02d%c%02d%c%c%c%c%s%c%c%c%c%c%c%c%c%c%c%s%s%s%c%c%c%c",
                                  EFF_FRAME_RATE,0x00,EFF_HSV_AH,0x00,0xff,0xff,0xff,0xff,0xff,
                                  EFF_SCROLL_LEFT,"     ",h,':',m,EFF_DELAY_FRAMES,0x00,0x60,EFF_CUSTOM_RC,0x02,
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
  else if (rc == 2)         //EFFECT_CUSTOM_RC "\x02"
  {
    StaticgMsg.SetText((unsigned char *)txtDateA, sizeof(txtDateA) - 1);
    StaticgMsg.UpdateText();
    FastLED.show();
    delay(2000);
    StaticgMsg.SetText((unsigned char *)txtDateB, sizeof(txtDateB) - 1);
    StaticgMsg.UpdateText();
    FastLED.show();
    delay(2000);
    StaticgMsg.SetText((unsigned char *)txtDateA, sizeof(txtDateA) - 1);
    StaticgMsg.UpdateText();
    FastLED.show();
    delay(2000);
    StaticgMsg.SetText((unsigned char *)txtDateB, sizeof(txtDateB) - 1);
    StaticgMsg.UpdateText();
    FastLED.show();
    delay(2000);
    StaticgMsg.SetText((unsigned char *)txtDateA, sizeof(txtDateA) - 1);
    StaticgMsg.UpdateText();
    FastLED.show();
    delay(2000);
  }
  else
  {
    FastLED.show();
  }
  delay(10);
}