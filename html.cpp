#include <Ethernet.h>
#include "html.h"
#include "pString.h"
#include "relays.h"

boolean found_GET = false;
 char key_l1[3] = {2, 'l', '1'};
 char key_l2[3] = {2,'l', '2'};

 //char value_on[3] = {2, 'o', 'n'};
 char value_off[4] = {3,'o', 'f', 'f'};
 char web_page[12] = {0, 0};//10 letters + 1 for length and +1 for 0 termination.

char lineS[t_str_len+2] = {[0]=0};// +2 so can add 0 to end of string
char keyS[key_str_len+1] = {[0]=0};
char vS[v_str_len+1] = {[0]=0};



void html_head(EthernetClient c){
  c.println(F("HTTP/1.1 200 OK") );
  c.println(F("Content-Type: text/html") );
  c.println(F("Connection: close") );  // the connection will be closed after completion of the response
  //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
  c.println();
  c.println(F("<!DOCTYPE HTML>") );
  c.println(F("<html> <head>") );
  c.println(F("<style>") );
// button style
  c.println(F(" button {border-radius: 20%; display: inline-block; padding: 15px 25px; font-size: 24px; cursor: pointer; text-align: center; text-decoration: none; ") );
  c.println(F(" outline: none; color: #fff; background-color: darkgreen; border: none; border-radius: 20px; box-shadow: -4px 9px #999; }") );
// deprested class
  c.println(F(".depressed { background-color:lime;  box-shadow: 1px 3px #666;  transform: translate(3px, 6px); }") );
  c.println(F("div {max-width: 300px; margin: 5px; padding-left: 9px; padding-bottom: 18px; border: 5px solid blue; border-style: double;}") );
  
//  c.println(F("") );
//  c.println(F("") );
//  c.println(F("") );
  c.println(F("</style>") );
//  c.println(F("") );

}

void html_foot(EthernetClient c){
  c.println(F("</html>") );
}

void html_buttons(EthernetClient c){
  byte x;
  for(x=1;x<=no_of_relays; x++){
    c.print(F("<div><h3>Light on "));
    if(x==1) { c.print(F("South wall East") ); }
    else if (x == 2) { c.print(F("East wall South") ); }
    c.println(F(" side</h3></ br>") );
    c.print(F("<a href=\'/outside?l") );
    c.print(x);
    c.print(F("=") );
    if(relayState[x-1] == light_on) { c.print(F("off") ); }
    else  { c.print(F("on") ); }
    c.println(F("'><button") );
    if(relayState[x-1] == light_on){c.println(F(" class='depressed'>Turn off") ) ;}
    else  {c.println(F(">Turn on") ) ;}
    c.println(F("</button>") );
    c.println(F("</a>") );
    c.println(F("</div></ br>") );
  }
}

void parseString(){// works with the globle lineS defined in the main ino unit
  byte key;
  const char k1[] = {2,'?', ' '};//pascal type string.  k1 := '? ';
// save the web page part of s if any in web_page  
  if(FindFirstC(lineS, '/') > 0){
    DelToC(lineS, '/');
    SplitK1_(lineS, k1, web_page);
  } else {web_page[0] = 0; }
// process any switch commands passed with get syntax
  if(FindFirstC(lineS, '?') > 0){
    DelToC(lineS, '?');
      //i = 
    while (FindFirstC(lineS, '=') > 0){
      SplitC2_(lineS, '=' , keyS);
      SplitK2_(lineS, (char*)"\2& ", vS );
// check if keyS is of l?? format where ? can are digits. can be only 1 digit
      if(keyS[0] >= 2){//could be 0 terminated string etc so maybe longer but only care about the first 3
        if(keyS[1] == 'l'){
          if( (keyS[2] >= '0') and (keyS[2] <= '9') ){
            key = keyS[2] - '0';
            if( (keyS[3] >= '0') and (keyS[3] <= '9') ){ key = key * 10 + (keyS[3] - '0'); }
            updateRelayState(key, vS);
          }
        }
      }
      Serial.print("Key found, Key = '");
      pPrint(keyS);
      Serial.print("' value = '");
      pPrint(vS);
      Serial.println("'");
    }
  }
}

void LookForGet(char s[], char c){
  // Store each line in an array var.
  if( !found_GET ){
    if (c == '\n'){//When we have 1 line of text saved in the var array check it for key words
      //s[0] = ts_i;
      if (s[0] <= t_str_len){ s[s[0]+1] = 0; }
  // search the line for the GET keyword, if found stop looking.
      Serial.print("s = '");
      pPrint(s);
      Serial.println("'");
      Serial.print("_getH = '");
      pPrintln(_getH);
      Serial.print("'");
      if (InStringS(s, _getH) > 0 ){
        found_GET = true;
      } else {// if the text line has no keyword then set the string to length 0;
        s[0] = 0;
      }
      
    }
    if (s[0] <= t_str_len){// as long as the line still fits into the array.
      s[0] ++;
      s[ (byte) s[0]] = c; //copy the last char from the server to the end of the string (array). 
    }
  }
}
