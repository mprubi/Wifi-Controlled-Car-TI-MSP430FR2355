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
#include "macros.h"

//a variable that is to be used both in main and in an interrupt service routine
//must be volatile: assigns the variable to a register, to be accessed quicker
//than if assigned to a memory location

extern char display_line[4][11];
extern char *display[4];
extern unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned long ADC_Left_Sensor;
//extern volatile unsigned char start_process;
//extern volatile unsigned char current_start_process = 0;
//extern volatile int time_seconds,
//                    time_half_seconds;
//int start_counting = 0;
//char one_time = 1;
extern volatile char msg_recieved_from_IoT,
                     msg_recieved_from_PC,
                     activate_open_TCP_port_timer;
extern char ok_to_parse_recieved_iot_commands;
extern volatile char bang;
extern volatile char PID_control_enabled;
int oops = 0;

extern volatile char SW2_Pressed;
extern volatile char SW1_Pressed;


void main(void){
//------------------------------------------------------------------------------
// Main Programs
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings

  PM5CTL0 &= ~LOCKLPM5;

  Init_Ports();                        // Initialize Ports  Note: switched with init_clocks because of endless loop
  Init_Clocks();                       // Initialize Clock System
  Init_Display();                      // Initialize Variables and Initial Conditions
  Init_Timers();                       // Initialize Timers
  Init_LCD();                          // Initialize LCD
  Init_ADC();                          //Initialize ADC
  Init_Serial_UCA1(BAUD_115200);                  // Initialize Serial Port for USB
  Init_Serial_UCA0(BAUD_115200);
  Init_Interrupts(); // Interrupts are disabled by default, enable them.

  display_changed = 1; //refresh LCD module

  activate_open_TCP_port_timer = TRUE;

//------------------------------------------------------------------------------
// Begining of Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {
    
//    if(!oops){
          
      update_display = 1;
      display_changed = 1;
      //clear_display();
      if(msg_recieved_from_IoT){
        RX_IoT_Process_Buff();
      }
      if(ok_to_parse_recieved_iot_commands){
        parse_commands_from_IoT();
      }
      if(msg_recieved_from_PC){
        RX_USB_Process_Buff();
      }
      if(bang == TRUE){
        follow_black_line_bang();
      }
      if(PID_control_enabled == TRUE){
        follow_black_line();
      }
      
      if(SW2_Pressed){
        forward(SLOW, STRAIGHT, 0);
      }
      if(SW1_Pressed){
        stop_car();
      }
      
      
      //    #define TRAVELING
      //    #define INTERCEPTED_8
      //    #define PIVOT_RIGHT_INTERCEPT
      //    #define FORWARD_AGAIN
      //    #define FORWARD_UNTIL_INTERCEPTED
      //    char start_intercept, intercept_state, blck_line_state;
      //
      //    if(start_intercept){
      //      switch(intercept_state){
      //      case TRAVELING:
      //        forward(MEDIUM);
      //        blck_line_state = determine_current_state();
      //        if(blck_line_state == ON_LINE){
      //          stop_car();
      //          intercept_state = INTERCEPTED_8
      //        }
      //        break;
      //      case INTERCEPTED_8:
      //        forward(SLOW);
      //        // kick off timer, stop and change state
      //        break;
      //      case PIVOT_RIGHT_INTERCEPT:
      //        pivot(RIGHT_PIVOT, 13000)}
      //        intercept_state = FORWARD_AGAIN;
      //        break;
      //      case FORWARD_AGAIN:
      //        forward(MEDIUM);
      //        //kick off timer for longer time, stop, change state
      //        break;
      //      case FORWARD_UNTIL_INTERCEPTED:
      //      }
      //    }
      
      //    follow_black_line();
      
      strcpy(display_line[0], "FJDNFIEJGN");
      update_string(display_line[0], 0);
      strcpy(display_line[1], "KFOIROTKGJ");
      update_string(display_line[1], 1);
      strcpy(display_line[2], "LLLLLLLLLL");
      update_string(display_line[2], 2);
      strcpy(display_line[3], "FFFFFFFFFF");
      update_string(display_line[3], 3);
      
      display_changed = 1;
      
      display_menu_state_process();
      update_display = 1;
      Display_Process();  // carlson's compiled code
      
      
      
      //TODO: implement backlight power on based on ambient light detection from photo resistor
      // and turn completely off, with display refresh turned off/on on command
      
      //TODO: consider slowing down the display_update refresh rate to conserve energy
      
      //TODO: Display ADC values on LCD screen in more organized fashion
      if((LEFT_REVERSE_SPEED && LEFT_FORWARD_SPEED) || (RIGHT_REVERSE_SPEED && RIGHT_FORWARD_SPEED)){
        oops=1;
      }
//    }else if(oops){
//      stop_car();
//      P6DIR &= ~LCD_BACKLITE;
//      GREEN_LED_ON();
//      RED_LED_ON();
//    }
  }
 }


