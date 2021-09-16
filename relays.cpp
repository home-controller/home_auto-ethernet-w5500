//#include "html.h"
#include "relays.h"
#include "pString.h"
#include "html.h"
#include "mqtt.h"
#include "defs.h"
#include <Arduino.h>
#include "s.h"

;

byte pinsA[no_of_relays]  = { relayPins };

byte relayState[no_of_relays];
byte relayOnVal[no_of_relays] initHLa ;// eg 0 for pull the relay LOW to turn on, 1 for pull high. Pull opisit to turn off. Or should it be set to input to turn off?
byte relayOffVal[no_of_relays];//0: pull low to turn of, 1: pull high, 2: set pin to INPUT mode.

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
  Serial.print( F("Toggle light, light state: ") );
  Serial.println(relayState[n-1] );
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

void UpdateRelayStateI2c(byte n, byte v){
  if(v==0){
    if(relayOnVal[n]==0){ expand_io.setPin(pinsA[n], 1);}
    else {expand_io.setPin(pinsA[n], 0);}
  } else if(v==1){
    expand_io.setPin(pinsA[n], relayOnVal[n]);
  }
}

void UpdateRelayState(byte n, byte v){// n = 1 for first relay
  //This should be the only function used to turn the relays on/off etc.
  // v = 0 to turn off & 1 to turn on
#ifdef _debug_relays
  ;
  Serial.print( F("UpdateRelayState called with relay = ") );
  Serial.print(n);
  Serial.print( F(" and value = ") );
  Serial.print(v);
#endif  
  if (n > no_of_relays){
    Serial.println( F("Trying to switch more relays than we have.") );
    return;
  }
  if(n<1){
    Serial.println( F("Relay No. can't be < 1") );
    return;
  }
  n--;//as the arrays start at 0 for the first elemet 0 will be relay 1 from here.
#ifdef _debug_relays
  Serial.print(F(", pin = "));
  Serial.println(pinsA[n] );
#endif  
  relayState[n] = v;
  #if uses_I2C__ == 0
    UpdateRelayStatePins(n, v);
  #else
    UpdateRelayStateI2c(n, v);
  #endif
  MqttPushRelayState(n+1);//n had 1 subtracted above to work with 0 index arrays so +1 here.
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

// +++++++++++++++++++++++++++++++++++++++++++++++ i2c expander class +++++++++++++++++++++++++++++++++++++++++++
#include <Wire.h>
bool wireSetup = false;
I2C_expander_IO::I2C_expander_IO(){
}
void I2C_expander_IO::init(){
  if (!wireSetup){
    Wire.begin(); //creates a Wire objec
    wireSetup = true;
  }
  updateOutPins();
  
}
void I2C_expander_IO::setOutPins(word pinsBitmap){// set I/O pins to outputs or input
  io_input = pinsBitmap;
  updateOutPins();
}

void I2C_expander_IO::updateOutPins(){// set I/O pins to outputs or input
  byte ioA,ioB;
  ioA = lowByte(io_input);// io_input is a class var
  ioB = io_input >> 8;
  Wire.beginTransmission(i2c_control); //begins talking to the slave device
  Wire.write(0x00); //selects the IODIRA register
  Wire.write(ioA); //this sets all port A pins to outputs
  Wire.endTransmission(); //stops talking to device
  Wire.beginTransmission(0x20);//begins talking again to slave device
  Wire.write(0x01); //selects the IODIRB register
  Wire.write(ioB); // sets all port B pins to input(bit = 1) or outputs(bit = 0)
  Wire.endTransmission(); //ends communication with slave device

}
void I2C_expander_IO::setPinA(byte bitMap)
{
  Wire.beginTransmission(i2c_control); //starts talking to slave device
  Wire.write(0x12); //selects the GPIOA pins
  Wire.write(bitMap); // turns on/off pins of GPIOA
  Wire.endTransmission(); //ends communication with the device
}

void I2C_expander_IO::setPinB(byte bitMap)
{
  Wire.beginTransmission(i2c_control); //starts talking to slave device
  Wire.write(0x13); //selects the GPIOB pins
  Wire.write(bitMap); //turns on/off pins of GPIOA
  Wire.endTransmission();//ends communication with the device
}

void I2C_expander_IO::setPin(byte pin, byte on){
  
}
