#ifndef _s_h
#define _s_h
// Setup per unit vars here. e.g. the house and relays / lights etc.


#include <Arduino.h>
#include <Ethernet.h>
#include "relays.h"

#define uses_I2C__ 1

#ifdef uses_I2C__
extern I2C_expander_IO expand_io;
#endif

#define W5500_RESET_PIN 9

// id 0: Test uno board.
// id 2: main house Ouside lights + plan to add upstairs lights
// id 5: Getta's house outsid lights
#define NextBoardId 5
#define eepromIdAddr 250

#define house_id "h1"

extern byte netwokState;// = 0;

extern IPAddress ip;//(192, 168, 1, 180);
extern IPAddress dns;//(192, 168, 11, 1);
extern IPAddress gateway;//(192, 168, 11, 1);
extern IPAddress subnet;//(255, 255, 240, 0);
extern EthernetServer server;//(80);
extern byte unit_id;

extern byte mac[];

void initMqttVars();
void resetW5500();
bool checkEthernet();
void startWebServer();
void printRelaysInfo();
byte IP_offsetSetup();




#endif
