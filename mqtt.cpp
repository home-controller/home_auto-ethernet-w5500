#include "mqtt.h"
#include "pString.h"
#include "relays.h"
#include "defs.h"



// Make sure to leave out the http and slashes!
// const char* mqtt_server = "test.mosquitto.org";

const char* mqtt_serverIp = "192.168.11.170";

//const char* rootTopic = "xh1/outside/light/s/e";//h1 for house 1, s = south & e = east

const char* relayMqttTopicBase = "\x11h1/outside/light/";
const char* relay1MqttTopic = "\3s/e";//h1 for house 1, s = south & e = east
const char relay2MqttTopic[] = {3,'e','/', 's'} ;// "\x03e" = 1 char = to 0x3E apparently ;(
#define tempStrMaxLen 30
char temp_pString[tempStrMaxLen + 1];
//void initVars(){
//  relay1MqttTopic[0] = sizeof(relay1MqttTopic) - 2;//-1 for 0 turm and 1 for length at [0]
//  relay2MqttTopic[0] = sizeof(relay2MqttTopic) - 2;//-1 for 0 turm and 1 for length at [0]
//}


//IPAddress MQTTserverIp(192, 168, 11, 170);


  EthernetClient ethClient;
  PubSubClient mqtt_client(ethClient);

void Publish(const char* s, char* bufz){
  const char p[7] = "\x6state/";
#ifdef _mqtt_debug
  Serial.print( F("Publish called with"));
  Serial.print( F(" Topic: ") );
  pPrint(s);
  Serial.print( F(", bufz: ") );
  Serial.println( bufz);
  Serial.print( F("prefix set to: ") ); 
  pPrintln(p);
#endif
  temp_pString[0] = 0;
  AddS_(temp_pString, p);
//  Serial.print( F("Set temp_pString to prefix: ") );
//  pPrintln(temp_pString);
  AddS_(temp_pString, relayMqttTopicBase);
//  Serial.print( F("Add topic base: ") );
//  pPrintln(temp_pString);
  AddS_(temp_pString, s);
//#ifdef _mqtt_debug
  Serial.print( F("MQTT publish. Topic: ") );
  pPrintln(temp_pString);
  mqtt_client.publish(temp_pString+1, bufz);
}
void MqttPushRelayState(byte r){
  Serial.print( F("MqttPushRelayState called with r = "));
  Serial.println(r);
  char bufz[] = "s=0";
  bufz[2] = 48 + relayState[r-1];
  if(r == 0){
    Serial.println("Error: The first relay is 1");
  }
  else if(r == 1){
    Publish(relay1MqttTopic, bufz);    
  }
  else if(r == 2){
    Publish(relay2MqttTopic, bufz);    
  }
#if  no_of_relays >= 3  
  if(r == 3){
    Publish(relay3MqttTopic, bufz);    
  }
#if  no_of_relays >= 4  
  if(r == 4){
    Publish(relay4MqttTopic, bufz);    
  }
#if  no_of_relays >= 5  
  if(r == 5){
    Publish(relay5MqttTopic, bufz);    
  }
#if  no_of_relays >= 6  
  if(r == 6){
    Publish(relay6MqttTopic, bufz);    
  }
#if  no_of_relays >= 7  
  if(r == 7){
    Publish(relay7MqttTopic, bufz);    
  }
#if  no_of_relays >= 8  
  if(r == 8){
    Publish(relay8MqttTopic, bufz);    
  }
#if  no_of_relays >= 9  
  if(r == 9){
    Publish(relay9MqttTopic, bufz);    
  }
#if  no_of_relays >= 10 
  if(r == 10){
    Publish(relay10MqttTopic, bufz);    
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
    Serial.print( F("Attempting MQTT connection..."));
    // Attempt to connect
    if (mqtt_client.connect("li_o")) {
      Serial.println( F("connected"));
      // Once connected, publish an announcement...
      mqtt_client.publish("h1/outside/light","connected");
      // ... and resubscribe
      JoinS_(relayMqttTopicBase, "\x1#", temp_pString)
      if(mqtt_client.subscribe((temp_pString + 1), 1) ) {
        Serial.println( F("Subscribed to topic: ") ); 
        pPrintln(temp_pString);
      }
      else {Serial.println( F("failed to subscribed") ); }
      
    } else {
      Serial.print( F("failed, rc=") );
      Serial.print(mqtt_client.state());
      Serial.println( F(" try again in 5 seconds") );
      // Wait 5 seconds before retrying
//      delay(5000);
      failDelay = 45;
      return false;
    }
  }
  return true;
}

byte GetTopicRelay(char topic[]){// topic is C string of char.
#ifdef _mqtt_debug
  Serial.print( F("GetTopicRelay: Topic: ") ); 
  Serial.print(topic);
#endif  
  byte l = StrLenZ(topic);
  byte i;
  if(l > relayMqttTopicBase[0]){
    if( l > 255 - tempStrMaxLen) l = 255 - tempStrMaxLen;
    if( l > (relayMqttTopicBase[0] + tempStrMaxLen) ) l = relayMqttTopicBase[0] + tempStrMaxLen; 
    for(i=0; i <= l; i++){//<= to include the trailing 0.
      temp_pString[i+1] = topic[i+relayMqttTopicBase[0]];
    }
    if(l > relayMqttTopicBase[0]){ temp_pString[0] = l - relayMqttTopicBase[0]; }
    else {temp_pString[0] = 0;}
    
  } else temp_pString[0] = 0;
#ifdef _mqtt_debug
  Serial.print( F(", Without base: ") ); 
  pPrintln(temp_pString);
#endif  
  if( StrCom(relay1MqttTopic, temp_pString) ) { return 1; }
  if( StrCom(relay2MqttTopic, temp_pString) ) { return 2; }
  return 0;
}

void callback(char* topic, byte* payload, unsigned int length) {
byte i;
  Serial.print( F("Message arrived, Topic: [") );
  Serial.print(topic);
  Serial.print("] ");
  for (i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  i = GetTopicRelay(topic);
  if(length == 1){
    Serial.println( F("Payload length  = 1"));
    if( (payload[0] == '0') and (i > 0) ){ UpdateRelayState(i, (byte) 0); }
    else if( (payload[0] == '1') and (i > 0) ){ UpdateRelayState(i, (byte) 1); }
    else if( (payload[0] == 't') and (i > 0) ){ ToggleRelayState(i); }
  }
}
