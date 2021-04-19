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
extern volatile unsigned char start_process;
extern volatile unsigned char current_start_process = 0;

extern volatile int time_seconds;
extern volatile int time_half_seconds;
int start_counting = 0;
char one_time = 1;

// for Tx
char UCA1_index;
char test_command[11];
//for Rx
extern volatile unsigned int usb_rx_ring_wr;
extern volatile char USB_Char_Rx [SMALL_RING_SIZE];
extern unsigned int usb_rx_ring_rd;
char USB_Process_Buff [32];
char processing_bool;

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
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_Timers();                       // Initialize Timers
  Init_LCD();                          // Initialize LCD
  Init_ADC();                          //Initialize ADC
  Init_Serial_UCA1();                  // Initialize Serial Port for USB

  //Startup  display message
  strcpy(display_line[0], "          ");
  update_string(display_line[0], 0);
  strcpy(display_line[1], "          ");
  update_string(display_line[1], 1);
  strcpy(display_line[2], "          ");
  update_string(display_line[2], 2);
  strcpy(display_line[3], "          ");
  update_string(display_line[3], 3);

  display_changed = 1;

  enable_display_update();  //removed as per HW 6 suggestions
  //Display_Update(3,2,1,0);


  //Tx code (relates to transmit ISR, case 4)
  UCA1_index = 0;
  UCA1IE |= UCTXIE; // Enable RX interrupt
  UCA1TXBUF = test_command[0];

  //RX code
//  if(usb_rx_ring_rd != usb_rx_ring_wr){
//    //there is a character to copy to the process buffer now
//    if()
//    USB_Process_Buff[0] = USB_Char_Rx[usb_rx_ring_rd]
//
//    //null the ring character after transferring it
//    USB_Char_Rx[usb_rx_ring_rd] = '0';
//
//    usb_rx_ring_rd++;
//
//    if(!processing_bool){
//      //clear everything and continue polling for CR/LF
//
//    }
//  }

//------------------------------------------------------------------------------
// Begining of Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {

    //TODO: implement backlight power on based on ambient light detection from photo resistor
    // and turn completely off, with display refresh turned off/on on command
    //P6OUT &= ~LCD_BACKLITE;

    //TODO: consider slowing down the display_update refresh rate to conserve energy

    //TODO: Display ADC values on LCD screen in more organized fashion




/* TRANSMIT WHILE LOOP CODE FOR DEBUGGING
The following function can be called to transmit a character passed to it. You will need to protect
against a stall.
void
out_character (char
//
// The while loop will stall as long as the Flag is not set [port is busy]
while (!(UCA0IFG & UCTXIFG)); // USCI_A0 TX buffer ready?
UCA0TXBUF = character;
//
}
*/

/*
//    if(current_start_process != start_process){
//
//      switch(proj_6_state){
//      case 0:
//        if(ADC_Left_Sensor < 1020){
//          if(one_time){
//            start_counting = time_seconds + 1;
//            one_time = 0;
//          }
//          if(start_counting == time_seconds){
//            GREEN_LED_ON();
//            forward(MEDIUM);
//          }
//        }else if(ADC_Left_Sensor > 1020){
//          proj_6_state = 1;
//          one_time = 1;
//          GREEN_LED_OFF();
//        }
//        break;
//      case 1:
//        stop_car();
//
//        if(one_time){
//          start_counting = time_seconds + 1;
//          one_time = 0;
//          GREEN_LED_ON();
//        }
//        if(start_counting == time_seconds){
//          GREEN_LED_OFF();
//          proj_6_state = 2;
//          one_time = 1;
//        }
//        break;
//      case 2:
//        if(one_time){
//          start_counting = time_half_seconds + 1;
//          one_time = 0;
//          GREEN_LED_ON();
//
//          LEFT_FORWARD_SPEED = FAST;
//          RIGHT_REVERSE_SPEED = FAST;
//        }
//        if(start_counting == time_half_seconds){
//          stop_car();
//          proj_6_state = 0;
//          one_time = 1;
//          current_start_process = start_process;
//
//        }
//
//      break;
//    }
//  }
*/

//// main function calls
    //carlsons_code();
    //motion_process();         // motion state processing
    display_menu_state_process();
    update_display = 1;
    Display_Process();
    //fet_test();
  }
 }


