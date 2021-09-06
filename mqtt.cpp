#include "mqtt.h"
#include "pString.h"
#include "relays.h"
#include "defs.h"



// Make sure to leave out the http and slashes!
// const char* mqtt_server = "test.mosquitto.org";

const char* mqtt_serverIp = "192.168.11.170";

//const char* rootTopic = "xh1/outside/light/s/e";//h1 for house 1, s = south & e = east

#define _str_join(B, C)

char id_as_str[6];

char relayMqttTopicBase[unit_prefix_len + 2]; // con

const char* relay1MqttTopic = "\13outside/s/e";//h1 for house 1, s = south & e = east
const char relay2MqttTopic[] = {11,'o','u','t','s','i','d','e','/','e','/', 's'} ;// "\x03e" = 1 char = to 0x3E apparently ;(
const char relay3MqttTopic[] = {11,'o','u','t','s','i','d','e','/','s','/', 'w'} ;// "\x03e" = 1 char = to 0x3E apparently ;(

const char relay4MqttTopic[] = {7,'f','2','/','h','a','l','l'} ;// "f2/hall"
const char relay5MqttTopic[] = {9,'f','2','/','o','f','f','i','c','e'} ;
const char relay6MqttTopic[] = {9,'f','2','/','j','u','n','k','/','e'} ;
const char relay7MqttTopic[] = {10,'f','2','/','j','u','n','k','/','e','2'} ;
const char relay8MqttTopic[] = {9,'f','2','/','j','u','n','k','/','w'} ;

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
  pPrint(temp_pString);
  if( !mqtt_client.connected() ){
    MqttConnect();
  }
  if( mqtt_client.publish(temp_pString+1, bufz) ){
    Serial.println( F(", Successful publish") );
  } else {
    if(mqtt_client.connected()){
      Serial.println( F(", Unsuccessful publish but conected"));
    } else {
      Serial.println( F(", Can't publish, unconected") );
    }
  }
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
//  initVars(); NumToStr(unit_id, id_as_str, 5);// n is the number, s is the array to retur the string in, l is the array size - 1.

  mqtt_client.setServer(mqtt_serverIp , 1883);
  mqtt_client.setCallback(callback);
  //mqtt_connecton_check();
}

boolean MqttConnect(){
  boolean conected_r;
  if (mqtt_client.connected() ){
    Serial.println( F("MQTT alread connected"));
    conected_r = true;
  } else {
    Serial.print( F("Mqtt state: ") ); Serial.println(mqtt_client.state () );
    Serial.print( F("Attempting MQTT connection..."));
    conected_r = mqtt_client.connect(id_as_str);
    // Serial.print( F("Debug, mqtt_client.connect(\""));
    // Serial.print(id_as_str+1);
    // Serial.println( F( "\") returned: ") );
    if (conected_r) {
      Serial.println( F(" Connected"));
      // Once connected, publish an announcement...
      mqtt_client.publish(relayMqttTopicBase+1, "connected");
      // ... and resubscribe
      JoinS_(relayMqttTopicBase, "\x1#", temp_pString)// "\x1" is an array of length 3 with {1, '#', 0}
      //pPrint2ln(temp_pString, temp_pString[0]+2);
      Serial.print( F("Subscribed to topic: '") );
      Serial.println( (temp_pString + 1) );
      if(mqtt_client.subscribe((temp_pString + 1), 1) ) {
      }
      else {// subscribe failed
        Serial.println( F("': Failed") ); 
      }
    } else {// connect failed
      Serial.print( F("Failed, "));
      Serial.print( F("Mqtt state: ") ); Serial.println(mqtt_client.state () );
      if(mqtt_client.state() == -2){Serial.print( F(", The network connection failed (unplugged?)") );}
      Serial.println();
    }
  }
  Serial.print( F("connected still? ") );  Serial.println(mqtt_client.connected() );
}

boolean mqtt_connecton_check() {
  static byte failDelay = 0;
  boolean conected_r;
  static boolean first_call = true;
 // unsigned long lastcalled;
  if(failDelay > 0){
    delay(100);// tenth of a second
    failDelay--;
    return false;
  } else {
    //lastcalled = millis();
    if(!first_call ) failDelay = 190;//delay for 19 seconds as other stuf each loop may take smoe time.
    first_call = false;
  }
  // Loop until we're reconnected
  conected_r = mqtt_client.connected();
  if (!conected_r) {
    conected_r = MqttConnect();
    if (conected_r) {
      // all good
    } else {
      Serial.println( F(" try again in 20 seconds") );
      return false;
    }
  }
    Serial.print( F("+++++++ Debug2, mqtt_client.connected() = ") );
    Serial.println(mqtt_client.connected());
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
  _check_relay(1);
  _check_relay(2);
  //if( StrCom(relay1MqttTopic, temp_pString) ) { return 1; }
  //if( StrCom(relay2MqttTopic, temp_pString) ) { return 2; }
#if  no_of_relays >= 3  
  _check_relay(3);
//  if( StrCom(relay3MqttTopic, temp_pString) ) { return 3; }
#if  no_of_relays >= 4  
  _check_relay(4);
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
  _check_relay(9);
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
