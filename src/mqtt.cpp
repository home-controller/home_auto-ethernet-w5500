#include "mqtt.h"
#include "pStr.h"
#include "O.h"
#include "defs.h"

extern const char room_names[] PROGMEM;
extern byte room_note_i[12];
extern byte room_names_i[8];
extern byte switchInfo[][3];
extern room_T rooms_A[];


// Make sure to leave out the http and slashes!
// const char* mqtt_server = "test.mosquitto.org";

const char* mqtt_serverIp = "192.168.11.170";
char mqtt_clientID[7]; //idxxx
//const char* rootTopic = "xh1/outside/light/s/e";//h1 for house 1, s = south & e = east

#define _str_join(B, C)

//char relayMqttTopicBase[] = "h1/c01/";//converted to pas string in s.cpp
char relayMqttTopicBase[] = "h2/c01/";//converted to pas string in s.cpp

const char* relay1MqttTopic = "\13outside/s/e";//h1 for house 1, s = south & e = east i.e. over rachels room
const char relay2MqttTopic[] = {11,'o','u','t','s','i','d','e','/','e','/', 's'} ;// "\x03e" = 1 char = to 0x3E apparently ;( over my room
const char relay3MqttTopic[] = {11,'o','u','t','s','i','d','e','/','s','/', 'w'} ;// "\x03e" = 1 char = to 0x3E apparently ;( tool room

pStr relay4MqttTopic = pStr_("f2/hall") ;// "f2/hall"
const char relay5MqttTopic[] = {9,'f','2','/','o','f','f','i','c','e'} ;
const char relay6MqttTopic[] = {9,'f','2','/','j','u','n','k','/','e'} ;
const char relay7MqttTopic[] = {10,'f','2','/','j','u','n','k','/','e','2'} ;
const char relay8MqttTopic[] = {9,'f','2','/','j','u','n','k','/','w'} ;
// // {9,'f','2','/','j','u','n','k','/','w'} ;.
//relay9MqttTopic[0]= (char) sizeof(relay9MqttTopic)-1;//don't count trailing 0
pStr relay9MqttTopic;// = {9,"dfgfg"};//pStr_("f2/stairs");

#define tempStrMaxLen 30
char temp_pString[tempStrMaxLen + 1];
//void initVars(){
//  relay1MqttTopic[0] = sizeof(relay1MqttTopic) - 2;//-1 for 0 turm and 1 for length at [0]
//  relay2MqttTopic[0] = sizeof(relay2MqttTopic) - 2;//-1 for 0 turm and 1 for length at [0]
//}


//IPAddress MQTTserverIp(192, 168, 11, 170);


  EthernetClient ethClient;
  PubSubClient mqtt_client(ethClient);
/**
 * @brief Publish s to mqtt server.
 * 
 * @param s in 
 * @param bufz 
 */
void Publish(const char* s, char* bufz){
  const char p[8] = "\x6state/";
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
  if (mqtt_client.connected() ) mqtt_client.publish(temp_pString+1, bufz);
}

;;;;


void MqttPushSwitchState(byte switchN, byte state){
  Serial.print( F("MqttPushSwitchState(switch = "));
  Serial.print(switchN);
  Serial.print( F(", state = "));
  Serial.println(switchN);
  char bufz[] = "s=0";
  char topic[30];
  byte room = (switchInfo[switchN][1] bitand 0b1111);
  byte floor1 = (rooms_A[room].houseFloor bitand 0b111);
  bufz[2] = 48 + state;
  if(switchN == 0){
    Serial.println(F("Error: The first switch is 1") );
  }
  //byte f = floor1;
  topic[0]= 1;
  topic[1] = 'f';
  addByteToString(floor1,topic, sizeof(topic) - 1);//has no array len checks
  byte start;
  if(switchN==1){ start = 0; }
  else {start = room_names_i[switchN - 1]; }
  byte l = room_names_i[switchN] - start;
  if(topic[0] >= 30) return;
  if(topic[0] + l >= 30) {l = (30-2) - topic[0];}// need 1 byte for length an 1 for zero terminator
  memcpy_P(topic+1+topic[0], room_names + start, l);
  topic[0] = l;

  Publish(topic, bufz);
}

