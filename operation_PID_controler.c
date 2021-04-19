//
//#deine WHITE_STATE
//
//// Monitoring Loop pseudo code
//void LineFollowing(void)
//  {
//    // Check if it is time to re-evaluate system
//    if(ready){
//        ready = false;
//        // Get State Information
//        current_state = GetCurrentState(); // Check combinational sensor state (On line, off line, right of line, left of line)
//        if(current_state == WHITE_STATE) { // if we are currently reading white, controller is not valid so turn back until we see the line
//          switch( previous_state ) {
//          case LEFT_STATE:
//            TurnRight();
//            break;
//          case RIGHT_STATE:
//            TurnLeft();
//            break;
//          default:
//            SpinInPlace();// If we completely overshoot the line and don’t know how to go back, spin and hope for the best
//            break;
//          }
//        }
//        else WheelControl(SimplePid); // we are not just reading white so we are able to track set point
//        // Set variables for next iteration
//        previous_state = current_state;
//        ready = false;
//      }
//  }
//
//// Basic PID pseudo code
//int control_SimplePid(void)
//  {
//    // Get sensor readings
//    Read_Adc();
//    // Calculate error
//    // NOTE: Your error calculation will differ depending on how many inputs you have and what the desired alignment of your vehicle is!
//    e = reference - (left_sensor - right_sensor); // Magnitude of error indicates how far off the line, sign of error indicates direction of movement
//    // Set PID error variables
//    current_error = e;
//    sum_error = sum_error + (current_error*sample_time);
//    delta_error = (current_error - previous_error)/sample_time;
//    previous_error = current_error; // save error for next loop
//    // Calculate individual controller outputs
//    P = current_error;
//    I = sum_error;
//    D = delta_error;
//    // Calculate PID control signal
//    u = Kp*P + Ki*I + Kd*D;
//    Return u; //this is the total signal to the actuator
//  }
//
//// Basic Control Actuation pseudo code
//void WheelControl(control)
//  {
//    // Assume vehicle is designed to move at some cruise speed, and adjust wheel PWM values to track a curved line
//    // Negative valued control indicates a right turn, positive valued control indicates left turn
//    lspeed = CRUISE_SPEED - control;
//    rspeed = CRUISE_SPEED + control;
//    // Saturate speed values within PWM range
//    if( lspeed < MIN_SPEED ) lspeed = MIN_SPEED; if( rspeed < MIN_SPEED ) rspeed = MIN_SPEED;
//    else if( lspeed > MAX_SPEED ) lspeed = MAX_SPEED; else if( rspeed > MAX_SPEED ) rspeed = MAX_SPEED;
//    // Set speed values to PWM registers
//    LEFT_FORWARD_SPEED = lspeed;
//    RIGHT_FORWARD_SPEED = rspeed;
//  }