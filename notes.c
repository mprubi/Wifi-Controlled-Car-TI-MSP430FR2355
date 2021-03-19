//____________________


/*  //This is my first code to start and stop the motor for predefined time, defined in MAIN
    my_timer++;
    if(my_timer > 0xC350) { //about one second of time
      stop_flag = 1;
      my_timer = RESET_STATE;
    }
    if((my_timer < 0xC350) && (stop_flag == 0)){
      turn_on++;  //move forward/motor power
        if(turn_on == 75){
          //GREEN_LED_ON();
          R_FORWARD_ON();
          L_FORWARD_ON();
        }
        if(turn_on == 150){
          //GREEN_LED_OFF()
          R_FORWARD_OFF();
          L_FORWARD_OFF();
          turn_on = RESET_STATE;
        }
    }
    if(stop_flag == 1){
      //GREEN_LED_OFF();
          R_FORWARD_OFF();
          L_FORWARD_OFF();
          //my_timer = ZERO;
     }

*/

/*
//Carlsons example code for project 3, getting car to briefly move, no need to use this. Just keeping to look back

    if(Time_Sequence > 250){
      Time_Sequence = ZERO;

      if(my_start_moving == 1){
        my_moving++;
      }
      if(my_start_moving >= 2){
        forward();
      }
      if(my_start_moving >= 4){
        stop();
        my_moving = ZERO;
        my_start_moving = ZERO;
      }
    }


  }



//------------------------------------------------------------------------------
}

*/

    //--------------------------------------------------------------
    /*code for basic bare bones custom timing derived from Time_sequence
        the importance of this code is to be able to use timing without access
        to advanced clock tools (like the constraint imposed by an arduino)
      */
    /*

      extern unsigned int old_time_sequence = 0;
      extern unsigned int my_time;
      extern unsigned int period_count = 0;

      if(Time_Sequence != old_time_sequence){ // if timer moved forward
      my_time++;                            // derive new timing variable
      old_time_sequence = Time_Sequence;    // reset timer for new reference
      period_count++;  // "poor man's" PWM
      time_changed = 1;
    }
    //----------------------------------------------------------------
*/


/*
void wheels_straight_control(void){
switch(operation){ //switch control should determine whether this operation is true
    case IDLE:  //does nothing but stay in this state until button is pressed
//will have to check everything in idle every time, but other cases need a
//one_time varible, polling for wait state

    case WAIT:  // just for moving away from button, NOT for switch debouncing
// use timing to wait until safe to check button state
// wait for a portion of the period (a few count cycles)
// commence counting of some count values using myTime
//myTime = 0;
//if(myTime == some value ){swich to next case, no break statement}

    case OFF:
//turn wheels off;
break;
    case ON:
if(period_count >= DEFINED_PERIOD){
period_count = 0;
//turn both wheels on
      }
if(period_count >= TOO_FAST){
//turn fast wheel off
      }
break;
default: break;
  }
}

*/

/*
switch(STRAIGHT){
        case IDLE:
break;
        case WAIT:
//count some time off your my_time value
break;
        case START:
//this is where you place your wheel control
break;
        case END:
break;
default: break;
      }

switch(figure8_state){
        case IDLE:
break;
        case WAIT:
//count some time off your my_time value
break;
        case START:
//this is where you place your wheel control
break;
        case END:
break;
default: break;
      }

*/
/*
//my countdown timer code

if(this_timer != my_time){
      wait_count++;
      this_timer = my_time;
      GREEN_LED_OFF();
      RED_LED_TOGGLE();
    }
    if(wait_count == WAIT_THIS_LONG) {
      motion_state = START;
      wait_count = RESET_STATE;
    }
*/

/*

A few things to remember this week.
There is a set of Slides under Lecture Slides that discuss button configuration.
You can use your time to get that to work. A good test is to comment out the LED
control for the Switch Statement in Main and use a switch to turn on an Led and
Turn off an LED.
We covered extern and globals in class, but I am sure that likely is one of those
discussions that did not get fully absorbed.

In one of your files where you first require a variable, you will define a global
[unless it is a variable only needed in a specific function]. Defining that global
will be in the form of:

unsigned int period_count;

Since it is not assigned an initial value, it will be placed in the .bss section
[This is more to help with your first test].

As you continue coding and you discover you need to use that same global in another
file, then you will identify to that file that it has already been defined by giving
it the global definition in the firm:

extern unsigned int period_count;

*/
//ctrl A, ctrl T  re-does indentation as configured in settings

