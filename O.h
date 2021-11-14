#ifndef _Out_h
#define _Out_h

#include <Arduino.h>


#define no_of_relays 8
//#define initHLa ={LOW,LOW,LOW,LOW,HIGH,HIGH,HIGH,HIGH}// the value to use to turn the relay etc. on. e.g. 0 for most relay modules, as they are pull low to turn on.
#define initHLa ={LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW}// the value to use to turn the relay etc. on. e.g. 0 for most relay modules, as they are pull low to turn on.
//#define relayPins 2,3,5,6,A0,A1,A2,A3 // 4 is SPI select for SD card.
#define relayPins 1,2,3,4,5,6,7,8 // For use with i2c expander IOA is relays and IOB is input by default.
#define defaultOffV 2 //2 is for set pin to input mode to turn off.



;
//extern bool using_i2c_io = true;
extern byte pinsA[no_of_relays];
extern byte relayState[no_of_relays];// 0 = off, 1 = on
extern byte relayOnVal[no_of_relays];// eg 0 for pull the relay LOW to turn on, 1 for pull high. Pull opposite to turn off. Or should it be set to input to turn off?
// Should maybe set pin to input to turn off instead? 

void SetUpRelays();

void UpdateRelayState(byte n, byte v, bool updateMqtt = true);
void updateRelayState(byte key, char value[]);
void updateRelayState(char key[], char value[]);

void ToggleRelayState(byte n);

// If using I2C MCP23017 expander
// registers
#define MCP23XXX_IODIR 0x00   //!< I/O direction register 1 = input, 0 = output
#define MCP23XXX_IPOL 0x01    //!< Input polarity register
#define MCP23XXX_GPINTEN 0x02 //!< Interrupt-on-change control register
#define MCP23XXX_DEFVAL                                                        \
  0x03 //!< Default compare register for interrupt-on-change
#define MCP23XXX_INTCON 0x04 //!< Interrupt control register
#define MCP23XXX_IOCON 0x05  //!< Configuration register
#define MCP23XXX_GPPU 0x06   //!< Pull-up resistor configuration register
#define MCP23XXX_INTF 0x07   //!< Interrupt flag register
#define MCP23XXX_INTCAP 0x08 //!< Interrupt capture register
#define MCP23XXX_GPIO 0x09   //!< Port register
#define MCP23XXX_OLAT 0x0A   //!< Output latch register

#define MCP23XXX_ADDR 0x20 //!< Default I2C Address
#define MCP23XXX_SPIREG                                                        \
  ADDRESSED_OPCODE_BIT0_LOW_TO_WRITE //!< SPI register type

#define MCP_PORT(pin) ((pin < 8) ? 0 : 1) //!< Determine port from pin number



#endif
