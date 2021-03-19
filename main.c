//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//
//  Mark Rubianes
//  Jan 2021
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.11.2)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include <stdio.h>
#include "macros.h"

/* Variables for my code for first timr moving motors
  int my_timer = 0x0;
  char stop_flag = 0;
  char turn_on = 0; //my timing variable
*/

// Global Variable declarations (**not defined here**)
extern char display_line[4][11];
extern char *display[4];
extern unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time = 1;
//a variable that is to be used both in main and in an interrupt service routine
//must be volatile: assigns the variable to a register, to be accessed quicker
//than if assigned to a memory location
extern char shape_state; //default, straight
extern char motion_state; //default, IDLE
extern unsigned int old_time_sequence = 0;
extern unsigned int my_time;
extern unsigned int cycle_time;
extern unsigned int period_count = 0;
extern char time_changed;

void main(void){
//------------------------------------------------------------------------------
// Main Programso
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings

  PM5CTL0 &= ~LOCKLPM5;

  Init_Ports();      // Initialize Ports  Note: switched with init_clocks because of endless loop
  Init_Clocks();     // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_Timers();                       // Initialize Timers
  Init_LCD();                          // Initialize LCD
// Place the contents of what you want on the display, in between the quotes
// Limited to 10 characters per line
//

  //display_line_0[] = {"asdfa"};
  //strcpy(display_line[0], "   NCSU   ");

  if(shape_state == STRAIGHT){
    strcpy(display_line[1], "STRAIGHT  ");
  }
  if(motion_state == IDLE){
    strcpy(display_line[0], "   IDLE   ");
  }


/*
  //BASIC FET BOARD TEST: To make sure forward and reverse are not on simultaneously
  if(((P6OUT | R_FORWARD) + (P6OUT | R_REVERSE)) == (R_FORWARD+R_REVERSE)){
    int oops=1;
  }
  if(((P6OUT | L_FORWARD)+(P6OUT | L_REVERSE)) == (L_FORWARD+L_REVERSE)){
    int oops=1;
  }

*/


  update_string(display_line[0], 0);
  //strcpy(display_line[1], " WOLFPACK ");
  update_string(display_line[1], 1);
  //strcpy(display_line[2], "  ECE306  ");
  update_string(display_line[2], 2);
  update_string(display_line[3], 3);
  enable_display_update();
  //Display_Update(3,2,1,0);

//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run


    if(Time_Sequence != old_time_sequence){ // if timer moved forward
      old_time_sequence = Time_Sequence;    // reset timer for new reference
      time_changed = 1;
      my_time++;
      cycle_time++;
    }


    //turing this off to  prevent overcurrent draw from USB connection to PC
    //P6OUT &= ~LCD_BACKLITE;

    switch(Time_Sequence){  //Time_Sequence is counting upwards in a timer
      //interrupt that is in timersB0.r43 (compiled code))
       // New code from "hints from project 3a
      case TIME_SEQ_250:
        if(one_time){  //one_time makes sure each case statement is only run once
          //per switch block, then one_time is set to zero (these statements are
          //still hit another 2- 3 hundred times after first actions are completed)
          //Init_LEDs();
          //lcd_BIG_mid();
          display_changed = 1;
          one_time = ZERO;
        }
        Time_Sequence = ZERO;    //gets set back to zero after reaching 250
        //counts (arbitrary rate that the instructor doesn't remember); this
        //restarts the switch case loop
                                //interrupt might also set this to zero
        break;

      case TIME_SEQ_200:
        if(one_time){
          //P6OUT |= GRN_LED;            // Change State of LED 5
          one_time = ZERO;
        }
        break;
      case TIME_SEQ_150:
        if(one_time){
          //P1OUT |= RED_LED;            // Change State of LED 4
          //P6OUT &= ~GRN_LED;           // Change State of LED 5
          one_time = ZERO;
        }
        break;
      case TIME_SEQ_100:
        if(one_time){
          lcd_4line();
          //P6OUT |= GRN_LED;            // Change State of LED 5
          display_changed = 1;
          one_time = ZERO;
        }
        break;
      case  TIME_SEQ_50:
        if(one_time){
          //P1OUT &= ~RED_LED;           // Change State of LED 4
          //P6OUT &= ~GRN_LED;           // Change State of LED 5
          one_time = ZERO;
        }
        break;
      default: break;
    }
    //Switches_Process();       // Deprecated, using interrupts now
    motion_process();         // motion state processing
    Display_Process();
    //fet_test();


  }
 }




