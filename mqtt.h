#ifndef _mqtt_h
#define _mqtt_h
#include <Arduino.h>
#include <Ethernet.h>
#include <PubSubClient.h>

extern EthernetClient ethClient;
extern PubSubClient mqtt_client;

extern byte mac[];


boolean reconnect();
void callback(char* topic, byte* payload, unsigned int length);
void MQTT_setup();

#endif
