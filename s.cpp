// Starting functions moved from the main.ino
#include "s.h"
#include <EEPROM.h>
#include "O.h"
#include "I.h"
#include "IO.h"
#include "mqtt.h"
#include "pStr.h"
#include <avr/wdt.h>

byte unit_id;

char value_on[3] = {2, 'o', 'n'};
char value_off[4] = {3, 'o', 'f','f'} ;

#define no_of_relays_ 8
byte const no_of_relays = no_of_relays_;
//#define initHLa ={LOW,LOW,LOW,LOW,HIGH,HIGH,HIGH,HIGH}// the value to use to turn the relay etc. on. e.g. 0 for most relay modules, as they are pull low to turn on.
#define initHLa ={LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW}// the value to use to turn the relay etc. on. e.g. 0 for most relay modules, as they are pull low to turn on.
//#define relayPins 2,3,5,6,A0,A1,A2,A3 // 4 is SPI select for SD card.
//                1,2,3,4,5,6,7,8
#define relayPins 0,1,2,3,4,5,6,7 // For use with i2c expander IOA is relays and IOB is input by default.
#define contrType 1,1,1,1,1,1,1,1 //0: mcu pins, 1: i2c_MCP23017, 5:shift register
#define defaultOffV 2 //2 is for set pin to input mode to turn off.

I2C_expander_IO expand_io;

byte pinsA[no_of_relays]  = { relayPins };
byte pinsTypeA[no_of_relays] = { contrType };//0: mcu pins, 1: i2c_MCP23017, 5:shift register

byte relayState[no_of_relays];
byte relayOnVal[no_of_relays] initHLa ;// eg 0 for pull the relay LOW to turn on, 1 for pull high. Pull opisit to turn off. Or should it be set to input to turn off?
byte relayOffVal[no_of_relays];//0: pull low to turn of, 1: pull high, 2: set pin to INPUT mode.


//                                          0        1             2        3       4       5      6      7      8     9
const static char room_names[] PROGMEM ={"kitchen,front room,meter room,bedroom,bathroom,office,utility,hall,outside,junk"};
byte room_names_i[10];
const static char room_note[] PROGMEM ={"MarthaRubinHannaAsenathNorthEastSouthWestJosephRachelJohnGettalightswitchplug"};
                                       //012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
                                       //          1         2         3         4         5         6         7         8         9         10
byte room_note_i[15] = {6,11,16,23,28,32,37,41,47,53,57,62,67,73,77};
// Format of record below. I think array in EEPROM is enough. bytes = 2 per
  // 0: byte: No. of lights         {B00011111: up to 32 per room}
  // 0: byte: No. of switch senders {B11100000: up to 8 per room}
  // 1: byte: room type             {B00001111: 0: kitchen, 1: front room, 2: meter room, 3: bedroom, 4: bathroom, 5: office, 6: utility, 7: hall, 8: outside, 9: junk
  // 1: byte: room coment id        {B01110000: 0:no comment, 1:Martha, 2:Rubin, 3:Hanna, 4:Asenath, 5:North, 6:East, 7:South, 8:West, 9:Joseph, 10:Rachel, 11:John, 12:Getta

byte groupsSateA[No_groups +1] = {No_groups};


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
byte switchInfo[no_of_switchs][3];
#define switchInfo_(X,A,B,C) switchInfo[ X ][0] = A ;  switchInfo[ X ][1] = B ; switchInfo[ X ][2] = C ;

room_T rooms_A[no_of_rooms];

