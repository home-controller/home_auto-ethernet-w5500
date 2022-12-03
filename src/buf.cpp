

#include "buf.h"
#include <Arduino.h>

#include <EEPROM.h>


buf_e::buf_e(byte start, word max_size){
  eeprom_start = start;
  max_addr = eeprom_start + max_size;
  current_i = 0;
  str_count = 0;
  last_pos_addr = eeprom_start;
}

word buf_e::find_addr(byte i){
  byte string_len, x;
  if(current_i > i){
    current_i = 0;
    last_pos_addr = eeprom_start;
  }
  for(x=current_i; x < i; x++){
    string_len = EEPROM.read(last_pos_addr);
    last_pos_addr += string_len + 1;
  }
  return last_pos_addr;
}

void buf_e::addc(char s[]){
  word i = 0;
  find_addr(str_count);//index start at 0 so count will give addrs past end of last string.
  //if(t_addr
  while( (last_pos_addr + i <= max_addr) and (s[i]) != 0 ){
    EEPROM.update(last_pos_addr+i+1, s[i]);
  }
  EEPROM.update(last_pos_addr, i);
  str_count++;
}

void buf_e::getStr(byte i, char s[]){
  byte x, l;
  if(i<= str_count){
    find_addr(i);
    l = EEPROM.read(last_pos_addr);
    s[0] = l;
    for(x=1;x<=l; x++){
      s[x] = EEPROM.read(last_pos_addr+x);
    }
  } else {
    s[0] = 0;
  }
}