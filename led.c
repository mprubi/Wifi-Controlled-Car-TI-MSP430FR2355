////------------------------------------------------------------------------------
////
////  Description: LED file
////
////
////  Mark Rubianes
////  Jan 2021
////  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.11.2)
////------------------------------------------------------------------------------
//
//
//#include  "functions.h"
//#include  "msp430.h"
//#include <string.h>
//#include "macros.h"
//
////unsigned int Time_Sequence;
//char execute_once;
//
//void Init_LEDs(void){
////------------------------------------------------------------------------------
//// LED Configurations
////------------------------------------------------------------------------------
//// Turns on both LEDs
//  P1OUT &= ~RED_LED;
//  P6OUT &= ~GRN_LED;
////------------------------------------------------------------------------------
//}
//
///*
//void LED_blink_at_rate(void){
//  switch(Time_Sequence){
//
//      case TIME_SEQ_250:
//        Time_Sequence = ZERO;
//        break;
//      case TIME_SEQ_200:
//        if(execute_once){
//          GREEN_LED_TOGGLE();
//          execute_once = ZERO;
//        }
//        break;
//      case TIME_SEQ_150:
//        if(execute_once){
//          RED_LED_TOGGLE();
//          GREEN_LED_TOGGLE();
//          execute_once = ZERO;
//        }
//        break;
//      case TIME_SEQ_100:
//        if(execute_once){
//          GREEN_LED_TOGGLE();
//          execute_once = ZERO;
//        }
//        break;
//      case  TIME_SEQ_50:
//        if(execute_once){
//          RED_LED_TOGGLE();
//          GREEN_LED_TOGGLE();
//          execute_once = ZERO;
//        }
//        break;
//      default: break;
//    }
//}
//
//*/