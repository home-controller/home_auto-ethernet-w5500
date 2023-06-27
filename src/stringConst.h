#ifndef _stringConst_h
#define _stringConst_h
#include <Arduino.h>

extern char relayMqttTopicBase[] = "#h2/i01/";  // h will need setting to house/building nNo. and i to unit last part of IP address.
                                                // and # to string length, i.e. 6. {7,"h2/c03/"}

byte buildTopicForRelay(uint8_t relay, uint8_t r[], byte l = 20);
void printRelayTopics(byte r[], byte l);

#endif