// Starting functions moved from the main.ino
#include "s.h"
#include <EEPROM.h>
#include "relays.h"
#include "mqtt.h"
#include "pString.h"
#include <avr/wdt.h>

byte unit_id;

boolean EthernetConected = false;

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
  unit_id = id;
  return id;
}

void initMqttVars(){
  cTo_pString( relayMqttTopicBase );
  if(unit_id > 9) {relayMqttTopicBase[5] = (unit_id / 10) + '0'; }
  relayMqttTopicBase[6] = (unit_id % 10) + '0';
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

boolean testEthernet(){
static unsigned long EthlastTestMils = millis();
const unsigned long millsD = (unsigned long)2 * 60 * 1000;// should be 2 minutes
  unsigned long t;
  unsigned long t2;
  t = millis();
  t2 = t - EthlastTestMils;
#ifdef _debugTimings
  Serial.print(F("2 minutes in mills: ") );
  Serial.println(millsD);
  Serial.print(F("now        : ") );
  Serial.println(t);
  Serial.print(F("saved      : ") );
  Serial.println(EthlastTestMils);
  Serial.print(F("now - saved: ") );
  Serial.println(t2);
  delay(1000);
#endif
  if( t2 > millsD){// 2 minutes
    //EthlastTestMils = millis();
    if(!checkEthernet() ){
      ConnectEthernet();
    }
    EthlastTestMils =t;
  }
}

boolean checkEthernet(){
  // Check for Ethernet hardware present
  EthernetConected = false;
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println(F("Ethernet shield was not found.") );  //Sorry, can't run without hardware. :(
  } else {
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println(F("Ethernet cable is not connected.") );
    }
    else EthernetConected = true;
  }
  return EthernetConected;
}

void ConnectEthernet(){
  // can be a problem to leave internet module reset line floting?
  resetW5500(); //so reset it as it can also need a reset with power fail to module.
  delay(500);
  wdt_reset();
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip, dns, gateway,subnet);

  //wdt_reset();
  // Check for Ethernet hardware present
  //Serial.println(F("Call checkEthernet") );
  wdt_reset();
  if (checkEthernet() ){//check for ethernet hardware e.g shield & cable conectted
    // start the web server (on port 80)
    //Serial.println(F("Call startWebServer()") );
    wdt_reset();
    startWebServer();
  }
}
