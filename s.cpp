// Starting functions moved from the main.ino
#include "s.h"
#include <EEPROM.h>
#include "relays.h"
#include "mqtt.h"
#include "pString.h"

byte unit_id;
byte netwokState = 0;

void resetW5500(){
    pinMode(W5500_RESET_PIN, OUTPUT);
    digitalWrite( W5500_RESET_PIN, LOW);
    delay(10);
    digitalWrite( W5500_RESET_PIN, HIGH);
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
  unit_id = id;
  return id;
}

void initMqttVars(){
  NumToStr(unit_id+1000 , id_as_str, 5);
  id_as_str[1]= 'c';
  relayMqttTopicBase[0] = 0;// String length set to zero
  relayMqttTopicBase[unit_prefix_len+1] = 0; // Make sure there is a Zero at end of string
  addSc(relayMqttTopicBase, house_id, 10);
  if ( unit_id > 99) {addSc(relayMqttTopicBase, "/c", unit_prefix_len); }
  else if(unit_id > 9) {addSc(relayMqttTopicBase, "/c0", unit_prefix_len); }
  else  {addSc(relayMqttTopicBase, "/c00", unit_prefix_len); }
  addSC(relayMqttTopicBase, (unit_id % 10) + '0', unit_prefix_len);
  addSC(relayMqttTopicBase, '/', unit_prefix_len);
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
  Serial.print(F("Web server on this board is at: ") );
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

bool checkEthernet(){
  // Check for Ethernet hardware present
  Serial.print(F("Ethernet shield: "));
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.print("was not found.");
    return false;
  }
  else if (Ethernet.hardwareStatus() == EthernetW5100) {
    Serial.print(F("W5100") );
  }
  else if (Ethernet.hardwareStatus() == EthernetW5200) {
    Serial.print(F("W5200") );
  }
  else if (Ethernet.hardwareStatus() == EthernetW5500) {
    Serial.print(F("W5500"));
  }
  else {Serial.print(F("chip un")); }
  Serial.println(F(" detected."));
  Serial.print(F("Link status: ") );
  if (Ethernet.linkStatus() == Unknown) {
    Serial.println(F("unknown. Detection only on W5200 & W5500.") );
  } else {
    Serial.println(F("Ethernet cable "));
    if (Ethernet.linkStatus() == LinkOFF) {// Not work on 5100
      Serial.println(F("not ") );
    }
    Serial.println(F("connected.") );
  }
  return true;
}
