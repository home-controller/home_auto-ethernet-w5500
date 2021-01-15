#include "mqtt.h"
#include "pString.h"
#include "relays.h"



// Make sure to leave out the http and slashes!
// const char* mqtt_server = "test.mosquitto.org";

const char* mqtt_serverIp = "192.168.11.170";

//const char* rootTopic = "xh1/outside/light/s/e";//h1 for house 1, s = south & e = east

const char* relay1MqttTopic = "\x14h1/outside/light/s/e";//h1 for house 1, s = south & e = east
const char* relay2MqttTopic = "\x14h1/outside/light/e/s";
//void initVars(){
//  relay1MqttTopic[0] = sizeof(relay1MqttTopic) - 2;//-1 for 0 turm and 1 for length at [0]
//  relay2MqttTopic[0] = sizeof(relay2MqttTopic) - 2;//-1 for 0 turm and 1 for length at [0]
//}


//IPAddress MQTTserverIp(192, 168, 11, 170);


  EthernetClient ethClient;
  PubSubClient mqtt_client(ethClient);
void MqttPushRelayState(byte r){
  char buf[] = "s=0";
  buf[2] = 48 + relayState[r-1];
  if(r == 1){
    mqtt_client.publish(relay1MqttTopic+1, buf);    
  }
  if(r == 2){
    mqtt_client.publish(relay2MqttTopic+1, buf);    
  }
#if  no_of_relays >= 3  
  if(r == 3){
    mqtt_client.publish(relay3MqttTopic+1, buf);    
  }
#if  no_of_relays >= 4  
  if(r == 4){
    mqtt_client.publish(relay4MqttTopic+1, buf);    
  }
#if  no_of_relays >= 5  
  if(r == 5){
    mqtt_client.publish(relay5MqttTopic+1, buf);    
  }
#if  no_of_relays >= 6  
  if(r == 6){
    mqtt_client.publish(relay6MqttTopic+1, buf);    
  }
#if  no_of_relays >= 7  
  if(r == 7){
    mqtt_client.publish(relay7MqttTopic+1, buf);    
  }
#if  no_of_relays >= 8  
  if(r == 8){
    mqtt_client.publish(relay8MqttTopic+1, buf);    
  }
#if  no_of_relays >= 9  
  if(r == 9){
    mqtt_client.publish(relay9MqttTopic+1, buf);    
  }
#if  no_of_relays >= 10 
  if(r == 10){
    mqtt_client.publish(relay10MqttTopic+1, buf);    
  }
#endif //no_of_relays >= 3
#endif //no_of_relays >= 4
#endif //no_of_relays >= 5
#endif //no_of_relays >= 6
#endif //no_of_relays >= 7
#endif //no_of_relays >= 8
#endif //no_of_relays >= 9
#endif //no_of_relays >= 10
}

  
void MQTT_setup(){
//  Ethernet.begin(mac, ip);
//  initVars();
  mqtt_client.setServer(mqtt_serverIp , 1883);
  mqtt_client.setCallback(callback);
}



boolean reconnect() {
  static byte failDelay = 0;
  if(failDelay > 0){
    delay(100);
    failDelay--;
    return false;
  }
  // Loop until we're reconnected
  if (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt_client.connect("li_o")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqtt_client.publish("h1/outside/light","connected");
      // ... and resubscribe
      if(mqtt_client.subscribe((relay1MqttTopic + 1), 1) ) {Serial.println("Subscribed to topic"); }
      else {Serial.println("failed to subscribed"); }
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
//      delay(5000);
      failDelay = 45;
      return false;
    }
  }
  return true;
}
byte GetTopicRelay(char topic[]){
  if( StrComSZ(relay1MqttTopic, topic) ) { return 1; }
  if( StrComSZ(relay2MqttTopic, topic) ) { return 2; }
  return 0;
}
void callback(char* topic, byte* payload, unsigned int length) {
byte i;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  i = GetTopicRelay(topic);
  if(length == 1){
    Serial.println("Payload length  = 1");
    if( (payload[0] == '0') and (i > 0) ){ UpdateRelayState(i, (byte) 0); }
    else if( (payload[0] == '1') and (i > 0) ){ UpdateRelayState(i, (byte) 1); }
    else if( (payload[0] == 't') and (i > 0) ){ ToggleRelayState(i); }
  }
}