void initMqttVars(){
  cTo_pString( relayMqttTopicBase );
  if(unit_id > 9) {relayMqttTopicBase[5] = (unit_id / 10) + '0'; }
  relayMqttTopicBase[6] = (unit_id % 10) + '0';
  //switchInfo:
  // 0: byte: room No.        {B00001111
  // 0: byte: pos in room     {B01110000: the switch possition, Clockwize direction: north, north east, east etc. 0: kitchen, 
  // 1: byte: door to         {B00001111: 0: kitchen, 1: front room, 2: meter room, 3: bedroom, 4: bathroom, 5: office, 6: utility, 7: hall, 8: outside, 9: junk
  // 1: byte: room coment id        {B11110000: 0:no comment, 1:Martha, 2:Rubin, 3:Hanna, 4:Asenath, 5:North, 6:East, 7:South, 8:West, 9:Joseph, 10:Rachel, 11:John, 12:Getta, 13:Outside light
  switchInfo_(0, (0 + (1<<4)), 0, 1); //{8 + 5<<4,  0, 1};    // 1st outside switch on north wall. first floor
  switchInfo_(1, 0 + (2<<4),  1, 1);    // 2nd outside switch on north wall.
  switchInfo_(2, 1 + (1<<4),  0, 0);    // switch grooming room(=1).
  switchInfo_(3, (2 + (7<<4)),  0, 1);  // 1st switch west side(hall=2).
  switchInfo_(4, 3 + (13<<4), 0, 0);    // 1st outside switch in kitchen(= room 3). ground floor

  groupSet(0,0b00000000,0b00000000);
  groupSet(1,0b00000000,0b00000100);
  groupSet(2,0b00000000,0b00010000);
  groupSet(3,0b00000000,0b01000000);
  groupSet(4,0b00000000,0b10000000);
  groupSet(5,0b00000000,0b10000001);

  groupSet(10,0b00000000,0b00001111);
  groupSet(11,0b00000000,0b00110000);
  groupSet(12,0b00000000,0b01100000);
  groupSet(13,0b00000000,0b10000011);
  groupSet(18,0b00000000,0b11111111,5);// switch all group to last switch pos.
  groupSet(7,0b00000000,0b11111111,0);// all group off
  
  //setSwitchGroups(byte switch, byte quick_i, byte normal_i, count2_i, byte count3_i, byte count4_i)
  setSwitchGroups(0 , 1, 10, 10, 18, 0);
  setSwitchGroups(1 , 2, 11, 11, 18, 0);
  setSwitchGroups(2 , 3, 12, 12, 18, 0);
  setSwitchGroups(3 , 3, 12, 12, 18, 0);
  //setSwitchGroups(3 , 0, 5, 18, 18, 0);
  setSwitchGroups(4 , 4, 13, 13, 18, 0);

  // setup room info //4 rooms: outside, grooming, hall, kitchen
  // rooms format
    // 0: byte: No. of lights         {B00011111: up to 32 per room} Set from relays array
    // 0: byte: No. of switchs        {B11100000: up to 8 per room} = << 5 Setup from switchInfo array.
    // 1: byte: room type             {B00001111: 0: kitchen, 1: front room, 2: meter room, 3: bedroom, 4: bathroom, 5: office, 6: utility, 7: hall, 8: outside, 9: junk
    // 1: byte: room coment id        {B11110000: <<4 0:no comment, 1:Martha, 2:Rubin, 3:Hanna, 4:Asenath, 5:North, 6:East, 7:South, 8:West, 9:Joseph, 10:Rachel, 11:John, 12:Getta
    // 2: byte: floor                 {B00000111: up to 8 floors}// even this many would run out of room for other stuff, if your flat starts at floor 100 then just count 100 as 0 :P

  // room 0 = outside.
    rooms_A[0].roomType = 8;//outside = 8.
    //rooms_A[0].roomType |= (0<<4);
    rooms_A[0].houseFloor = 0;
  // room 1 = grooming.
    rooms_A[1].lights = 0;
    rooms_A[1].lights |= (1<<5);
    rooms_A[1].roomType = 8;//outside = 8.
    //rooms_A[0].roomType |= (0<<4);
    rooms_A[1].houseFloor = 0;
  // room 2 = hall.
    rooms_A[2].lights = 0;
    rooms_A[2].lights |= (1<<5);
    rooms_A[2].roomType = 8;//outside = 8.
    //rooms_A[0].roomType |= (0<<4);
    rooms_A[2].houseFloor = 0;
  // room 3 = kitchen
    rooms_A[3].lights = 0;
    rooms_A[3].lights |= (1<<5);
    rooms_A[3].roomType = 8;//outside = 8.
    //rooms_A[0].roomType |= (0<<4);
    rooms_A[3].houseFloor = 0;
}


void printRelaysInfo(){
  byte i;
  Serial.print( F("Relay pins array size = ") );
  Serial.print(sizeof(pinsA) );
  Serial.println( F(" Pins: ") );
  for(i=0; i< sizeof(pinsA); i++){
    if(i>0) Serial.print(", ");
    Serial.print(pinsA[i]); // Serial.print(':');
    if(pinsTypeA[1] == 0)Serial.print(F(":MCU pin"));
    else if(pinsTypeA[1] == 1)Serial.print(F(":i2c_MCP23017"));
    else if(pinsTypeA[1] == 5)Serial.print(F(":Shift reg"));
    else Serial.print(F(":unknown"));
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
  return EthernetConected;
}

boolean checkEthernet(){
  // Check for Ethernet hardware present
  EthernetConected = false;
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println(F("Ethernet shield was not found.") );
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
