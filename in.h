#ifndef _in_h
#define _in_h
/*
 * Switches conected directly to a pin
 */

#include <Arduino.h>

#define inPins A7,A6
#define no_of_switchs 2
#define in_initHLa = B00000011  //{HIGH,HIGH}// the value to use to for on.

extern byte pinsA_in[no_of_switchs];
//for more than 8 switch inputs will need to change type of use array.
extern byte switchState;// 0 = off, 1 = on
extern byte switchOnVal;// eg 0 for pull the relay LOW to turn on, 1 for pull high. Pull opposite to turn off. Or should it be set to input to turn off?


void SetUpInputs();
boolean checkInput();
void SwitchesExe();

#endif
