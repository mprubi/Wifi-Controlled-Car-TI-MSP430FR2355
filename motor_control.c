//// New code: from "hints from project 3a
//
////------------------------------------------------------------------------------
////
////  Description: This file contains function definitions for
////              movement and motor control
////
////  Mark Rubianes
////  Jan 2021
////  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.11.2)
////------------------------------------------------------------------------------
//
////-----------------------------------------------------------------------------
//
//#include  "functions.h"
//#include  "msp430.h"
//#include <string.h>
//#include "macros.h"
//
//char motion_state = IDLE; //default, IDLE
//unsigned int wait_count = ZERO;
//extern char display_line[4][11];
//unsigned int delay_start = 0;
//extern unsigned int speed_L_component;
//extern unsigned int speed_R_component;
//extern unsigned int speed_dual_component;
//extern unsigned int L_wheel_speed;
//extern unsigned int R_wheel_speed;
//
//void motion_state_select(void){
//  motion_state++;
//  if(motion_state > WAIT) motion_state = IDLE;
//
//  switch(motion_state){
//    case IDLE:
//      strcpy(display_line[0], "IDLE      ");
//      break;
//    case WAIT:
//      strcpy(display_line[0], "ARM       ");
//      break;
//    case 2:
//      strcpy(display_line[0], "2222222222");
//      break;
//    case 3:
//      strcpy(display_line[0], "3333333333");
//      break;
//  }
//}
//// TODO: implement sleep(seconds) function using dedicated timer // use debouce switch method
////void my_sleep(int duration, char set_motion_state){
////  if(time_changed){
////    time_changed = 0;
////    if(delay_start++ >= duration){ //this is what forces the extra cycles
////      delay_start = 0;
////      if(set_motion_state) {
////        motion_state = START;
////      }
////  }
//// }
////}
//
//
////void initiate_motion(){
////
////  if(time_changed){  // on every time_changed, do/consdier all this (else statement included)
////    time_changed = 0;
////    if(segment_count <= travel_distance){  //travel distance is how many periods until stopping
////      debug_travel_distance++;
////      if(right_motor_count++ >= right_count_time){ //if right motor > designated count time...
////        P6OUT &= ~R_FORWARD;  // ...slow right motor down by turning it off for once cycle
////
////        debug_right++;
////      }
////      if(left_motor_count++ >= left_count_time){  //these statements slow the motors down for turns as defined
////        P6OUT &= ~L_FORWARD;
////
////        debug_left++;
////      }
////      if(cycle_time >= WHEEL_PERIOD){ // if cycle time exceeds the period
////        cycle_time = 0;
////        right_motor_count = 0;
////        left_motor_count = 0;
////        segment_count++;
////        forward_ON();
////
////        debug_cycle_wheel_count++;
////      }
////    }else{ //this is the end of travel distance
////        motion_state = END;
////         // time_changed
////    }
////  }  // function
////}
//
//
///*
//function: start_motion - commences predefined motion patterns for each
//individual shape. Motors stop once shape pattern is complete
//*/
//void motion_process(void){
//
//
////  switch(motion_state){
////  case IDLE: // state 0
////    strcpy(display_line[2], "   IDLE   ");
////    //blink_LED();
////    break;
////  case WAIT: //  state 1  (delay to allow for button press and removing fingers before car runs)
////    strcpy(display_line[2], "   WAIT   ");
////    my_sleep(duration = WAITING_TO_START, set_motion_state = 1);
////    break;
////  case START:   // state 2
////    strcpy(display_line[2], "   START  ");
////    start_case();
////    break;
////  case RUN:
////    strcpy(display_line[2], "   RUN    ");
////    initiate_motion();
////    break;
////  case END:  // state 3
////    strcpy(display_line[2], "   END    ");
////    end_case();
////    break;
////  }
//}
//
//
//
//
