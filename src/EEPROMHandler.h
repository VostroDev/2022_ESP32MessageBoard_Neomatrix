/*----------------------------------------------------------------------------------------
  Platforms: ESP32 and ESP8266
  Language: C/C++/Arduino
  File: EEPROMHandler.h
  Parent: 2022_ESPMessageBoard_Neamatrix.cpp
  ----------------------------------------------------------------------------------------
  Description:
  ESP32 EEPROM
  Simple read and write string to EEPROM
----------------------------------------------------------------------------------------*/

#include <EEPROM.h>

void eepromWriteString(int address,String data){
  int _size = data.length();
  int i;
  for(i=0;i<_size;i++){
    EEPROM.writeChar(address+i,data[i]);
  }
  EEPROM.writeChar(address+_size,'\0');       // Add termination null character for String Data
  EEPROM.commit();                            // save to EEPROM flash
  delay(100);
}

String eepromReadString(int address, int16_t buffer_size){
  char data[buffer_size];
  int len=0;
  unsigned char k;
  k=EEPROM.readChar(address);
  while(k != '\0' && len<buffer_size){        // Read until null character
    k=EEPROM.readChar(address+len);
    data[len]=k;
    len++;
  }
  data[len]='\0';
  return String(data);
}

#if defined(ESP32)
  void eepromWriteChar(int address,char data){
    EEPROM.writeChar(address,data);
    EEPROM.commit();                            // save to EEPROM flash
    delay(100);
  }

  char eepromReadChar(int address){
    return EEPROM.readChar(address);
  }

  void eepromWriteInt(int address,int data){
    EEPROM.writeInt(address,data);
    EEPROM.commit();                            // save to EEPROM flash
    delay(100);
  }

  int eepromReadInt(int address){
    return EEPROM.readInt(address);
  }

  void eepromWriteByte(int address, byte data){
    EEPROM.write(address,data);
    EEPROM.commit();                            // save to EEPROM flash
    delay(100);
  }

  byte eepromReadByte(int address){
    return EEPROM.read(address);
  }
  
#elif defined(ESP8266)
  void eepromWriteChar(int address,char data){
    EEPROM.write(address,data);
    EEPROM.commit();                            // save to EEPROM flash
    delay(100);
  }

  char eepromReadChar(int address){
    return EEPROM.read(address);
  }

  void eepromWriteInt(int address,int data){    // sudo to emulate esp32
    EEPROM.write(address,data);
    EEPROM.commit();                            // save to EEPROM flash
    delay(100);
  }

  int eepromReadInt(int address){               // sudo to emulate esp32
    return EEPROM.read(address);
  }
#endif


