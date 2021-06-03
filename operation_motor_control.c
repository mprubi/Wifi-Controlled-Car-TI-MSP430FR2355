#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "macros.h"

unsigned int speed_global = WHEEL_OFF,
             travel_time_seconds = 0;
extern volatile char from_forward,
                     from_reverse,
                     time_seconds,
                     FET_delay_timer,
                     activate_pivot_timer,
                     activate_travel_time;

/*TODO:

forward(speed, direction) //this function will set the speed
accelerate_decelerate(direction) //this function will make incremental changes to current speed
spin(speed, direction) //will utilize both forward and reverse of oppisite wheels
reverse(speed, direction)

TODO: gradual acc_dec using timer on order of mSec which increase/decrease wheel speeds in small increments

*/

void forward(int speed, char direction, char time_seconds){
  //    Hand over these volatiles to a temporary local in order to get rid of this warning:
  //    -Warning[Pa082]: undefined behavior: the order of volatile accesses is undefined in this statement
//  unsigned int speed_L_component = 0;
//  unsigned int speed_R_component = 0;
  unsigned int L_wheel_speed = 0;
  unsigned int R_wheel_speed = 0;
  unsigned int Left_Reverse_Speed = LEFT_REVERSE_SPEED;
  unsigned int Right_Reverse_Speed = RIGHT_REVERSE_SPEED;
  speed_global = speed;

//  stop_car();
  
  switch(speed){
  case 'F':
    speed = FAST;
    break;      
  case 'M':
    speed = MEDIUM;
    break;
    case 'T':
    speed = TURBO;
    break;
  }

  //determine the general velocity
  switch(direction){
  case STRAIGHT:
    L_wheel_speed = speed;
    R_wheel_speed = speed;
    break;
  case SOFT_LEFT_TURN:
    //    speed_L_component = -SOFT_TURN;
    //    speed_R_component = SOFT_TURN;

    L_wheel_speed = (speed - SOFT_TURN);
    R_wheel_speed = (speed + SOFT_TURN);
    break;
  case SOFT_RIGHT_TURN:
    //    speed_L_component = SOFT_TURN;
    //    speed_R_component = -SOFT_TURN;
    L_wheel_speed = (speed + SOFT_TURN);
    R_wheel_speed = (speed - SOFT_TURN);
    break;
  case MEDIUM_LEFT_TURN:
    //    speed_L_component = -MEDIUM_TURN;
    //    speed_R_component = MEDIUM_TURN;
    L_wheel_speed = (speed - MEDIUM_TURN);
    R_wheel_speed = (speed + MEDIUM_TURN);
    break;
  case MEDIUM_RIGHT_TURN:
    //    speed_L_component = MEDIUM_TURN;
    //    speed_R_component = -MEDIUM_TURN;
    L_wheel_speed = (speed + MEDIUM_TURN);
    R_wheel_speed = (speed - MEDIUM_TURN);
    break;
  default: //straight
    L_wheel_speed = speed;
    R_wheel_speed = speed;
    break;
  }

  //check if motors are powered in reverse

  //WARNING: need to figure out other way to call same function after delay
  if(Left_Reverse_Speed || Right_Reverse_Speed){
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;

    from_forward = TRUE;

//    BACKLIGHT_TOGGLE(); // debug

    //come back a second later and set the other speeds
    FET_delay_timer += time_seconds; //turn on a one second timer, handled in TB2 CCR1

  }else if(!Left_Reverse_Speed || !Right_Reverse_Speed){

//    L_wheel_speed = (speed + speed_L_component);
//    R_wheel_speed = (speed + speed_R_component);
    if(time_seconds != 0){
      travel_time_seconds = time_seconds;
      activate_travel_time = TRUE;
    }
    LEFT_FORWARD_SPEED = L_wheel_speed;
    RIGHT_FORWARD_SPEED = R_wheel_speed;
  }
}

void pivot(char direction, int magnitude){
  unsigned int L_wheel_speed = magnitude;
  unsigned int R_wheel_speed = magnitude;
  unsigned int Left_Reverse_Speed = LEFT_REVERSE_SPEED;
  unsigned int Right_Reverse_Speed = RIGHT_REVERSE_SPEED;
  unsigned int Left_Forward_Speed = LEFT_FORWARD_SPEED;
  unsigned int Right_Forward_Speed = RIGHT_FORWARD_SPEED;

  //unsigned int speed = MEDIUM;
  //determine the general velocity
  switch(direction){
  case LEFT_PIVOT:
    if(Left_Forward_Speed || Right_Reverse_Speed){
      LEFT_FORWARD_SPEED = WHEEL_OFF;
      RIGHT_REVERSE_SPEED = WHEEL_OFF;
      from_forward = TRUE;
      FET_delay_timer += time_seconds; //turn on a one second timer, handled in TB2 CCR1
    }else if(!Left_Forward_Speed || !Right_Reverse_Speed){
      LEFT_REVERSE_SPEED = L_wheel_speed;
      RIGHT_FORWARD_SPEED = R_wheel_speed;
      activate_pivot_timer = TRUE;
    }
    break;
  case RIGHT_PIVOT:
    if(Left_Reverse_Speed || Right_Forward_Speed){
      LEFT_REVERSE_SPEED = WHEEL_OFF;
      RIGHT_FORWARD_SPEED = WHEEL_OFF;
      from_forward = TRUE;
      FET_delay_timer += time_seconds; //turn on a one second timer, handled in TB2 CCR1
    }else if(!Left_Reverse_Speed || !Right_Forward_Speed){
      LEFT_FORWARD_SPEED = L_wheel_speed;
      RIGHT_REVERSE_SPEED = R_wheel_speed;
      activate_pivot_timer = TRUE;
    }
    break;
  default:
    //    L_wheel_speed = speed;
    //    R_wheel_speed = speed;
    break;
  }
}

void reverse(int speed){
//    Hand over these volatiles to a temporary local in order to get rid of this warning:
//    -Warning[Pa082]: undefined behavior: the order of volatile accesses is undefined in this statement
  unsigned int L_wheel_speed = 0;
  unsigned int R_wheel_speed = 0;
  unsigned int Left_Forward_Speed = LEFT_FORWARD_SPEED;
  unsigned int Right_Forward_Speed = RIGHT_FORWARD_SPEED;
  speed_global = speed;

//  speed_L_component = 0;
//  speed_R_component = 0;
  L_wheel_speed = speed;
  R_wheel_speed = speed;

  if(Left_Forward_Speed || Right_Forward_Speed){
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;

    from_reverse = TRUE;

//    BACKLIGHT_TOGGLE(); // debug

    FET_delay_timer += time_seconds;

  }else if(!Left_Forward_Speed || !Right_Forward_Speed){

//    L_wheel_speed = (speed_dual_component + speed_L_component);
//    R_wheel_speed = (speed_dual_component + speed_R_component);

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

//void change_velocity(int magnitude, char acc_dec){
////TODO: implement this
//}


