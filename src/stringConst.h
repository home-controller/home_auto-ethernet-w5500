#ifndef _stringConst_h
#define _stringConst_h
#include <Arduino.h>


byte buildTopicForRelay(uint8_t relay, uint8_t r[], byte l = 20) ;
void printRelayTopics(byte r[], byte l);

#endif