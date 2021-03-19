// New code: from "hints from project 3a

//------------------------------------------------------------------------------
//
//  Description: This file contains function definitions for
//              movement and motor control
//
//  Mark Rubianes
//  Jan 2021
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.11.2)
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------

#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"

char shape_state = STRAIGHT; //default, straight
char motion_state = IDLE; //default, IDLE
unsigned int wait_count = ZERO;
unsigned int my_time = RESET_STATE;
unsigned int this_timer = RESET_STATE;
extern char display_line[4][11];
extern char time_changed = 0;
unsigned int delay_start = 0;
unsigned int cycle_time;
char right_motor_count;
char left_motor_count;
char segment_count;
int duration = 0;
char set_motion_state = 0;
char right_count_time;
char left_count_time;
char travel_distance;
char travel_section = 0;
int debug_left = 0;
int debug_right = 0;
int debug_cycle_wheel_count = 0;
int debug_travel_distance = 0;

char motor_test_state = 0;
char activate_fet_test = 0;
volatile char perform_statement_once;

void forward_ON(void){
  R_FORWARD_ON();
  L_FORWARD_ON();
}

void forward_OFF(void){
  R_FORWARD_OFF();
  L_FORWARD_OFF();
}

void reverse_ON(void){
  R_REVERSE_ON();
  L_REVERSE_ON();
}

void reverse_OFF(void){
  R_REVERSE_OFF();
  L_REVERSE_OFF();
}

void all_motors_OFF(void){
  forward_OFF();
  reverse_OFF();
}


/*
function: shape_state_select - cycles through four states that represent different shapes
of movement. Currently activated by SW2
*/
void shape_state_select(void){
  shape_state++;
  if(shape_state > TRIANGLE) shape_state = STRAIGHT;

  switch(shape_state){
    case STRAIGHT:
      strcpy(display_line[1], "STRAIGHT  ");
      break;
    case CIRCLE:
      strcpy(display_line[1], "CIRCLE    ");
      break;
    case FIGURE_8:
      strcpy(display_line[1], "FIGURE_8  ");
      break;
    case TRIANGLE:
      strcpy(display_line[1], "TRIANGLE  ");
      break;
  }
}

void motion_state_select(void){
  //try to condense this to just a toggle later(^=)
  motion_state++;
  if(motion_state > WAIT) motion_state = IDLE;

  //DEBUGGING
  //shape_state = TRIANGLE;

  switch(motion_state){
    case IDLE:
      strcpy(display_line[0], "IDLE      ");
      break;
    case WAIT:
      strcpy(display_line[0], "ARM       ");
      break;
    case 2:
      strcpy(display_line[0], "2222222222");
      break;
    case 3:
      strcpy(display_line[0], "3333333333");
      break;
  }
}

void my_sleep(int duration, char set_motion_state){
  if(time_changed){
    time_changed = 0;
    if(delay_start++ >= duration){ //this is what forces the extra cycles
      delay_start = 0;
      if(set_motion_state) {
        motion_state = START;
      }
  }
 }
}

void start_case(void){
  switch(shape_state){ //need different initializations for different shapes
  case STRAIGHT:
    cycle_time = 0;
    right_motor_count = 0;
    left_motor_count = 0;
    forward_ON();
    segment_count = 0;
    travel_section = 0;
    motion_state = RUN;

    right_count_time = 25;
    left_count_time = 25;
    travel_distance = 50;
    break;
  case CIRCLE:
    cycle_time = 0;
    right_motor_count = 0;
    left_motor_count = 0;
    forward_ON();
    segment_count = 0;
    travel_section = 0;
    motion_state = RUN;

    right_count_time = CIRCLE_R_LEFT;
    left_count_time = CIRCLE_L_LEFT;
    travel_distance = TWO_CIRCLE_TRAVEL_DIST;
    break;
  case FIGURE_8:
    cycle_time = 0;
    right_motor_count = 0;
    left_motor_count = 0;
    forward_ON();
    segment_count = 0;
    travel_section = 0;
    motion_state = RUN;

    //left one full circle
    right_count_time = CIRCLE_R_LEFT;
    left_count_time = CIRCLE_L_LEFT;
    travel_distance = ONE_CIRCLE_TRAVEL_DIST;
    break;
  case TRIANGLE:
    cycle_time = 0;
    right_motor_count = 0;
    left_motor_count = 0;
    forward_ON();
    segment_count = 0;
    travel_section = 0;
    motion_state = RUN;

    right_count_time = T_STRAIGHT;
    left_count_time = T_STRAIGHT;
    travel_distance = T_STRAIGHT_TRAVEL_DIST;
    break;
  }
}


