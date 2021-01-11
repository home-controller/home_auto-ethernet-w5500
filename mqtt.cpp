#include "mqtt.h"

// Make sure to leave out the http and slashes!
// const char* mqtt_server = "test.mosquitto.org";

const char* mqtt_serverIp = "192.168.11.170";

//IPAddress MQTTserverIp(192, 168, 11, 170);


  EthernetClient ethClient;
  PubSubClient mqtt_client(ethClient);
void MQTT_setup(){
//  Ethernet.begin(mac, ip);
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
      mqtt_client.publish("h1/outside/light/s/e","test");
      // ... and resubscribe
      if(mqtt_client.subscribe("h1/outside/light/s/e", 1) ) {Serial.println("Subscribed to topic"); }
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

void callback(char* topic, byte* payload, unsigned int length) {
unsigned int i;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
