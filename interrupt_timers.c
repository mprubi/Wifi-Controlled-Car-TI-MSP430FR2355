// This file is for timer operation
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"

extern volatile int HW6_10_Count = 0;
extern volatile int Time_Seq_Reset_Count = 0;
extern volatile unsigned char update_display;
//extern volatile unsigned char update_display_timer = 0;
extern volatile unsigned int Time_Sequence = 0;

//proving my adc setup
extern volatile unsigned int ADC_Thumb;
extern volatile unsigned int ADC_Left_Sensor;
extern volatile unsigned int ADC_Right_Sensor;
extern volatile unsigned int left_sensor_mov_average, right_sensor_mov_average;
extern char display_line[4][11];
extern char *display[4];
extern unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile char adc_char[4];
extern char proj_6_state;
extern volatile unsigned char start_process = 0;
extern volatile int time_half_seconds = 0;
volatile char from_forward;
volatile char from_reverse;
extern unsigned int speed_global;
volatile char one_second_from_fifty_msec_count = 0;
volatile char one_second_delay = FALSE;
volatile int time_seconds = 0;
extern volatile int FET_delay_timer;

/* Various Timer related notes
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

Rule of thumb: don't have a faster timer than what you need and use it to count
up to slower times if the fast timer is completely unused

*/


//TODO: TB0 CC0 is for the update display and other similar order timers
// TB0 CCR1 and CCR 2 could be for debouncing SW1 and SW2
// TB1 can be used for the order of 50 miliseconds (going forward and reverse?)
//     CCR0 using SMCLK again
//TB2 can be on the order of seconds, using ACLCK as the source
// use this clock with 5 or 10 seconds to avoid having to send IoT commands
// every 1 second to go across atrium floor

// ACLK will be good for obtaining minutes to turn on LED on wifi module to indicate
//whether it's received a command in the last minute or not

#pragma vector = TIMER_B0_CCR_0_VECTOR
__interrupt void TIMER_B0_CCR_0_ISR(void){
  // IV (Interrupt Vector) for TimerB0 CCRO
  //Usage of these vectors is to derive any number of
  // additional timing elements (variables) for different applications in the program
  TB0CCR0 += CCR0_16_MSEC_INTERVAL; // Add Offset to TBCCR0

    //GREEN_LED_TOGGLE();
}

#pragma vector= TIMER_B0_CCR_1_2_OVFL_VECTOR
__interrupt void TIMER_B0_CCR_1_2_OVFL_ISR(void){

  // IV (Interrupt Vector) for TimerB0 CCR1, CCR2, and OFVL

  switch(__even_in_range(TB0IV, 14)){
  case 0:
    break; // No interrupt
  case 2:
    TB0CCR1 += TB0_SEC_0_100; //increment timer
    TB0CCTL1 &= ~CCIE; //disable timer

    //debug actions
    //BACKLIGHT_TOGGLE();
    //GREEN_LED_TOGGLE();

    P4IE |= SW1;  // re-enable SW1 via "Interrupt Enable" bit
    P4IFG &= ~SW1; // 2) clear SW1 associated flags

    break;
  case 4:
    TB0CCR2 += TB0_SEC_0_100; //increment timer

    TB0CCTL2 &= ~CCIE; //disable timer

    //debug actions
    //BACKLIGHT_TOGGLE();
    //RED_LED_TOGGLE();

    //after this timer interrupt triggers (TB0_SEC_1_250 later),
    // re-enable switch interrupts so they can be reused

    P2IE |= SW2;  // 1) "Interrupt Enable" bit
    P2IFG &= ~SW2; // 2) IFG SW1 cleared

    break;
  case 14: // overflow
    break;
  default:
    break;
  }
  //
}