void initiate_motion(){

  if(time_changed){  // on every time_changed, do/consdier all this (else statement included)
    time_changed = 0;
    if(segment_count <= travel_distance){  //travel distance is how many periods until stopping
      debug_travel_distance++;
      if(right_motor_count++ >= right_count_time){ //if right motor > designated count time...
        P6OUT &= ~R_FORWARD;  // ...slow right motor down by turning it off for once cycle

        debug_right++;
      }
      if(left_motor_count++ >= left_count_time){  //these statements slow the motors down for turns as defined
        P6OUT &= ~L_FORWARD;

        debug_left++;
      }
      if(cycle_time >= WHEEL_PERIOD){ // if cycle time exceeds the period
        cycle_time = 0;
        right_motor_count = 0;
        left_motor_count = 0;
        segment_count++;
        forward_ON();

        debug_cycle_wheel_count++;
      }
    }else{ //this is the end of travel distance
        motion_state = END;
         // time_changed
    }
  }  // function
}
void end_case(void){ // reset everything
  switch(shape_state){
  case STRAIGHT:
    forward_OFF();
    motion_state = IDLE;
    strcpy(display_line[0], "IDLE      ");
    shape_state = STRAIGHT;
    strcpy(display_line[1], "STRAIGHT  ");

    travel_section = 0;
    break;
  case CIRCLE:
    forward_OFF();
    motion_state = IDLE;
    strcpy(display_line[0], "IDLE      ");
    shape_state = STRAIGHT;
    strcpy(display_line[1], "STRAIGHT  ");

    travel_section = 0;
    break;
  case FIGURE_8:
    // need an earlier setup dependend on FIGURE_8 for the first travel distance
    switch (travel_section){
    case 0:  //full right circle
      cycle_time = 0;
      right_motor_count = 0;
      left_motor_count = 0;
      segment_count = 0;
      forward_ON();

      right_count_time = CIRCLE_R_RIGHT;
      left_count_time = CIRCLE_L_RIGHT;
      travel_distance = ONE_CIRCLE_TRAVEL_DIST;
      travel_section++;
      motion_state = RUN;
      break;

      //38 = 2 circles
      // 19 = one circle
      // 9.5 = one half circle, or 9
      // 4.75 = one quarter circle, or 5

    case 1:  //full right circle
      cycle_time = 0;
      right_motor_count = 0;
      left_motor_count = 0;
      segment_count = 0;
      forward_ON();

      right_count_time = CIRCLE_R_LEFT;
      left_count_time = CIRCLE_L_LEFT;
      travel_distance = ONE_CIRCLE_TRAVEL_DIST;
      travel_section++;
      motion_state = RUN;
      break;

    case 2:  //full right circle
      cycle_time = 0;
      right_motor_count = 0;
      left_motor_count = 0;
      segment_count = 0;
      forward_ON();

      right_count_time = CIRCLE_R_RIGHT;
      left_count_time = CIRCLE_L_RIGHT;
      travel_distance = ONE_CIRCLE_TRAVEL_DIST;
      travel_section++;
      motion_state = RUN;
      break;

    case 3:
      forward_OFF();
      motion_state = IDLE;
      strcpy(display_line[0], "IDLE      ");
      shape_state = STRAIGHT;
      strcpy(display_line[1], "STRAIGHT  ");

      travel_section = 0;


      //        cycle_time = 0;
      //        right_motor_count = 0;
      //        left_motor_count = 0;
      //        segment_count = 0;
      //        forward_ON();
      //
      //        right_count_time = F8_R_HALF_L_TURN;
      //        left_count_time = F8_L_HALF_L_TURN;
      //        travel_distance = F8_HALF_L_TURN_TRAVEL_DIST_2;
      //        travel_section++;
      //        motion_state = RUN;
      //        break;

    }
    break;
  case TRIANGLE:
    switch (travel_section){
    case 0:  //right corner 1
      cycle_time = 0;
      right_motor_count = 0;
      left_motor_count = 0;
      segment_count = 0;
      forward_ON();

      right_count_time = T_R_RIGHT_TURN;
      left_count_time = T_L_RIGHT_TURN;
      travel_distance = T_TURN_TRAVEL_DIST;
      travel_section++;
      motion_state = RUN;
      break;
    case 1:  //full line
      cycle_time = 0;
      right_motor_count = 0;
      left_motor_count = 0;
      segment_count = 0;
      forward_ON();

      right_count_time = T_STRAIGHT;
      left_count_time = T_STRAIGHT;
      travel_distance = T_STRAIGHT_TRAVEL_DIST;
      travel_section++;
      motion_state = RUN;
      break;
    case 2:  //right corner 2
      cycle_time = 0;
      right_motor_count = 0;
      left_motor_count = 0;
      segment_count = 0;
      forward_ON();

      right_count_time = T_R_RIGHT_TURN;
      left_count_time = T_L_RIGHT_TURN;
      travel_distance = T_TURN_TRAVEL_DIST;
      travel_section++;
      motion_state = RUN;
      break;
    case 3:  //full line
      cycle_time = 0;
      right_motor_count = 0;
      left_motor_count = 0;
      segment_count = 0;
      forward_ON();

      right_count_time = T_STRAIGHT;
      left_count_time = T_STRAIGHT;
      travel_distance = T_STRAIGHT_TRAVEL_DIST;
      travel_section++;
      motion_state = RUN;
      break;
    case 4:  //right corner 3
      cycle_time = 0;
      right_motor_count = 0;
      left_motor_count = 0;
      segment_count = 0;
      forward_ON();

      right_count_time = T_R_RIGHT_TURN;
      left_count_time = T_L_RIGHT_TURN;
      travel_distance = T_TURN_TRAVEL_DIST;
      travel_section++;
      motion_state = RUN;
      break;
    case 5:  //full line (end)
      cycle_time = 0;
      right_motor_count = 0;
      left_motor_count = 0;
      segment_count = 0;
      forward_ON();

      right_count_time = T_STRAIGHT;
      left_count_time = T_STRAIGHT;
      travel_distance = T_STRAIGHT_TRAVEL_DIST;
      travel_section++;
      motion_state = RUN;
      break;
    case 6:  //right corner 4
      cycle_time = 0;
      right_motor_count = 0;
      left_motor_count = 0;
      segment_count = 0;
      forward_ON();

      right_count_time = T_R_RIGHT_TURN;
      left_count_time = T_L_RIGHT_TURN;
      travel_distance = T_TURN_TRAVEL_DIST;
      travel_section++;
      motion_state = RUN;
      break;
    case 7:  //full line (end)
      cycle_time = 0;
      right_motor_count = 0;
      left_motor_count = 0;
      segment_count = 0;
      forward_ON();

      right_count_time = T_STRAIGHT;
      left_count_time = T_STRAIGHT;
      travel_distance = T_STRAIGHT_TRAVEL_DIST;
      travel_section++;
      motion_state = RUN;
      break;
    case 8:  //right corner 5
      cycle_time = 0;
      right_motor_count = 0;
      left_motor_count = 0;
      segment_count = 0;
      forward_ON();

      right_count_time = T_R_RIGHT_TURN;
      left_count_time = T_L_RIGHT_TURN;
      travel_distance = T_TURN_TRAVEL_DIST;
      travel_section++;
      motion_state = RUN;
      break;
    case 9:  // last full line (end)
      cycle_time = 0;
      right_motor_count = 0;
      left_motor_count = 0;
      segment_count = 0;
      forward_ON();

      right_count_time = T_STRAIGHT;
      left_count_time = T_STRAIGHT;
      travel_distance = T_STRAIGHT_TRAVEL_DIST;
      travel_section++;
      motion_state = RUN;
      break;

    case 10:
      // end case
      forward_OFF();
      motion_state = IDLE;
      strcpy(display_line[0], "IDLE      ");
      travel_section = 0;
      shape_state = STRAIGHT;
      strcpy(display_line[1], "STRAIGHT  ");
      break;
    }
    break;
  }
}

