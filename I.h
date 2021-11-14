#ifndef _in_h
#define _in_h
/*
 * Switches conected directly to a pin
 */

#include <Arduino.h>

#define inPins A7,A6
#define no_of_switchs 5
#define in_initHLa = B00000011  //{HIGH,HIGH}// the value to use to for on.

/*
 * simple count meaning:
 * 1: tog
 * 2, 3: all hard on/off
 */

//extern byte pinsA_in[no_of_switchs];
extern byte pinsA_in[];
/*pinsA_in array with each byte:
 *      
 *        
 */
//for more than 8 switch inputs will need to change type of use array.
extern byte switchState[no_of_switchs];// 0 = off, 1 = on
//extern byte switchOnVal;// eg 0 for pull the relay LOW to turn on, 1 for pull high. Pull opposite to turn off. Or should it be set to input to turn off?

extern byte switchInfo[no_of_switchs][3];


void SetUpInputs();
boolean checkInput();
void SwitchesExe();

#endif
