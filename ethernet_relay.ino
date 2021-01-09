/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 02 Sept 2015
 by Arturo Guadalupi
 
 */

#include <SPI.h>
#include <EEPROM.h>
#include <Ethernet.h>
#include "pString.h"
#include "html.h"
#include "relays.h"

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x00//0xED
};
IPAddress ip(192, 168, 1, 180);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

#define NextBoardId 2
#define eepromIdAddr 250
#define W5500_RESET_PIN 9

//#define output1 3



void setup() {
  byte id, i;
  Serial.begin(9600);
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet
  id = EEPROM.read(eepromIdAddr);
  //id=0xFF; hard set of board id
  if(id == 0xFF){
    Serial.print(F("Setting") );
    EEPROM.update(eepromIdAddr, NextBoardId);
    id = NextBoardId;
  } else {
    Serial.print(F("Got") );
  }
  Serial.print(F(" EEprom board ID:") );
  Serial.println(id);
  ip[3] += id;
  mac[5] += id;
  
  // Open serial communications and wait for port to open:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Ethernet WebServer Example");

// can be a problem to leave reset floting?
    pinMode(W5500_RESET_PIN, OUTPUT);
    digitalWrite(W5500_RESET_PIN, LOW);
    delay(10);
    digitalWrite(W5500_RESET_PIN, HIGH);
    delay(2*1000);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println(F("Ethernet shield was not found.  Sorry, can't run without hardware. :(") );
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println(F("Ethernet cable is not connected.") );
  }

  // start the server
  server.begin();
  Serial.print(F("server is at ") );
  Serial.println(Ethernet.localIP());
  byte macBuffer[6];  // create a buffer to hold the MAC address
  Ethernet.MACAddress(macBuffer); // fill the buffer
  Serial.print("The MAC address is: ");
  for (byte octet = 0; octet < 6; octet++) {
    Serial.print(macBuffer[octet], HEX);
    if (octet < 5) {
      Serial.print('-');
    }
  }
  Serial.println();
  Serial.print( F("Relay pins array size = ") );
  Serial.print(sizeof(pinsA) );
  Serial.print(" Pins: ");
  for(i=0; i< sizeof(pinsA); i++){
    if(i>0) Serial.print(", ");
    Serial.print(pinsA[i]);
  }
  Serial.println();
  Serial.print("Sizeof _getH = ");
  Serial.println(sizeof(_getH ));
  Serial.print("Sizeof 'get ' = ");
  Serial.println(sizeof("GET "));
  Serial.print("_getH[0] = '");
  Serial.print(_getH[0]);
  Serial.println("'");
  byte x;
  Serial.println('0',DEC);
  pPrintln(_getH);
  Serial.println();
  for (x=0; x< sizeof(pinsA); x++){
    updateRelayState(x+1, (byte) light_off); //turn all relays off at startup
    pinMode(pinsA[x], OUTPUT);
  }
}


void loop() {
  //byte i;
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
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
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println(F("client disconnected") );
    Serial.print("found_GET = ");
      Serial.println(found_GET);
    
  }
}