/*
function: start_motion - commences predefined motion patterns for each
individual shape. Motors stop once shape pattern is complete
*/
void motion_process(void){
  //start motion sequence
  switch (project_5_Seconds){
  case 2:
    forward_ON();
    break;
  case 3:
    all_motors_OFF();
    break;
  case 4:
    reverse_ON();
    break;
  case 6:
    all_motors_OFF();
    break;
  case 7:
    forward_ON();
    break;
  case 8:
    all_motors_OFF();
    break;
  case 9: //spin
    R_REVERSE_ON();
    L_FORWARD_ON();
    break;
  case 12:
    all_motors_OFF();
    break;
  case 14:
    L_REVERSE_ON();
    R_FORWARD_ON();
    break;
  case 17:
    all_motors_OFF();
    break;
  case 19:
    GREEN_LED_ON();
    break;
  }

























//  switch(motion_state){
//  case IDLE: // state 0
//    strcpy(display_line[2], "   IDLE   ");
//    //blink_LED();
//    break;
//  case WAIT: //  state 1  (delay to allow for button press and removing fingers before car runs)
//    strcpy(display_line[2], "   WAIT   ");
//    my_sleep(duration = WAITING_TO_START, set_motion_state = 1);
//    break;
//  case START:   // state 2
//    strcpy(display_line[2], "   START  ");
//    start_case();
//    break;
//  case RUN:
//    strcpy(display_line[2], "   RUN    ");
//    initiate_motion();
//    break;
//  case END:  // state 3
//    strcpy(display_line[2], "   END    ");
//    end_case();
//    break;
//  }
}

