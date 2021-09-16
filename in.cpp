
#include "in.h"
#include <Arduino.h>
#include "relays.h"
#include <avr/wdt.h>
byte pinsA_in[no_of_switchs]  = { inPins };

//for more than 8 switch inputs will need to change type of use array.
byte switchState[no_of_switchs];// 0 = off, 1 = on
byte switchOnVal in_initHLa;// eg set bit to 1 for logic high being on(5v) and 0 for logic low being on.
byte newSwitchState; // needs to be same type as switchState.
byte deboMsk = 0;
unsigned long lastMils = millis();

boolean ReadSwitch(byte i){ //digitalRead() don't works with A6 and A7. Also moved looking up the pin from switch No. index here.
  byte pin;
  pin = pinsA_in[i];
  if ( (pin == A6) or (pin == A7) ){
    if ( analogRead(pin) > 600 ) {return HIGH; }
    return LOW;
  }
  return digitalRead(pin);
}

void SetUpInputs(){
  byte i;
  for(i=0; i<no_of_switchs;i++){
    pinMode(pinsA_in[i], INPUT);      // sets the input pins as input
    if(ReadSwitch(i) ) {
      bitSet(switchState, i);
    } else {
      switchState &= ~(1 << i); // bit clear;
    }
  }
  newSwitchState = switchState;
}


boolean checkInput(){
  byte i;
  // If any pins are A6 or A7 on arduino digital read don't work.
//#define _debug_switchs
#ifdef _debug_switchs
  Serial.println(F("entering checkInput()") );
#endif
  
  for(i=0; i<no_of_switchs;i++){
    if( ReadSwitch(i) ) {
      newSwitchState |= (1 << i);
    } else {
      newSwitchState &= ~(1 << i); // bit clear;
    }
#ifdef _debug_switchs  
  Serial.print(F("pin state: ") );  Serial.print(ReadSwitch(i) );  Serial.print(F(", i: ") );  Serial.print(i);  Serial.print(F(", pin: ") );  Serial.print(pinsA_in[i]);  Serial.print(F(", saved: b") );  Serial.print(switchState, BIN);  Serial.print(F(", New: b") );  Serial.print(newSwitchState, BIN);  Serial.print(F(", analogRead: ") ); Serial.println(analogRead(pinsA_in[i]));
#endif
  }
#ifdef _debug_switchs
  wdt_disable();
  delay(10000);
  wdt_enable(WDTO_8S);
#endif
  return (newSwitchState != switchState);
}

#define s1Msk B00000001
#define s2Msk B00000010
#define s3Msk B00000100

/** Check for changes no more than 1/10 of a second, If sending mqtt or web page may be longer?. Should be good for debounce?
  * If same switch changes again in less than 2 seconds add one to switch state to max of 8. 
  * stateVar      : 0b0000 0000 
  *  state        : 0b0000 0001  current state, incloulding updated mqtt etc. 0 = off 1 = on
  *  last state   : 0b0000 0010  State at last check. 0 = off 1 = on 
  *  change count : 0b0001 1100  Number of changes within 0.1 and 2 seconds betwean changes. max changes 8
  *  time         : 0b0110 0000 time in 1/2 seconds since last change. Within 1/2 second
**/  
void SwitchesExe(){
#ifdef _debug_switchs
  Serial.println(F("entering SwitchesExe()") );
#endif
  byte swMsk = s1Msk;
  static byte count = 0;
  if( (lastMils - millis() ) < (1000 * count) ) return ;
  if( (lastMils - millis() ) < 10000){ // wait 10 seconds betwean changes. Allow 1 update per switch in this time;
    deboMsk |= (newSwitchState ^ switchState);
    count++;
  } else {
    deboMsk = 0;
    count = 0;
    lastMils = millis();
  }
// 1: middle switch by front door.
  if( ((swMsk & switchState) != (swMsk & newSwitchState) ) 
  && ( (swMsk & deboMsk) == 0) ){
    if( (swMsk & newSwitchState) > 0){
      //Tern on outside lights    //void UpdateRelayState(byte n, byte v){// n = 1 for first relay
      UpdateRelayState(1, 1);
      UpdateRelayState(2, 1);
    } else {
      UpdateRelayState(1, 0);
      UpdateRelayState(2, 0);
    }
    switchState ^= swMsk;// only in this 'if' because old and new are diferent so flip old with xor to mach new now relays are updated :)
  }
// 2: switch light on cornner by tool room.
  swMsk = s2Msk;
  if( ((swMsk & switchState) != (swMsk & newSwitchState) )
  && ( (swMsk & deboMsk) == 0) ){
    if( (swMsk & newSwitchState) > 0){
      //Tern on outside lights    //void UpdateRelayState(byte n, byte v){// n = 1 for first relay
      UpdateRelayState(3, 1);
    } else {
      UpdateRelayState(3, 0);
    }
    switchState ^= swMsk;// only in this 'if' because old and new are diferent so flip old with xor to mach new now relays are updated :)
  }
}
