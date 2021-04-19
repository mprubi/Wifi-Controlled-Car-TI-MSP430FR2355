#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "macros.h"

extern unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned int Time_Sequence;

extern char display_line[4][11];
extern char *display[4];
//extern volatile char one_time;

void carlsons_code(void){
  switch(Time_Sequence){  //Time_Sequence is counting upwards in a timer
    //interrupt that is in timersB0.r43 (compiled code))
    // New code from "hints from project 3a
  case TIME_SEQ_250:
    //if(one_time){  //one_time makes sure each case statement is only run once
      //per switch block, then one_time is set to zero (these statements are
      //still hit another 2- 3 hundred times after first actions are completed)
      Init_LEDs();
      //lcd_BIG_mid();
      //display_changed = 1;
      //one_time = ZERO;
    //}
    Time_Sequence = ZERO;    //gets set back to zero after reaching 250
    //counts (arbitrary rate that the instructor doesn't remember); this
    //restarts the switch case loop
    //interrupt might also set this to zero
    break;

  case TIME_SEQ_200:
    //if(one_time){
      P6OUT |= GRN_LED;            // Change State of LED 5
      //one_time = ZERO;
    //}
    break;
  case TIME_SEQ_150:
    //if(one_time){
      P1OUT |= RED_LED;            // Change State of LED 4
      P6OUT &= ~GRN_LED;           // Change State of LED 5
      //one_time = ZERO;
    //}
    break;
  case TIME_SEQ_100:
    //if(one_time){
      //lcd_4line();
      P6OUT |= GRN_LED;            // Change State of LED 5
      //display_changed = 1;
      //one_time = ZERO;
    //}
    break;
  case  TIME_SEQ_50:
    //if(one_time){
      P1OUT &= ~RED_LED;           // Change State of LED 4
      P6OUT &= ~GRN_LED;           // Change State of LED 5
      //one_time = ZERO;
    //}
    break;
  default: break;
  }
}