# 2022_ESP32MessageBoard_Neomatrix

## Description
This project is intended for use with an ESP32 microcontroller, DS3231 I2C RTC module and 4 8x8 NeoMatrix boards.
The display will scroll through a sequence displaying:
* Current Time
* Temperature
* Day of the week
* Day and Month
* Custom Message
The time and message can be set using the build in webserver.

## Getting Started
Project build with Visual Studio Code and PlatformIO.

![Matrix Display](https://github.com/VostroDev/2022_ESP32MessageBoard_Neomatrix/blob/master/doc/matrix.png)

![Web Server](https://github.com/VostroDev/2022_ESP32MessageBoard_Neomatrix/blob/master/doc/webserver.png)

### Dependencies
* Visual Studio Code: https://code.visualstudio.com/
* PlatformIO: https://platformio.org/
* NeoPixel NeoMatrix: https://www.adafruit.com/product/1487
* DS3231 RTC Module: https://www.adafruit.com/product/3013
* ESP32 Dev Board: https://www.adafruit.com/product/3405


## Authors

Contributors names and contact info

* [R Wilson](vostrodev@gmail.com)  

## Version History

* 1.0
    * Initial Release
* 1.1
    * Added brightness control
    
## Acknowledgments

All credit attributes of the various authors of the libraries used.

* [Adafruit - NeoMatrix Boards](https://www.adafruit.com/product/1487)
* [Adafruit - RTC Lib](https://github.com/adafruit/RTClib)
* [ArduinoJson - Benoit Blanchon](https://arduinojson.org)
* [FastLED - Daniel Garcia](https://fastled.io)
* [LEDText - A Liddiment](https://github.com/AaronLiddiment/LEDText)
* [LEDMatrix - A Liddiment](https://github.com/AaronLiddiment/LEDMatrix)
* [MTR Design - Datepicker](http://mtrdesign.github.io/mtr-datepicker/)
