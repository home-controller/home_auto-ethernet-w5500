/*
  Web Server / MQTT client to control relays.

 A simple web server + MQTT that conntrols relays with the web page or with MQTT.
 using an Arduino Wiznet Ethernet shield.

 The web server is there so you can still ture the lights on and off from any web
 browser if the MQTT server is down for any reson.

 Circuit:
 * Ethernet shield attached with SPI to pins 10, 11, 12, 13 + 9 for reset
 * Output for relays on pins 3, 4 See relays.h tab
Created to start with by copying from The examples of the libraries used.
Joseph
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "defs.h"
#include "pStr.h"
#include "html.h"
#include "O.h"
#include "mqtt.h"
#include <avr/wdt.h>
#include "s.h"
#include "I.h"

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x00//0xED
};
IPAddress ip(192, 168, 1, 180);
IPAddress dns(192, 168, 11, 1);
IPAddress gateway(192, 168, 11, 1);
IPAddress subnet(255, 255, 240, 0);


// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);


//#define output1 3



void setup() {
  //byte id, i;
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println();
  Serial.println(F("Serial connected") );
  Serial.print(F("Board type: ") );
  Serial.print( eth_type );
  if (arduino_type == Uno_board) {Serial.print(" Uno "); }
  else if (arduino_type == Nano_board) {Serial.print(" Nano, "); }
  if (eth_chip == _W5100){ Serial.println("W5100"); }
  else if (eth_chip == _W5500){ Serial.println("W5500"); }
  else {Serial.println();}
  
  
  IP_offsetSetup();
  initMqttVars();// Setup mqtt base unit name id and converts pascal type string. the 01 part of: char relayMqttTopicBase[] = "h1/c01/";
  wdt_enable(WDTO_8S);
  // MQTT setup.
  //Serial.println(F("Call MQTT_setup") );
  MQTT_setup ();//setup ready for conecting.
  //Serial.println(F("returned MQTT_setup") );
  wdt_reset();
  ConnectEthernet();
  //Serial.println(F("Call printRelaysInfo();") );
  wdt_reset();
  expand_io.init();
  printRelaysInfo();
  Serial.println( F("++++++++++++++Call SetUpRelays();") );
  wdt_reset();
 // wdt_disable();
  SetUpRelays();
  Serial.println( F("No internal pullup mode for A6 & A7"));
  Serial.print( F("A0 = "));Serial.print(A0); Serial.print( F(", A7 = "));Serial.println(A7);// Serial.print( F("D1 = "));Serial.print(D1);
  SetUpInputs();// Wall switches
  Serial.print( F("main.cpp: End of Setup(). Line No.: "));Serial.println(__LINE__);
}

void loop() {
  wdt_reset();
  if (EthernetConected){
  //Check for MQTT messages.
    if (!mqtt_client.connected()) {
      if(reconnect() ){
      }
    }
    if (mqtt_client.connected()) { mqtt_client.loop(); }
  
    // listen for incoming clients
    EthernetClient client = server.available();
    if (client) {
      wdt_reset();
      Serial.println( F("new client") );
      found_GET = false;
      lineS[0] = 0;
      // an http request ends with a blank line
      boolean currentLineIsBlank = true;
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          LookForGet(lineS, c);//add c to lineS when c is new line char check if GET is in this line, if so stop looking else set string to "" and start again.
  
          Serial.write(c);
          // if you've gotten to the end of the line (received a newline
          // character) and the line is blank, the http request has ended,
          // so you can send a reply
          if (c == '\n' && currentLineIsBlank) {
            // send a standard http response header
            html_head(client);
            if(found_GET){
              parseString();
            }
  
            html_buttons(client);
            // out put relay state to html
            html_foot(client);
            break;
          }
          if (c == '\n') {
            // you're starting a new line
            currentLineIsBlank = true;
          } else if (c != '\r') {
            // you've gotten a character on the current line
            currentLineIsBlank = false;
          }
        }
      }
      wdt_reset();
      // give the web browser time to receive the data
      delay(10);
      // close the connection:
      client.stop();
      Serial.println(F("client disconnected") );
      Serial.print( F("found_GET = "));
      Serial.println(found_GET);
    }
  }  
  //else testEthernet(); todo: maybe recheck if ethernet boad conected now and then or could just reboot.
    SwitchesExe(); //Func is bebounced
}
