#include "hn.h";
/*
 * slow Home Network.
 * 1: The line is pullup so to send data first pull it low.
 * 2: If when sending stuff the line is pulled down when you have released it that means someone else was sening something to, so colision.
 * 3: When colision the one trying to send the high pulse gives up and waits for the line to be free.
 * This only works if we go slow enough that  when we pull LOW all points on the line are LOW at the same time, for the magority of the pulse length.
 * If you go to fast someone down the line can have sent the pulse and moved on to the next bit before you even see it.
 * Also there is bounce from the line end so it can look like someone else sent a pulse when it is your pulse coming back.
 * And other problems with speed so the idea is to go some slow that all that is no problem.
 * 
 * Althouth I am no expert and just read a wiki page.
 * 
 * This will work like a very cutdown CAN bus with no need for the extra modules. Hopfully. :)
 * verry losely
 */
SlowHomeNet::SlowHomeNet(){
  pinMode( _hn_int_pin,  INPUT_PULLUP );// should add some external resistor to make stronger pullup. be safer to use 2 pins and pull down through a transistor, easy to replase if you shore the line high.
                                        // Would one of the soid state fuses be fast enough to save the pin?
}

void SlowHomeNet::IntCallback(){// expects 11 bit: 8 data 1 ack, 1 parity & 1 low bit at start.
  unsigned long t;
  word mod_t;
  byte state = _pinReg & _pinMask; //LOW = 0 but HIGH value will = the mask not 1.
  word dTemp = 0;
  // if last state change time > 9 bits + 1/3 bit margin and was low then reset and wait for new start.
  // 2: if was high then start counting.
  
  CurrentTime = micros ();// not sure if should try and use the registers strait?
  t = CurrentTime - lastTime;
  // If time since last called less then 1/2 pulse time egnore call
  if(t < (bitPulseLength >> 1) ){ return; }
  t = CurrentTime - lastTime;
  lastTime = CurrentTime;
  mod_t = t & 0x7ff; //= 11 bit mask (0x7ff = 2048 - 1 = 2^11 - 1 = 0b11111111111)
  t = t>>11;
  if(mod_t > (bitPulseLength >> 1) ) t++;// t now = the number of bits sent with the last line pulse length.

  if(t + bitPos > 11){
    if(overflowCount< 0xFF) overflowCount++;
    bitPos = 0;
  } else {
    if(bitPos == 0){
      dataIn = 0;
      dFlags = 0;
    }
    bitPos += t;
    dataIn = dataIn << t;
    if(lastState == 0){// if line high add the t high bits (1s).  // if state is low last state should be high.
      //parity is only about odd or even number of high bits so only changes on high pulses.
      dFlags = (t + (dFlags & B1)) & B1;// parity = last bit of parity + t i.e. = (parity + t) bitand b00000001. hence count of all hight bits inclouding the parity will alwasy be even i.e. last bit = 0
      dataIn |= (1 << t) - 1;// e.g. t=3 becomes (1 << 3) sub 1 = B1000 sub 1 = B111. Also |= should be equivilent to += here.
    } // else leave as already set to 0s with the shift left.
    //if(bitPos > 11){should never get here as would mean the interupt was delaied by a pulse length. (I hope :P)}
    if(bitPos >= 11){// Should never be greater than 11.
      if( (dFlags & B1) == 0){
        
        dataIn = dataIn >> 2;
        buff[(headP + lengthB) & bufMsk] = (byte) dataIn;
        lengthB++;
      } else {// parity error
        if(parityErrorCount < 0xFF) parityErrorCount++;
        bitPos = 0;
      }
    }
    
  }
  lastState = state;
}
