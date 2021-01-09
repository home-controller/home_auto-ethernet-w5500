#ifndef _html_h
#define _html_h

#include <Arduino.h>
#include <Ethernet.h>

#define t_str_len 50
#define key_str_len 5
#define v_str_len 5
#define light_off 0
#define light_on 1

extern byte light_S_E, light_E_S;// light_S_E = light_southWall_EastSide

extern char lineS[t_str_len+2];// +2 so can add 0 to end of string
extern char keyS[key_str_len+1];
extern char vS[v_str_len+1];

const char _getH[] = {4,'G','E','T', ' '};

extern char key_l1[3];
extern char key_l2[3];
extern boolean found_GET;

//char const key_on[3] = "\2on";
//char const key_on[3] = "\2on";
//char const key_on[3] = "\2on";

const char value_on[3] = {2, 'o', 'n'};
extern char value_off[4];
//char const key_on[3] = "\2on";
//char const key_on[3] = "\2on";
//char const key_on[3] = "\2on";


void html_head(EthernetClient c);
void html_foot(EthernetClient c);
void html_buttons(EthernetClient c);

void LookForGet(char s[], char c);
void parseString();// works with global vars.

;
#endif
