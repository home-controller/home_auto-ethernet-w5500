
#include "I.h"
#include "defs.h"
#include <Arduino.h>
#include "O.h"
#include <avr/wdt.h>
#include <EEPROM.h>
#include "mqtt.h"


byte pinsA_in[no_of_switchs]  = { inPins };

//for more than 8 switch inputs will need to change type of use array.
byte switchState[no_of_switchs];// 0 = off, 1 = on
byte switchOnVal in_initHLa;// eg set bit to 1 for logic high being on(5v) and 0 for logic low being on.
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
      switchState[i] = 11;
    } else {
      switchState[i] = 0;
    }
  }
}


void getInputStates(){
  //This updates the 'current state' bit in switchState array of switches for each switch.
  //This will overwrite the value withot updating any other bits, eg. count.
  byte i;
  // If any pins are A6 or A7 on arduino digital read don't work.
//#define _debug_switchs
#ifdef _debug_switchs
  Serial.println(F("entering checkInput()") );
#endif
  
  for(i=0; i<no_of_switchs;i++){
    if( ReadSwitch(i) ) {
      switchState[i] |= 0b10; 
      //newSwitchState |= (1 << i);
    } else {
      switchState[i] &= ~(0b10);
      //newSwitchState &= ~(1 << i); // bit clear;
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
}

#define s1Msk B00000001
#define s2Msk B00000010
#define s3Msk B00000100

/** Check for changes no more than 1/8 of a second, If sending mqtt or web page may be longer?. Should be good for debounce?
  * If same switch changes again in less than 2 seconds add one to switch state to max of 8. 
  * stateVar      : 0b0000 0000 
  *  state          : 0b0000 0001  Fully updated state, incloulding updated mqtt etc. 0 = off 1 = on
  *  current state  : 0b0000 0010  State at last check. Current switch state but count may still be updating etc. 0 = off 1 = on 
  *  change count   : 0b0000 1100  Number of changes within 0.1 and 2 seconds betwean changes. max changes 8, maybe 7. Use diference of bit 1 and 2 as first bit
  *  time         : 0b1111 0000 time in 1/8 seconds since last change. Within aprox 1/8 second
**/

void Switched(byte sw,byte count, byte state){
  byte e_index,i;
  word groupmask;
  if(sw>1) {
    sw--;
  }
  e_index = group_size * sw +groups_eeprom_start;
  EEPROM.get(e_index, groupmask);
  i=1;
  while (groupmask > 0){
    if(groupmask & 0b1){
      UpdateRelayState(sw,state);
    }
    i++;
    groupmask =  groupmask >> 1;
  }
  MqttPushSwitchState(i+1, state);
}

void SwitchesExe(){
  byte i, state1,state2, count1, time1;
#ifdef _debug_switchs
  Serial.println(F("entering SwitchesExe()") );
#endif
  byte swMsk = s1Msk;
  static byte count = 0;
  if( (lastMils - millis() ) < (125) ) return ; // 1/8th of a second
  getInputStates();//udate the curent switch state bit. Just that no turning on lights etc.
  for (i=0;i<no_of_switchs;i++){
    if(switchState[i] == 0 or switchState[i] == 0b11) { continue;}// If everything is up to date then skip to check next switch;
    state1 = switchState[i] & 0b1;
    state2 = (switchState[i] >> 1) & 0b1;
    count1 = (switchState[i] & 1100) >> 2;
    time1 = switchState[i] >> 4;
    if(state1 ==  state2){// work out if switch changed when state(see above) are the same
      if(count1 & 0b1){ // if count is an odd number then and states are the same then changed.
        count1++;
        time1 = 0;
      }
      else {
        if(count1 > 0) time1++;
      }// if no change then add to time since last switch
    } 
    else {
      if(count1 == 0){
        // todo call function to impliment the first quick switch
      }
      if( (count1 & 0b1) == 0){
        count1++;
        time1 = 0;
      }
      else {time1++;}// if no change then add to time since last switch
    }
    if (count1 >= 4 || time1 >= 0b1111){// reched max change count or time since last change > 2 seconds
      // todo call function to imlement changes 
      if (mqtt_client.connected()) {
        MqttPushSwitchState( (i+1), state2);
      }
      //and update switchState[i] to curent state
      switchState[i] = state2;
      switchState[i] &= (state2 << 1);
    } else {
      switchState[i] &= 0b11;
      switchState[i] |= (count1 << 2);
      switchState[i] |= (time1 << 4);
      
    }
    
  }
  lastMils = millis();
}
