#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "macros.h"

extern unsigned int speed_L_component = 0;
extern unsigned int speed_R_component = 0;
extern unsigned int speed_dual_component = 0;
extern unsigned int L_wheel_speed = 0;
extern unsigned int R_wheel_speed = 0;
extern unsigned int set_R_F, set_R_R, set_L_F, set_L_R;
unsigned int speed_global = WHEEL_OFF;
extern volatile char from_forward;
extern volatile char from_reverse;
char interface_bools;
extern volatile int time_seconds;
volatile int FET_delay_timer = 0;
//#define accelerate/decelerate   (0x0)
//#define unused   (0x1)
//#define unused   (0x2)
//#define unused   (0x3)
//#define unused   (0x4)
//#define unused   (0x5)
//#define unused   (0x6)
//#define unused   (0x7)



/*TODO:

forward(speed, direction) //this function will set the speed
accelerate_decelerate(direction) //this function will make incremental changes to current speed
spin(speed, direction) //will utilize both forward and reverse of oppisite wheels
reverse(speed, direction)

TODO: gradual acc_dec using timer on order of mSec which increase/decrease wheel speeds in small increments

*/

/*
//char assign_PWM_values_with_error_check(void){
//  char forward_and_reverse_FETS_enabled = 0;
//
//  //P6OUT &= ~R_REVERSE
//  if(RIGHT_REVERSE_SPEED == 0){
//    RIGHT_FORWARD_SPEED = R_wheel_speed;
//  }
//  if(LEFT_REVERSE_SPEED == 0){
//    LEFT_FORWARD_SPEED = L_wheel_speed;
//  }
//  if(RIGHT_FORWARD_SPEED == 0){
//    RIGHT_REVERSE_SPEED = R_wheel_speed;
//  }
//  if(LEFT_FORWARD_SPEED == 0){
//    LEFT_REVERSE_SPEED = L_wheel_speed;
//  }
//
//  if (((RIGHT_FORWARD_SPEED > 0) && (RIGHT_REVERSE_SPEED > 0)) || ((LEFT_FORWARD_SPEED > 0) && (LEFT_REVERSE_SPEED > 0))){
//    forward_and_reverse_FETS_enabled = 1;
//  }
//  return forward_and_reverse_FETS_enabled;
//}
*/

void forward(int speed){
//    Hand over these volatiles to a temporary local in order to get rid of this warning:
//    -Warning[Pa082]: undefined behavior: the order of volatile accesses is undefined in this statement
  int Left_Reverse_Speed = LEFT_REVERSE_SPEED;
  int Right_Reverse_Speed = RIGHT_REVERSE_SPEED;
  //int t_seconds = time_seconds;

  //initialize these to zero when only going straight
  speed_dual_component = 0;
  speed_L_component = 0;
  speed_R_component = 0;
  L_wheel_speed = 0;
  R_wheel_speed = 0;


  if(Left_Reverse_Speed | Right_Reverse_Speed){
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;

    from_forward = TRUE;

//    BACKLIGHT_TOGGLE(); // debug

    //come back a second later and set the other speeds
    FET_delay_timer += time_seconds; //turn on a one second timer, handled in TB2 CCR1

  }else if(!Left_Reverse_Speed | !Right_Reverse_Speed){

    speed_dual_component = speed;
    L_wheel_speed = (speed_dual_component + speed_L_component);
    R_wheel_speed = (speed_dual_component + speed_R_component);

    LEFT_FORWARD_SPEED = L_wheel_speed;
    RIGHT_FORWARD_SPEED = R_wheel_speed;
  }
}

void reverse(int speed){
//    Hand over these volatiles to a temporary local in order to get rid of this warning:
//    -Warning[Pa082]: undefined behavior: the order of volatile accesses is undefined in this statement
  int Left_Forward_Speed = LEFT_FORWARD_SPEED;
  int Right_Forward_Speed = RIGHT_FORWARD_SPEED;

  speed_dual_component = 0;
  speed_L_component = 0;
  speed_R_component = 0;
  L_wheel_speed = 0;
  R_wheel_speed = 0;

  if(Left_Forward_Speed | Right_Forward_Speed){
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;

    from_reverse = TRUE;

//    BACKLIGHT_TOGGLE(); // debug

    FET_delay_timer += time_seconds;

  }else if(!Left_Forward_Speed | !Right_Forward_Speed){

    speed_dual_component = speed;
    L_wheel_speed = (speed_dual_component + speed_L_component);
    R_wheel_speed = (speed_dual_component + speed_R_component);

    LEFT_REVERSE_SPEED = L_wheel_speed;
    RIGHT_REVERSE_SPEED = R_wheel_speed;
  }
}

void stop_car(void){
  RIGHT_FORWARD_SPEED = WHEEL_OFF;
  LEFT_FORWARD_SPEED = WHEEL_OFF;
  RIGHT_REVERSE_SPEED = WHEEL_OFF;
  LEFT_REVERSE_SPEED = WHEEL_OFF;
}

void change_velocity(int magnitude, char acc_dec){
//TODO: implement this
}

