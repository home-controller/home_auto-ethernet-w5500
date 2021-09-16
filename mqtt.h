#ifndef _mqtt_h
#define _mqtt_h
#include <Arduino.h>
#include <Ethernet.h>
#include <PubSubClient.h>

;
extern EthernetClient ethClient;
extern PubSubClient mqtt_client;

#define unit_prefix_len 10 
extern char relayMqttTopicBase[];// eg = "h1/c01/" for house 1 and controler 1. each controler one the same network needs a different id.
extern const char* mqtt_serverIp; //server address as a string. i.e. your raspbery pi. mine is. "192.168.11.170"
extern char id_as_str[6];

extern byte mac[];


boolean mqtt_connecton_check();
boolean MqttConnect();
void callback(char* topic, byte* payload, unsigned int length);
void MQTT_setup();

void MqttPushRelayState(byte r);
//void (const char* s, const char* p, char* buf);  


#endif
