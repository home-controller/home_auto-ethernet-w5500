#ifndef _relays_h
#define _relays_h

#include <Arduino.h>


#define no_of_relays 2
#define initHLa ={0,0}
#define relayPins 3,4
;
extern byte pinsA[no_of_relays];
extern byte relayState[no_of_relays];// 0 = off, 1 = on
extern byte relayOnVal[no_of_relays];// eg 0 for pull the relay LOW to turn on, 1 for pull high. Pull opisit to turn off. Or should it be set to input to turn off?

void SetUpRelays();

void UpdateRelayState(byte n, byte v);
void updateRelayState(byte key, char value[]);
void updateRelayState(char key[], char value[]);

void ToggleRelayState(byte n);


#endif
