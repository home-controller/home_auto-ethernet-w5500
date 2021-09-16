#ifndef _pin_in_h
#define _pin_in_h
#include <Arduino.h>

/*
relay is for output like using an IO pin for turning on a relays to ture on a light.
pin_in.h is for input eg. a wall switch conected to a IO pin, maybe throught I2C and an expander if running out of pins.
*/

// #define relayPins 2,3,5,6,A0,A1,A2,A3 used for output see relays.h

// I2C - SCL/SDA on Analog pins A4=clock and A5=data.

// used for the ethernet sheild:
// pin 9 on W5500 for reset see s.h
// SPI pins: 11, 12, 13 on Uno
// pin 10 SS for Ethernet shield.
// pin 4 SS for SD card.
/* So
pins:
0,1 serial, USB
2, 3 Relay 1 & 2
4 SS for SD card
5, 6 Relay 3 & 4
7, 8 unused. Maybe local switch network.
9 reset for W5500 module
10 SS for Ethernet
11, 12, 13 SPI
A0,A1,A2,A3 relay
A4, A5 IC2
A6, A7 Switches direct to pin
*/
#include <Arduino.h>


#define no_of_switches 2
#define initHLaS \
  { LOW, HIGH }           // input to count at on. #define _pwmPins {3, 5, 6, 9, 10, 11}
#define swichPins A6, A7  //Input only pins
;
extern byte s_pinsA[no_of_switches];
extern byte switchState[no_of_switches];  // 0 = off, 1 = on
extern byte switchOnVal[no_of_switches];  // = initHLaS;

void InitSwitches();
void CheckSwitches();
void AddSwitchTopic(char s[]);




#endif
