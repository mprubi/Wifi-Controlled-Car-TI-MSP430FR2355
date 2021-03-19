// This file is for timer operation
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"

volatile unsigned int debounce_count = 0;
extern volatile int HW6_10_Count = 0;
extern volatile int Time_Seq_Reset_Count = 0;
extern volatile unsigned char update_display;
extern volatile int Time_Sequence = 0;
extern volatile int project_5_timer = 0;
extern volatile int project_5_Seconds = 0;

/*
Two interrupt vectors are associated with the 16 bit
Timer _B module:
  – TBxCCR0 interrupt vector for TBxCCR0 CCIFG
  – TBxIV interrupt vector for all other CCIFG flags and TBIFG

-In capture mode, any CCIFG flag is set when a timer
value is captured in the associated TBxCCRn register

-In compare mode, any CCIFG flag is set if TBxR counts
to the associated TBxCCRn value Software may also
set or clear any CCIFG flag All CCIFG flags request an
interrupt when their corresponding CCIE bit and the
GIE bit are set

(reason why each Timer_Bx has two ISR's associated with them)
The TBxCCR0 CCIFG flag has the highest Timer _B
interrupt priority and has a dedicated interrupt vector.The TBxCCR0 CCIFG flag is automatically reset when
the TBxCCR0 interrupt request is serviced.


These functions determine what I'm going to do when
the timers expire, triggering an interrupt
*/

#pragma vector = TIMERB0_CCR0_VECTOR
__interrupt void Timer0_B0_ISR(void){
  // TimerB0 0 Interrupt handler
  // ...... Add What you need happen in the interrupt ......
  TB0CCR0 += CCR0_50_MSEC_INTERVAL; // Add Offset to TBCCR0

  project_5_timer++;

  if(project_5_timer == 30){
    P6OUT ^= LCD_BACKLITE;
    project_5_timer = 0;
    project_5_Seconds++;
  }
}

#pragma vector= TIMERB1_CCR0_VECTOR
__interrupt void Timer0_B1_ISR(void){

//
// TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler

  switch(__even_in_range(TB0IV, 14)){
    case 0:
      break; // No interrupt
    case 2:
      //...... Add What you need happen in the interrupt ......

      //Question: Should I clear faults with these CCRegisters in these case statements here?
      TB0CCR1 += CCR0_200_MSEC_INTERVAL; // Add Offset to TBCCR1
      update_display = 1;

      if(project_5_timer == 30){
    P6OUT ^= LCD_BACKLITE;
    project_5_timer = 0;
  }

      break;
    case 4:
      //...... Add What you need happen in the interrupt ......
      TB0CCR2 += Time_Sequence_Rate; // for Time_Sequence
      Time_Seq_Reset_Count++;
      Time_Sequence++;

//      HW6_10_Count++;
//
//      if(HW6_10_Count == 16){
//        P6OUT ^= LCD_BACKLITE;
//        HW6_10_Count = 0;
//      }

      if(Time_Seq_Reset_Count == 250){
        Time_Sequence = 0;
        Time_Seq_Reset_Count = 0;
      }

      break;
    case 14: // overflow
      //...... Add What you need happen in the interrupt ......
        break;
    default:
      break;
  }
//
}

#pragma vector = TIMERB1_CCR0_VECTOR
__interrupt void Timer1_B0_ISR(void){
  // TimerB1 0 Interrupt handler

  TB1CCR0 += CCR0_50_MSEC_INTERVAL; // Add Offset to TBCCR0

}

#pragma vector = TIMERB2_CCR0_VECTOR
__interrupt void Timer0_B1_ISR(void){
  switch(__even_in_range(TB0IV, 14)){
    case 0:
      break; // No interrupt
    case 2:
      break;
    case 4:
      break;
    case 14: // overflow
        break;
    default:
      break;
}

#pragma vector = TIMERB1_CCR0_VECTOR
__interrupt void Timer1_B0_ISR(void){
  // TimerB1 0 Interrupt handler

  TB1CCR0 += CCR0_50_MSEC_INTERVAL; // Add Offset to TBCCR0

}

#pragma vector = TIMERB2_CCR0_VECTOR
__interrupt void Timer0_B1_ISR(void){
  switch(__even_in_range(TB0IV, 14)){
    case 0:
      break; // No interrupt
    case 2:
      break;
    case 4:
      break;
    case 14: // overflow
        break;
    default:
      break;
}

#pragma vector = TIMERB3_CCR0_VECTOR
__interrupt void Timer1_B0_ISR(void){
  // TimerB1 0 Interrupt handler

  TB1CCR0 += CCR0_50_MSEC_INTERVAL; // Add Offset to TBCCR0

}

#pragma vector = TIMERB3_CCR0_VECTOR
__interrupt void Timer0_B1_ISR(void){
  switch(__even_in_range(TB0IV, 14)){
    case 0:
      break; // No interrupt
    case 2:
      break;
    case 4:
      break;
    case 14: // overflow
        break;
    default:
      break;
}
/*

INTERVAL = Clock / Input Divider [ID] / Input Divider Expansion [T?xEX0] / ( 1 / Desired Time )
Clock = 8,000,000
[SMCLK
Input Divider [ID] = 1,2,4,8 [this example 2]
Input Divider Expansion [T?xEX0] = 1,2,3,4,5,6,7,8 [this example 8]
Desired Time Between Interrupts =
= 8,000,000 / 2 / 8 / ( 1 / 5msec )
= 8,000,000 / 2 / 8 / 200
= 8,000,000 / 2 / 8 / 200
= 4,000,000 / 8 / 200
= 500,000 / 200
= 2,500
Example for Timer A0 Capture Compare Register 0 for desired interrupt every 5msec.
//#define
TB0CCR0 _INTERVAL (2500) // 8,000,000 / 2 / 8 / (1 /

*/