void blink_LED(void){
  switch(my_time++){
      case 25:
        GREEN_LED_TOGGLE();
        RED_LED_TOGGLE();
        break;
      case 5000:
        GREEN_LED_TOGGLE();
        RED_LED_TOGGLE();
        break;
      case 10000:
        GREEN_LED_TOGGLE();
        RED_LED_TOGGLE();
        break;
      case 15000:
        GREEN_LED_TOGGLE();
        RED_LED_TOGGLE();
        break;
    }
}

void fet_test_state_select(void){

  motor_test_state++;
  if(motor_test_state > RGHT_REV_ON) motor_test_state = ALL_OFF;

  switch(motor_test_state){
  case ALL_OFF:
    strcpy(display_line[0], "ALL OFF   ");
    perform_statement_once = 1;
    break;
  case LFT_FRD_ON:
    strcpy(display_line[0], "LT_FRD_ON ");
    perform_statement_once = 1;
    break;
  case RGHT_FRD_ON:
    strcpy(display_line[0], "RT_FRD_ON ");
    perform_statement_once = 1;
    break;
  case LFT_REV_ON:
    strcpy(display_line[0], "LT_REV_ON ");
    perform_statement_once = 1;
    break;
  case RGHT_REV_ON:
    strcpy(display_line[0], "RT_REV_ON ");
    perform_statement_once = 1;
    break;
  }
  all_motors_OFF();
}

void fet_test_activate(void){
  activate_fet_test++;
  if(activate_fet_test > 1) activate_fet_test = 0;

}

void fet_test(void){
//1) Write functions to turn all movement off, and then individual functions to
//turn on specific forward movement and specific reverse movement. Before
//issuing a movement command make a call to the movement off function.

  if(activate_fet_test == 1){
    switch(motor_test_state){
    case ALL_OFF:
      all_motors_OFF();
      break;
    case LFT_FRD_ON:
      //4) With Battery on, Check Test points TP5 and TP7. With L_FORWARD off, TP5 and TP7
      //should equal battery voltage. With L_FORWARD on, TP5 should be half of the
      //battery voltage or less and TP7 should be about at ground potential.

      if(perform_statement_once){
        all_motors_OFF();
        L_FORWARD_ON();
        perform_statement_once = 0;
      }
      break;
    case RGHT_FRD_ON:
      //2) With Battery on, Check Test points TP1 and TP3. With R_FORWARD off, TP1 and TP3
      //should equal battery voltage. With R_FORWARD on, TP1 should be half of the
      //battery voltage or less and TP3 should be about at ground potential.

      if(perform_statement_once){
        all_motors_OFF();
        R_FORWARD_ON();
        perform_statement_once = 0;
      }
      break;
    case LFT_REV_ON:
      //5) With Battery on, Check Test points TP6 and TP8. With L_REVERSE off, TP6 and TP8
      //should equal battery voltage. With L_REVERSE on, TP6 should be half of the
      //battery voltage or less and TP8 should be about at ground potential.
      if(perform_statement_once){
        all_motors_OFF();
        L_REVERSE_ON();
        perform_statement_once = 0;
      }
      break;
    case RGHT_REV_ON:
      //3) With Battery on, Check Test points TP2 and TP4. With R_REVERSE off, TP2 and TP4
      //should equal battery voltage. With R_REVERSE on, TP2 should be half of the
      //battery voltage or less and TP4 should be about at ground potential.

      if(perform_statement_once){
        all_motors_OFF();
        R_REVERSE_ON();
        perform_statement_once = 0;
      }
      break;
    default:
      all_motors_OFF();
    }
  }else{
    all_motors_OFF();
  }
}


