#ifndef _relays_h
#define _relays_h

#include <Arduino.h>

// moved to defs.h
#define no_of_relays 9
#define initHLa ={LOW,LOW,LOW,LOW,HIGH,HIGH,HIGH,HIGH,HIGH}// the value to use to turn the relay etc. on. e.g. 0 for most relay modules, as they are pull low to turn on.
#define relayPins 3,5,6,7,8,A0,A1,A2,A3 //if changed update defs.h
#define defaultOffV 2 //2 is for set pin to input mode to turn off.
;
extern byte pinsA[no_of_relays];
extern byte relayState[no_of_relays];// 0 = off, 1 = on
extern byte relayOnVal[no_of_relays];// eg 0 for pull the relay LOW to turn on, 1 for pull high. Pull opposite to turn off. Or should it be set to input to turn off?
// Should maybe set pin to input to turn off instead? 

void SetUpRelays();

void UpdateRelayState(byte n, byte v);
void updateRelayState(byte key, char value[]);
void updateRelayState(char key[], char value[]);

void ToggleRelayState(byte n);


#endif
