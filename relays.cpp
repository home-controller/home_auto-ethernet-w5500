//#include "html.h"
#include "relays.h"
#include "pString.h"
#include "html.h"

;

byte pinsA[no_of_relays]  = { relayPins };
byte relayState[no_of_relays];
byte relayOnVal[no_of_relays] initHLa ;// eg 0 for pull the relay LOW to turn on, 1 for pull high. Pull opisit to turn off. Or should it be set to input to turn off?


void updateRelayState(byte n, byte v){// n = 1 for first relay
  byte output;// if relay is pulled low for on this will be 0 if v is 1 and 1 if v = 0.
  if (n >= no_of_relays){
    Serial.println( F("Trying to switch more relays than we have.") );
    return;
  }
  if(n<1){
    Serial.println( F("Relay No can't be < 1") );
    return;
  }
  if(relayOnVal[n-1] == 0){
    if(v == 0) {output = HIGH; }//pull relay pin in1 HIGH to turn it off
    else if (v == 1){ output = LOW; } //pull relay in1 pin LOW to turn it on
    else {
      Serial.println( F("in fuc updateRelayState v out of range") );
      return;
    }
  } else output = v;
  Serial.print("v = ");
  Serial.println( v);
  Serial.print("output = ");
  Serial.println( output);
  Serial.print("pin = ");
  Serial.println(pinsA[n - 1] );
  relayState[n - 1] = v;
  if(v <= 1){
    Serial.print( F("Turn light ") );
    Serial.print(n - 1);
    if (v == 0){
      Serial.println( F(" off") );
      digitalWrite(pinsA[n - 1], output);
    
    }else if (v == 1){
      Serial.println(" on");
      digitalWrite(pinsA[n - 1], output);
    }
  }
}

void updateRelayState(byte key, char value[]){
  byte v;

  if(StrCom(value, value_off) ){ v = 0; }
  else if(StrCom(value, value_on)  ){ v = 1; }
  else { return;}
  updateRelayState(key,v);
}

void updateRelayState(char key[], char value[]){
  byte n;
  if(StrCom(key, (char*)"\2l1")) { n=0; }
  else if (StrCom(key, (char*)"\2l2")) { n=1; }
  else { return;}
  updateRelayState(n,value);
}
