// Starting functions moved from the main.ino
#include "s.h"
#include <EEPROM.h>
#include "relays.h"

void resetW5500(){
    pinMode(W5500_RESET_PIN, OUTPUT);
    digitalWrite(W5500_RESET_PIN, LOW);
    delay(10);
    digitalWrite(W5500_RESET_PIN, HIGH);
    delay(2*1000);
}    

  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

byte IP_offsetSetup(){
  byte id;
  id = EEPROM.read(eepromIdAddr);
  //id=0xFF; hard set of board id  
  if(id == 0xFF){
    Serial.print(F("Setting") );
    EEPROM.update(eepromIdAddr, NextBoardId);
    id = NextBoardId;
  } else {
    Serial.print(F("Got") );
  }
  Serial.print(F(" EEprom board ID:") );
  Serial.println(id);
  ip[3] += id;
  mac[5] += id;
  return id;
}

void printRelaysInfo(){
  byte i;
  Serial.print( F("Relay pins array size = ") );
  Serial.print(sizeof(pinsA) );
  Serial.print( F(" Pins: ") );
  for(i=0; i< sizeof(pinsA); i++){
    if(i>0) Serial.print(", ");
    Serial.print(pinsA[i]);
  }
  Serial.println();
}
  
void startWebServer(){
  server.begin();
  Serial.print(F("server is at ") );
  Serial.println(Ethernet.localIP());
  byte macBuffer[6];  // create a buffer to hold the MAC address
  Ethernet.MACAddress(macBuffer); // fill the buffer
  Serial.print( F("The MAC address is: ") );
  for (byte octet = 0; octet < 6; octet++) {
    Serial.print(macBuffer[octet], HEX);
    if (octet < 5) {
      Serial.print('-');
    }
  }
  Serial.println();
}

void checkEthernet(){
  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println(F("Ethernet shield was not found.  Sorry, can't run without hardware. :(") );
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println(F("Ethernet cable is not connected.") );
  }
}
