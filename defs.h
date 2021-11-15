#ifndef _defs_h
#define _defs_h

// debuging
//#define _debug_switchs

// serial terminal feadback & debug. Needs to be before #includes to work with that file.
#define _term_v
//#define _debug_relays
//#define _mqtt_debug
//#define _pString_debug

#ifdef __AVR_ATmega328P__
 #define ATmega328P_mcu
 #define ATmega328P
 #define  mem_eeprom 1024
 #define  mem_flash 32256
 #define mem_sram 2048
 #define _pwmPins {3, 5, 6, 9, 10, 11}//timer intafears with 5 & 6
 #define InOnlyPins = 2 // ATmega328P chips have 2 analog input only pins namely A6 & A7. Some older boards do not bring them out to pins but most do.
 #define _i2c__
#endif  

// Arduino board types
#define unknown_board  0
#define Uno_board  1
#define Nano_board 2

#define t_Ethershield 1
#define t_Module  2
#define _W5100  4
#define _W5200  8
#define _W5500  12


#if defined(ARDUINO_AVR_NANO)
  #define _Nano
  #define arduino_type Nano_board
#elif defined(ARDUINO_AVR_UNO)
  #define _Uno
  #define arduino_type Uno_board
#else
   #define arduino_type unknown_board 
#endif  

#ifdef _Uno
  #define eth_board_type  t_Ethershield
  #define eth_chip _W5100
#elif defined(_Nano)
  #define eth_board_type t_Module
  #define eth_chip _W5500
#endif 
#define eth_type (eth_board_type + eth_chip)

// Pins used; This is all the pins on a nano.
// 0, 1 for USB
// 14, 15 used for clock crystal, must be translated as A0 = 14 to
// 3,5,6,7,8,A0,A1,A2,A3 defined in relays.h A0 = 14 on nano
// 4 sd card select. maybe only shield, but might want card or eeprom module for others

// 9 for ethernet module reset. defined in s.h
// 10 eth select
// 11, 12, and 13 for SPI
// A4, A5 likley need for I2C at some point.

// A6,A7 Wall switch input
// D2 home net pin. As a plus this is one of the 2 int on pin change pins. The rest you int on a group with bitmasks etc. So use this to start with anyway.

//how sad, 1 pin short for 10 relays. Will have to use shift reg IC at some point I guess.

//==================================eeprom map========================
#define No_groups 20                // 16 groups. for simplisity switch 1 maps to group 1 for now.
#define group_size 3      // size in bytes. upto 16(16bits) relays with bitmap + 1 for opts.  If bitmap is 0 leave it to mqtt or web
#define groups_eeprom_start 20
// eeprom used 20 x 3 = 60. used address 20 to 79.

//switch to group index. 
#define No_switches_eeprom_res 20
#define switches_size 5 //5 bytes, 1 for quick, 1 for normal group, 3 for count 2, 3 and 4
#define switches_eeprom_start 80
// eeprom used 20 x 5 = 40. used address 80 to 179.

//#define eepromIdAddr 250 //in s.h

/*

#if defined(TEENSYDUINO) 

    //  --------------- Teensy -----------------

    #if defined(__AVR_ATmega32U4__)
        #define BOARD "Teensy 2.0"
    #elif defined(__AVR_AT90USB1286__)       
        #define BOARD "Teensy++ 2.0"
    #elif defined(__MK20DX128__)       
        #define BOARD "Teensy 3.0"
    #elif defined(__MK20DX256__)       
        #define BOARD "Teensy 3.2" // and Teensy 3.1 (obsolete)
    #elif defined(__MKL26Z64__)       
        #define BOARD "Teensy LC"
    #elif defined(__MK64FX512__)
        #define BOARD "Teensy 3.5"
    #elif defined(__MK66FX1M0__)
        #define BOARD "Teensy 3.6"
    #else
       #error "Unknown board"
    #endif

#else // --------------- Arduino ------------------

    #if   defined(ARDUINO_AVR_ADK)       
        #define BOARD "Mega Adk"
    #elif defined(ARDUINO_AVR_BT)    // Bluetooth
        #define BOARD "Bt"
    #elif defined(ARDUINO_AVR_DUEMILANOVE)       
        #define BOARD "Duemilanove"
    #elif defined(ARDUINO_AVR_ESPLORA)       
        #define BOARD "Esplora"
    #elif defined(ARDUINO_AVR_ETHERNET)       
        #define BOARD "Ethernet"
    #elif defined(ARDUINO_AVR_FIO)       
        #define BOARD "Fio"
    #elif defined(ARDUINO_AVR_GEMMA)
        #define BOARD "Gemma"
    #elif defined(ARDUINO_AVR_LEONARDO)       
        #define BOARD "Leonardo"
    #elif defined(ARDUINO_AVR_LILYPAD)
        #define BOARD "Lilypad"
    #elif defined(ARDUINO_AVR_LILYPAD_USB)
        #define BOARD "Lilypad Usb"
    #elif defined(ARDUINO_AVR_MEGA)       
        #define BOARD "Mega"
    #elif defined(ARDUINO_AVR_MEGA2560)       
        #define BOARD "Mega 2560"
    #elif defined(ARDUINO_AVR_MICRO)       
        #define BOARD "Micro"
    #elif defined(ARDUINO_AVR_MINI)       
        #define BOARD "Mini"
    #elif defined(ARDUINO_AVR_NANO)       
        #define BOARD "Nano"
    #elif defined(ARDUINO_AVR_NG)       
        #define BOARD "NG"
    #elif defined(ARDUINO_AVR_PRO)       
        #define BOARD "Pro"
    #elif defined(ARDUINO_AVR_ROBOT_CONTROL)       
        #define BOARD "Robot Ctrl"
    #elif defined(ARDUINO_AVR_ROBOT_MOTOR)       
        #define BOARD "Robot Motor"
    #elif defined(ARDUINO_AVR_UNO)       
        #define BOARD "Uno"
    #elif defined(ARDUINO_AVR_YUN)       
        #define BOARD "Yun"

    // These boards must be installed separately:
    #elif defined(ARDUINO_SAM_DUE)       
        #define BOARD "Due"
    #elif defined(ARDUINO_SAMD_ZERO)       
        #define BOARD "Zero"
    #elif defined(ARDUINO_ARC32_TOOLS)       
        #define BOARD "101"
    #else
       #error "Unknown board"
    #endif

#endif
*/
struct room_T{
  byte lights;
  byte roomType;
  byte houseFloor;// 0 is always the lowest floor. so if 10 underground floors then ground floor is floor 9
};
struct room_TE{// T for type def. E for expanded out the bits into vars
  room_T room;
  byte roomNumLights;
  byte roomSwitchCon;// switch sender/controller MCU
  byte type;
};

#endif