#pragma vector = TIMER_B1_CCR_0_VECTOR
__interrupt void TIMER_B1_CCR_0_ISR(void){
  // TimerB1 0 Interrupt handler

  TB1CCR0 += TB1_SEC_0_200; // Add Offset to TBCCR0

  //BACKLIGHT_TOGGLE();  //debug

  // update display with hex values from ADC
  //ADCCTL0 |= ADCENC;



//  HEXtoBCD(ADC_Left_Sensor);
  HEXtoBCD(left_sensor_mov_average);
  adc_line(2,0);

//  HEXtoBCD(ADC_Right_Sensor);
  HEXtoBCD(right_sensor_mov_average);
  adc_line(2,6);

  HEXtoBCD(ADC_Thumb);
  adc_line(3,6);

//  strcpy(display_line[3], "Thumb:");
//  update_string(display_line[3], 3);

  display_changed = 1;

}

#pragma vector = TIMER_B1_CCR_1_2_OVFL_VECTOR
__interrupt void TIMER_B1_CCR_1_2_OVFL_ISR(void){
  switch(__even_in_range(TB1IV, 14)){
  case 0:
    break; // No interrupt
  case 2:
    //30 msec timer here (for ADC measurements)
    TB1CCR1 += TB1_SEC_0_030;
    RED_LED_TOGGLE();

      //BACKLIGHT_TOGGLE();  //debug

    ADCCTL0 |= ADCENC;

    break;
  case 4:
    //50 msec timer here
    TB1CCR1 += TB1_SEC_0_050;
    GREEN_LED_TOGGLE();

    break;
  case 14: // overflow
    break;
  default:
    break;
  }
}

#pragma vector = TIMER_B2_CCR_0_VECTOR
__interrupt void TIMER_B2_CCR_0_ISR(void){
  // TimerB2 0 Interrupt handler

  TB2CCR0 += TB2_MIN_0_005; // Half second offset

  time_half_seconds++;

  //RED_LED_TOGGLE();

}

#pragma vector = TIMER_B2_CCR_1_2_OVFL_VECTOR
__interrupt void TIMER_B2_CCR_1_2_OVFL_ISR(void){
  switch(__even_in_range(TB2IV, 14)){
  case 0:
    break; // No interrupt
  case 2: // CCR1
    TB2CCR1 += 512; // one second offset
    //TB2CCTL1 &= ~CCIFG; // 3) clear any possible pending interrupt flags
    //TB2CCTL1 &= ~CCIE; // disable timer until I need it again
    //(inside a counter for multiple seconds)

    //MISTAKE: TB2CCR2 in case 4 was incorrectly named TB2CCR1,
    //and set to different interval, effectively halting CCR1 interrupt periodically
    time_seconds++;
    FET_delay_timer++;
    //RED_LED_TOGGLE();

    if(time_seconds != FET_delay_timer){ // Warning[Pa082]: undefined behavior: the order of volatile accesses is undefined in this statement

      if(from_forward){
        FET_delay_timer = time_seconds; //turn timer back off
        from_forward = FALSE;
        forward(speed_global);
      }
      if(from_reverse){
        FET_delay_timer = time_seconds; //turn timer back off
        from_reverse = FALSE;
        reverse(speed_global);
      }
    }
    break;
  case 4: //CCR2
    TB2CCR2 += TB2_MIN_1_000; // one minute

    //unused

    break;
  case 14: // overflow
    break;
  default:
    break;
  }
}


//Prcessor won't see this ISR because TB3 is a special purpose timer for PWM
// which operates on sepparate hardware, therefore, main won't even see the ISR


// Used to reside in TB0 CCR0 with 16 msec interrupt
//  Time_Seq_Reset_Count++;
//  Time_Sequence++;
//  if(Time_Seq_Reset_Count == 250){
//    Time_Sequence = 0;
//    Time_Seq_Reset_Count = 0;
//  }

//  // Use for reference on how to derive other time intervals from a timer
//  project_5_timer++;
//  update_display_timer++;
//  if(project_5_timer == 30){
//    P6OUT ^= LCD_BACKLITE;
//    project_5_timer = 0;
//    project_5_Seconds++;  //this incrementation gave me seconds
//  }

//  //keep for reference on how to refresh display variable. Used to reside in TB0 CCR1 with 10 mSec interrupt
//    update_display_timer++;
//    if(update_display_timer == 20){  // equates to every 200 msec WHEN interrupt occurs every 10 msec
//      update_display_timer = 0;
//      update_display = 1;
//    }
