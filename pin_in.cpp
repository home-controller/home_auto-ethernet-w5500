
#include "pin_in.h"

byte s_pinsA[no_of_switches] = { swichPins };
//byte switchState[no_of_switches];// 0 = off, 1 = on
byte switchOnVal[no_of_switches] = initHLaS;

char *switchMqttTopics[no_of_switches];
byte topicsAdded = 0;

void AddSwitchTopic(char s[]){
  if(topicsAdded < no_of_switches) {
    switchMqttTopics[topicsAdded] = s;
    topicsAdded++;
  }
}

void InitSwitches(){
  byte i;
//  s_pinsA[] = { swichPins };
//  s_pinsA = array(6, 7);
//  switchOnVal = _initHLaS;
  for(i=0;i<no_of_switches;i++){
    switchState[i] = 0;
    pinMode(s_pinsA[i], INPUT);
  }
}

void CheckSwitches(){
  byte i;
  for(i=0;i<no_of_switches;i++){
    if(digitalRead(s_pinsA[i]) != switchState[i]){
      //switch changed
    }
  }
}
