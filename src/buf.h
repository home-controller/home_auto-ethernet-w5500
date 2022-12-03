// used buf as is short. but meant to be generale storage like eeprom, SD card, Program mem etc.
#ifndef _buf_h
#define _buf_h
#include <Arduino.h>


class buf_e {//store strings in eeprom
  public:
    buf_e(byte start, word end_i);
    void addc(char s[]);// add 0 term str.
    void set(byte i, char s[]);
    void getStr(byte i, char s[]);
  private:
    word find_addr(byte i);
    byte eeprom_start;
    byte current_i;
    word last_pos_addr;
    byte str_count;
    word max_addr;
};



#endif
