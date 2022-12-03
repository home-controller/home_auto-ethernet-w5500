#ifndef _IO_h
#define _IO_h


/*
 * For any code needed to get the i2c etc. expander acting like the MCU pins, with equivelent functions to:
 *    pinMode()
 *    digitalWrite()
 *    digitalRead() 
 * and possably adding the analog equivilents later.
 * 
 * And also the bitmap vertions for each port register.
 */

/*
 * 
 */
#define defaultOffV 2 //2 is for set pin to input mode to turn off.
 
 
bool io_digitalWrite(byte pin, byte value);

// +++++++++++++++++++++++++++++++++++++++++++++++ i2c expander class +++++++++++++++++++++++++++++++++++++++++++
class I2C_expander_IO {// i.e. MCP23017 expander controlling relays or reading switches.
  // pin No. starts at 0
  public:
    I2C_expander_IO();
    void init();
    void setOutPins(word pinsBitmap);
    void updateOutPins();
    
    void setPin(byte pin, byte on);// set output IO pin on or off, high or low. 0 or 5v
    void setPinA(byte bitMap);// set output IO pin on or off, high or low. 0 or 5v.
    void setPinB(byte bitMap);// set output IO pin on or off, high or low. 0 or 5v.
    
    byte getPin(byte pin);// set output IO pin on or off, high or low. 0 or 5v
    byte getPinsA();// get bank A. As bitmap
    byte getPinsB();// get bank B. As bitmap
    word getPins();// get bank A and B = 1. As bitmap, lower 8 are bank A
    word getInPins();// get input IO values As bitmap. Any Output pins are removed.

    bool swichChanged();
    
  private:
    
  // I2C - SCL/SDA on Analog pins A4 and A5.
    byte i2c_SCL = A4; // i2c clock pin.  (SCL = Serial CLock)
    byte i2c_SDA = A5; // 12c data pin
    
    // i2c_control = control byte
    //              :B11110000 = B0010 fixed device addr = B0010 << 4
    //              :B00000010 = A0 pin / jumper / solder link etc. low = 0 ????
    //              :B00000100 = A1 pin / jumper / solder link etc
    //              :B00001000 = A2 pin / jumper / solder link etc
    //              :B00000001 = R/W bit, if 0 write or if 1 read
    byte i2c_control = 0b00100000;// fixed device addr(B0010 << 4) + (A0 << 1) + (A1 << 2) + (A2 << 3) + R/W bit
    word io_input = 0xff00; //(B00000000 << 8) + B11111111; // PAx set to Output(eg. switch relays) and PBx set to input(conect to switches) by default.
    byte lastIoValueA = 0; // If the IO pins are high or low
    byte lastIoValueB = 0; // If the IO pins are high or low
    
};

// extern I2C_expander_IO expand_io;

#endif
   
