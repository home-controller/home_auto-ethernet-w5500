#ifndef _s_h
#define _s_h
/*
 * Stuff called on statup to.
 * Moved hear to clear up main.cpp
 */
#include <Arduino.h>
#include <Ethernet.h>
#include "defs.h"

#define W5500_RESET_PIN 9 //if changed update comments in defs.h
//#define inPins A7,A6

//getta's house
#define no_of_switchs 5
#define no_of_rooms 4 // for switches = outside, grooming, hall, kitchen. Only outside lights
#define in_initHLa = B00000011  //{HIGH,HIGH}// the value to use to for on.


#define NextBoardId 3
#define eepromIdAddr 250

extern IPAddress ip;//(192, 168, 1, 180);
extern IPAddress dns;//(192, 168, 11, 1);
extern IPAddress gateway;//(192, 168, 11, 1);
extern IPAddress subnet;//(255, 255, 240, 0);
extern EthernetServer server;//(80);
extern byte unit_id;
extern boolean EthernetConected;

extern byte mac[];

void initMqttVars();
void resetW5500();
boolean checkEthernet();
void startWebServer();
void printRelaysInfo();
byte IP_offsetSetup();
void ConnectEthernet();
boolean testEthernet();


//                                         0        1             2        3       4       5      6      7      8     9
extern const char room_names[] PROGMEM;// ={"kitchen,front room,meter room,bedroom,bathroom,office,utility,hall,outside,junk"};
extern byte room_names_i[10];
//const static char room_note[] PROGMEM ={"MarthaRubinHannaAsenathNorthEastSouthWestJosephRachelJohnGettalightswitchplug"};
                                         //012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
                                         //          1         2         3         4         5         6         7         8         9         10
extern byte room_note_i[15];// = {6,11,16,23,28,31,36,41,47,53,57,62,67,73,77};
//"h1/outside/light/se"; mqtt string format. h1= house 1. outside = from room_names array. se = South East.
// Format of record below. I think array in EEPROM is enough. bytes = 4 per
  // 0: byte: No. of lights         {B00011111: up to 32 per room}
  // 0: byte: No. of switch senders {B11100000: up to 8 per room}
  // 1: byte: room type             {B00001111: 0: kitchen, 1: front room, 2: meter room, 3: bedroom, 4: bathroom, 5: office, 6: utility, 7: hall, 8: outside, 9: junk
  // 1: byte: room coment id        {B11110000: 0:no comment, 1:Martha, 2:Rubin, 3:Hanna, 4:Asenath, 5:North, 6:East, 7:South, 8:West, 9:Joseph, 10:Rachel, 11:John, 12:Getta
  // 2: byte: first light pos.      {B11111111: the position of the first light in the room in the array of lights. 0 for unset. May need a word if more than 256 lights in house.
  // 3: byte: floor                 {B00000111: up to 8 floors}// even this many would run out of room for other stuff, if your flat starts at floor 100 then just count 100 as 0 :P

extern room_T rooms_A[no_of_rooms];

// Pins used; This is all the pins on a nano.
// 0, 1 for USB
// 14, 15 used for clock crystal, must be translated as A0 = 14 to
// 4 sd card select. maybe only shield, but might want card or eeprom module for others

// 9 for ethernet module reset. defined in s.h
// 10 eth select
// 11, 12, and 13 for SPI
// A4, A5 likley need for I2C at some point.

// A6,A7 Wall switch input
// D2 home net pin. As a plus this is one of the 2 int on pin change pins. The rest you int on a group with bitmasks etc. So use this to start with anyway.

//usable pins
// 3,5,6,7,8,A0,A1,A2,A3 defaults to relays with no expander A0 = 14 on nano




#endif
