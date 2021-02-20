#ifndef _s_h
#define _s_h

#include <Arduino.h>
#include <Ethernet.h>

#define W5500_RESET_PIN 9

#define NextBoardId 3
#define eepromIdAddr 250

extern IPAddress ip;//(192, 168, 1, 180);
extern IPAddress dns;//(192, 168, 11, 1);
extern IPAddress gateway;//(192, 168, 11, 1);
extern IPAddress subnet;//(255, 255, 240, 0);
extern EthernetServer server;//(80);
extern byte unit_id;

extern byte mac[];

void initMqttVars();
void resetW5500();
void checkEthernet();
void startWebServer();
void printRelaysInfo();
byte IP_offsetSetup();




#endif