void MqttPushRelayState(byte r){
  Serial.print( F("MqttPushRelayState called with r = "));
  Serial.println(r);
  char bufz[] = "s=0";
  bufz[2] = 48 + relayState[r-1];
  if(r == 0){
    Serial.println( F("Error: The first relay is 1") );
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
    Publish(relay4MqttTopic.p, bufz);    
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
    Publish(relay9MqttTopic.p, bufz);    
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
    delay(100);// tenth of a second
    failDelay--;
    return false;
  }
  // Loop until we're reconnected
  if (!mqtt_client.connected()) {
    Serial.print( F("Attempting MQTT connection..."));
    // Attempt to connect
    if (mqtt_client.connect(mqtt_clientID+1)) {
      Serial.println( F("connected"));
      // Once connected, publish an announcement...
      CopyFirst_(relayMqttTopicBase, relayMqttTopicBase[0]-1, temp_pString);
      
      mqtt_client.publish(temp_pString+1,"connected");
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
      Serial.println( F(" try again in 20 seconds") );
      // Wait 5 seconds before retrying
//      delay(5000);
      failDelay = 190;//delay for 19 seconds as other stuf each loop may take smoe time.
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

  #define _check_relay(I)  { if( StrCom(relay ## I ## MqttTopic, temp_pString) ) { return I; } } 
  #define _check_relayP(I)  { if( StrCom(relay ## I ## MqttTopic.p, temp_pString) ) { return I; } } 
  _check_relay(1);
  _check_relay(2);
  //if( StrCom(relay1MqttTopic, temp_pString) ) { return 1; }
  //if( StrCom(relay2MqttTopic, temp_pString) ) { return 2; }
#if  no_of_relays >= 3  
  _check_relay(3);
//  if( StrCom(relay3MqttTopic, temp_pString) ) { return 3; }
#if  no_of_relays >= 4  
  _check_relayP(4);
  //if( StrCom(relay4MqttTopic, temp_pString) ) { return 4; }
#if  no_of_relays >= 5  
  _check_relay(5);
  //if( StrCom(relay5MqttTopic, temp_pString) ) { return 5; }
#if  no_of_relays >= 6  
  _check_relay(6);
  //if( StrCom(relay6MqttTopic, temp_pString) ) { return 6; }
#if  no_of_relays >= 7  
  _check_relay(7);
  //if( StrCom(relay7MqttTopic, temp_pString) ) { return 6; }
#if  no_of_relays >= 8  
  _check_relay(8);
  //if( StrCom(relay8MqttTopic, temp_pString) ) { return 2; }
#if  no_of_relays >= 9  
  _check_relayP(9);
  //if( StrCom(relay9MqttTopic, temp_pString) ) { return 2; }
#if  no_of_relays >= 10 
  _check_relay(10);
  //if( StrCom(relay10MqttTopic, temp_pString) ) { return 2; }
#endif  // relay 3
#endif  // relay 4
#endif  // relay 5
#endif  // relay 6
#endif  // relay 7
#endif  // relay 8
#endif  // relay 9
#endif  // relay 10
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
  Serial.print( F("Topic matched relay: ") );
  Serial.print(i);
  Serial.print(", ");

  if(length == 1){
    Serial.println( F("Payload length  = 1"));
    if( (payload[0] == '0') and (i > 0) ){ UpdateRelayState(i, (byte) 0); }
    else if( (payload[0] == '1') and (i > 0) ){ UpdateRelayState(i, (byte) 1); }
    else if( (payload[0] == 't') and (i > 0) ){ ToggleRelayState(i); }
  }
}
