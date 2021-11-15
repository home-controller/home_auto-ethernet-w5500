#ifndef _Out_h
#define _Out_h

#include <Arduino.h>
#include "IO.h"


;
extern I2C_expander_IO expand_io;

//extern bool using_i2c_io = true;
extern byte const no_of_relays;

extern byte pinsA[];
extern byte pinsTypeA[];//0: mcu pins, 1: i2c_MCP23017, 5:shift register
extern byte relayState[];// 0 = off, 1 = on
extern byte relayOnVal[];// eg 0 for pull the relay LOW to turn on, 1 for pull high. Pull opposite to turn off. Or should it be set to input to turn off?
extern byte relayOffVal[];//0: pull low to turn of, 1: pull high, 2: set pin to INPUT mode.
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
