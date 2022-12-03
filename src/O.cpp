//#include "html.h"
#include "O.h" 
#include "pStr.h"
#include "mqtt.h"
#include "defs.h"
#include <Arduino.h>
#include "s.h"
#include "IO.h"

extern char value_on[3];// = {2, 'o', 'n'};
extern char value_off[4];
;

//declaire in main or setup
  //byte pinsA[no_of_relays]  = { relayPins };
  //byte pinsTypeA[no_of_relays];//0: mcu pins, 1: i2c_MCP23017, 5:shift register

//byte relayState[no_of_relays];
//byte relayOnVal[no_of_relays] initHLa ;// eg 0 for pull the relay LOW to turn on, 1 for pull high. Pull opisit to turn off. Or should it be set to input to turn off?
//byte relayOffVal[no_of_relays];//0: pull low to turn of, 1: pull high, 2: set pin to INPUT mode.

void SetUpRelays(){
  byte i;
  byte off = 0;// used to set type for func overloading
  for(i=0;i<no_of_relays;i++){
    //changed to. Put pin in input mode to turn off.
    relayOffVal[i] = defaultOffV;
//    pinMode(pinsA[i], OUTPUT);
//    digitalWrite(pinsA[i], relayOnVal[i] xor 0xFF);
//    relayState[i] = 0;
    //Serial.print(F("=========Call SetUpRelays(); i = ") );
    //Serial.println(i);
    UpdateRelayState(i+1, off); //turn all relays off at startup
  }
}


void ToggleRelayState(byte n){// n = 1 for first relay
//  Serial.print( F("Toggle light, light state: ") );
//  Serial.println(relayState[n-1] );
  UpdateRelayState(n, (relayState[n-1] xor B00000001) );
}

void UpdateRelayStatePins(byte n, byte v){
  if(v==0){
    if(relayOffVal[n] <=1){
      pinMode(pinsA[n], OUTPUT);
      digitalWrite(pinsA[n], relayOffVal[n]);
    } else if(relayOffVal[n] == 2) {// Thinking about using the other valuse for pwm dimming
      pinMode(pinsA[n], INPUT);
    }
  } else if(v==1){
    pinMode(pinsA[n], OUTPUT);
    digitalWrite(pinsA[n], relayOnVal[n]);
  }
}

void UpdateRelayStateI2c(byte n, byte v){// n = 1 for first relay
  if(n > 0){ n--; };// most arrays and mcu pins start at 0;
  if(v==0){
    if(relayOnVal[n]==0){ expand_io.setPin(pinsA[n], 1);}
    else {expand_io.setPin(pinsA[n], 0);}
  } else if(v==1){
    expand_io.setPin(pinsA[n], relayOnVal[n]);
  }
}

void UpdateRelayState(byte n, byte v, bool updateMqtt = true){// n = 1 for first relay
  //This should be the only function used to turn the relays on/off etc.
  // v = 0 to turn off & 1 to turn on
//#define _debug_relays
#ifdef _debug_relays
  ;
  Serial.print( F("UpdateRelayState called with relay = ") );
  Serial.print(n);
  Serial.print( F(" and value = ") );
  Serial.print(v);
#endif  
  if (n > no_of_relays){
#ifdef _debug_relays
    Serial.print( F("Trying to switch relay ") );Serial.print(n);    Serial.print( F(". We only have ") );Serial.println(no_of_relays);
#endif  
    return;
  }
  if(n<1){
#ifdef _debug_relays
    Serial.println( F("Relay No. can't be < 1") );
    #endif
    return;
  }
  n--;//as the arrays start at 0 for the first elemet 0 will be relay 1 from here.
//#define _debug_relays
#ifdef _debug_relays
  Serial.print(F("pin = "));
  Serial.println(pinsA[n] );
#endif  
  relayState[n] = v;
  if(pinsTypeA[n] == 0){//mcu pin
    //Serial.print(F("MCU: "));
    UpdateRelayStatePins(n+1, v);
  }
  else if(pinsTypeA[n] == 1){//1: i2c_MCP23017, 5:shift register
    //Serial.print(F("i2c_MCP23017: "));
    UpdateRelayStateI2c(n+1, v);
  }
  else{
    #ifdef _debug_relays
    Serial.print( F("Unknown expander type: ") );Serial.print(pinsTypeA[n]);
    #endif
  }
  if( updateMqtt and EthernetConected) MqttPushRelayState(n+1);//n had 1 subtracted above to work with 0 index arrays so +1 here.
  #ifdef _term_v
  Serial.print( F("Turn light ") );
  Serial.print(n+1);
  if (v == 0){
    Serial.println( F(" off") );
  }else if (v == 1){
    Serial.println(" on");
  }
  #endif
}

void updateRelayState(byte key, char value[]){
  byte v;

  if(StrCom(value, value_off) ){ v = 0; }
  else if(StrCom(value, value_on)  ){ v = 1; }
  else { return;}
  UpdateRelayState(key,v);
}

void updateRelayState(char key[], char value[]){
  byte n;
  if(StrCom(key, (char*)"\2l1")) { n=1; }
  else if (StrCom(key, (char*)"\2l2")) { n=2; }
  else { return;}
  updateRelayState(n,value);
}
