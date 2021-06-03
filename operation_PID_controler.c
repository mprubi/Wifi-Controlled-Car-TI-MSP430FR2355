#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"

volatile char observe_control_system = FALSE;
char current_state = ON_LINE,
     previous_state;
int previous_error = 0;
extern volatile unsigned int left_sensor_mov_average,
                             right_sensor_mov_average;
extern volatile char PID_control_enabled,
                    bang;
extern int black_line_edge_threshold;
extern unsigned int speed_global,
                    speed_correction;
//long debug_follow_black_line_counter = 0, 
//      debug_observe_control_system_counter = 0,
//      debug_control_signal_calc_counter = 0;
int debug_L_BLACK[DEBUG_ARR_SIZE],
    debug_R_BLACK[DEBUG_ARR_SIZE],
    debug_L_SPEED[DEBUG_ARR_SIZE],
    debug_R_SPEED[DEBUG_ARR_SIZE],
    debug_Control[DEBUG_ARR_SIZE];
int debug_scroller = 0;

// Monitoring Loop
void follow_black_line(void){
  // Check if OK to re-evaluate system
  
  //debugging var
//  debug_follow_black_line_counter++;
  
  if(PID_control_enabled){
    if(observe_control_system){
      
      //debugging var
//      debug_observe_control_system_counter++;
      
      observe_control_system  = FALSE;
      // Get State Information
      current_state = determine_current_state();
      if(current_state == OFF_LINE) { // when completely off black line, handle car without PID control
        switch(previous_state){
        case LEFT_OF_LINE:
          forward(SLOW, SOFT_RIGHT_TURN, 0);
          break;
        case RIGHT_OF_LINE:
          forward(SLOW, SOFT_LEFT_TURN, 0);
          break;
        default:
          //TODO: await manual navigation instructions
          stop_car();// If prev state is unknown because of total overshoot, stop and await manual nav
//          if((left_sensor_mov_average > black_line_edge_threshold) && (right_sensor_mov_average > black_line_edge_threshold)){
//            stop_car();
//            current_state = determine_current_state();
//            previous_state = current_state;
//          }else reverse(SLOW);
          break;
        }

        //TODO: handle when to set current state back to = ON_LINE
      }
      else process_actuator(control_signal_calculation()); // we are not just reading white so we are able to track set point
      // Set variables for next iteration
      previous_state = current_state;
      observe_control_system  = FALSE; //TODO: tie this to the sample time (30 mSec timer) and set TRUE
    }
  }
}

// PID
int control_signal_calculation(void){
  int proportial_error = 0, integral_error = 0, derivative_error = 0;
  int u_control = 0;
  int sample_time = 30; // Hz - ADC readings occur 250 times per second ?
  int set_point = 0;
  float Kp_gain = 3;
  float Ki_gain = 1;
  float Kd_gain = 1;


  // Calculate PID error
  proportial_error = set_point - (left_sensor_mov_average - right_sensor_mov_average); // Magnitude of error indicates how far off the line, sign of error indicates direction of movement
  integral_error = previous_error + (proportial_error*sample_time);
  derivative_error = (proportial_error - previous_error)/sample_time;
  previous_error = proportial_error; // save error for next loop

  // Calculate PID control signal
  u_control = Kp_gain*proportial_error + Ki_gain*integral_error + Kd_gain*derivative_error;
  
//  debug_control_signal_calc_counter++;
    
  return u_control; //complete PID signal to the actuator
}

void process_actuator(int control_value){
  // Assume vehicle is designed to move at some cruise speed, and adjust wheel PWM values to track a curved line
  // Negative valued control indicates a right turn, positive valued control indicates left turn
//  int cruise_speed = MEDIUM, MIN_speed = SLOW, MAX_speed = FAST;
  int final_control_value = 0;
  
  int cruise_speed = SLOW, MIN_speed = 10000, MAX_speed = FAST;
  unsigned int left_speed = 0, right_speed = 0;

  final_control_value = control_value;  

  right_speed = cruise_speed - final_control_value;
  left_speed = cruise_speed + final_control_value;
  // speed boundaries
  if(left_speed < MIN_speed){
    left_speed = MIN_speed;
  }
  if(right_speed < MIN_speed){
    right_speed = MIN_speed;
  }
  else if(left_speed > MAX_speed){
    left_speed = MAX_speed;
  }else if(right_speed > MAX_speed){
    right_speed = MAX_speed;
  }
  // Set speed values to PWM registers
  LEFT_FORWARD_SPEED = left_speed;
  RIGHT_FORWARD_SPEED = right_speed;
  
  if (debug_scroller++ > DEBUG_ARR_SIZE){
      debug_scroller = 0;
    }
  
  debug_L_BLACK[debug_scroller] = left_sensor_mov_average;
  debug_R_BLACK[debug_scroller] = right_sensor_mov_average;
  debug_L_SPEED[debug_scroller] = LEFT_FORWARD_SPEED;
  debug_R_SPEED[debug_scroller] = RIGHT_FORWARD_SPEED;
  debug_Control[debug_scroller] = control_value;
  
}

char determine_current_state(void){
  //char current_state;  MISTAKE: This local existed in addition to the global!!

  // NOTE: MISTAKE: was using bitwise operator instead of logical AND !!!
  if((left_sensor_mov_average < black_line_edge_threshold) && (right_sensor_mov_average < black_line_edge_threshold)){
    current_state = OFF_LINE;
  }
  //need memory of when values peak and drop off
  if((left_sensor_mov_average > black_line_edge_threshold) && (right_sensor_mov_average< black_line_edge_threshold)){
    current_state = RIGHT_OF_LINE;
  }
  if((left_sensor_mov_average < black_line_edge_threshold) && (right_sensor_mov_average > black_line_edge_threshold)){
    current_state = LEFT_OF_LINE;
  }
  
  if((left_sensor_mov_average > black_line_edge_threshold) && (right_sensor_mov_average > black_line_edge_threshold)){
      current_state = ON_LINE;
  }
  // this is just for bang bang method
  if(bang == TRUE){
    if((left_sensor_mov_average > black_line_edge_threshold) && (right_sensor_mov_average > black_line_edge_threshold)){
      current_state = ON_LINE;
    }
  }
  return current_state;
}

void follow_black_line_bang(void){
  switch(determine_current_state()){
    case ON_LINE:
      LEFT_FORWARD_SPEED = speed_global;
      RIGHT_FORWARD_SPEED = speed_global;
    break;
    case LEFT_OF_LINE:
      LEFT_FORWARD_SPEED = (speed_global+speed_correction);
      RIGHT_FORWARD_SPEED = (speed_global);
      //needed correction
    break;
    case RIGHT_OF_LINE:
      LEFT_FORWARD_SPEED = (speed_global);
      RIGHT_FORWARD_SPEED = (speed_global + speed_correction);
      //needed correction
    break;
    case OFF_LINE:
      stop_car();
    break;
  }
  //previous_state = current_state;
}
