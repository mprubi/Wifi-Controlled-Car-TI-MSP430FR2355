//This file is for timer configurations

// TimerB0 initialization sets up both B0_0 , B0_1 B0_2 and overflow
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"

void Init_Timers(void){
  Init_Timer_B0();
  Init_Timer_B1();
  Init_Timer_B2();
  Init_Timer_B3();
}

void Init_Timer_B0(void){
  TB0CTL = TBSSEL__SMCLK; // set TimerB0 register control to use SMCLK source, ACLK could have been another source

  TB0CTL |= TBCLR ; // Resets TB0R , clock divider, count direction
                  // clears the counter
  TB0CTL |= MC__CONTINOUS; // Continuous up
  // counter counts from 0x0000 to 0xFFFF (0 to 65535), then starts over at zero
  // This generates a saw tooth wave

  TB0CTL |= ID__2; // Divide clock by 2
  // ID_1 would be a divide by two also (2^1 power)

  //Question: does EX stand for "external" or "extra"? Carlson used both terms
  TB0EX0 = TBIDEX__8; // EX ("Extra" divider)Divide clock by an additional 8
  // or TBIDEX_7 would also be a divide by 8
  //at this point, the timer would be at 500kHz

  TB0CCR0 = CCR0_50_MSEC_INTERVAL; // CCR0: this is the value up the saw tooth ramp
   // at which you want to trigger an interrupt and then add back in the ISR
   // this value is the interval that you want to add (when in the ISR) back to
  //continue to progress

  TB0CCTL0 &= ~CCIFG; //clear any possible pending interrupt flags
  TB0CCTL0 |= CCIE; // CCR0 enable interrupt

 /*
   CTL:  "Capture Control"
   CCR: "Capture Compare Register"
   There are SEVEN usable Timers, and SEVEN usable Capture Compare Registers
   When the TB0R = TB0CCR# (when the preset point on the saw tooth wave arrives),
    a hardware generated interrupt happens,
      the CCIFG (Interrupt Flag)
*/
  // CCR1 will be utilized for the debounce timer for SW1
  TB0CCR1 = CCR0_200_MSEC_INTERVAL; // CCR1 (200 msec is for update display)
  TB0CCTL1 &= ~CCIFG;  //clear any possible pending interrupt flags
  TB0CCTL1 |= CCIE; // CCR1 enable interrupt

  // CCR2 will be utilized for the debounce timer for SW2
  TB0CCR2 = Time_Sequence_Rate; // CCR2 (for Time_sequence)
  TB0CCTL2 &= ~CCIFG;  //clear any possible pending interrupt flags
  TB0CCTL2 |= CCIE; // CCR2 enable interrupt

//  QUESTION: For the TimerB0 of the MSP430FR2355 there are ONLY 3 Capture Compare Registers.
//TB0CCR0, TB0CCR1, TB0CCR2  WTF?

  TB0CTL &= ~TBIE ; // Disable Overflow Interrupt
  TB0CTL &= ~TBIFG ; // Clear Overflow Interrupt flag
  //When the TB0R = TB0CCR# rolls over from 65535 to 0,
  // a hardware generated interrupt happens, the TBIFG (Timer B0 Overflow Interrupt Flag)
}
//

void Init_Timer_B1(void){
}

void Init_Timer_B2(void){
}

// TimerB3 will be for PWM
// CCR0 will be the wheel period to work with (about 50,000) (slow might be 30K)
void Init_Timer_B3(void){
}