#include <Wire.h>
#include <Arduino.h>
#include "IO.h"

/**
 * pin:
 *  0bOO011111  For pins first 5 bits(max32 is pin No.
 */
bool io_digitalWrite(byte pin, byte value){
  return 1;
}

// +++++++++++++++++++++++++++++++++++++++++++++++ i2c expander class +++++++++++++++++++++++++++++++++++++++++++
bool wireSetup = false;
I2C_expander_IO::I2C_expander_IO(){
 // init();
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
  lastIoValueA = bitMap;
}

void I2C_expander_IO::setPinB(byte bitMap)
{
  Wire.beginTransmission(i2c_control); //starts talking to slave device
  Wire.write(0x13); //selects the GPIOB pins
  Wire.write(bitMap); //turns on/off pins of GPIOA
  Wire.endTransmission();//ends communication with the device
  lastIoValueB = bitMap;
}

void I2C_expander_IO::setPin(byte pin, byte on){// pin 0 = first relay.
  byte t;
  on &= 0b1;
  if(pin < 8){
    t = ((lastIoValueA >> pin) bitand 1);
    if(t != on){
      t = (lastIoValueA) xor (1 << pin);
      setPinA(t);
    }
  }
  else if(pin < 16){
    t = ((lastIoValueB >> (pin - 8) ) bitand 1);
    if(t != on){
      t = (lastIoValueB) xor (1 << (pin - 8) );
      setPinB(t);
    }
  }
  
}
