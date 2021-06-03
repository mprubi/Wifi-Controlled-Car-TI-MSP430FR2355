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

// If using ACLCK as src, then there are no ID aor TxyEX0 dividers

//Timer uses:
//TB0_CCR0 - update display (order of 200 mSec)
//TB0_CCR1 - SW1 debounce
//TB0_CCR2 - SW2 debounce

void Init_Timer_B0(void){
  TB0CTL = TBSSEL__SMCLK; // set TimerB0 register control to use SMCLK source, ACLK could have been another source

  TB0CTL |= TBCLR ; // Resets TB0R , clock divider, count direction
                  // clears the counter
  TB0CTL |= MC__CONTINOUS; // Continuous up
  // counter counts from 0x0000 to 0xFFFF (0 to 65535), then starts over at zero
  // This generates a saw tooth wave

  TB0CTL |= ID__2; // Divide clock by 2
  // ID_1 would be a divide by two also (2^1 power)

  TB0EX0 = TBIDEX__8; // EX ("Extra" divider)Divide clock by an additional 8
  // or TBIDEX_7 would also be a divide by 8
  //at this point, the timer would be at 500kHz

  TB0CCR0 = CCR0_16_MSEC_INTERVAL; // CCR0: this is the value up the saw tooth ramp
   // at which you want to trigger an interrupt and then add back in the ISR
   // this value is the interval that you want to add (when in the ISR) back to
  //continue to progress

  TB0CCTL0 &= ~CCIFG; //clear any possible pending interrupt flags
  TB0CCTL0 |= CCIE; // CCR0 enable interrupt

/*
   CTL:  "Capture Control"
   CCR: "Capture Compare Register"
   There are SEVEN usable Timers, and UP TO SEVEN usable Capture Compare Registers
   When the TB0R = TB0CCR# (when the preset point on the saw tooth wave arrives),
    a hardware generated interrupt happens,
      the CCIFG (Interrupt Flag)
*/
  // CCR1 will be utilized for the debounce timer for SW1
  TB0CCR1 = TB0_SEC_0_100; // CCR1
  TB0CCTL1 &= ~CCIFG;  //clear any possible pending interrupt flags
  TB0CCTL1 |= CCIE; // CCR1 enable interrupt

  // CCR2 will be utilized for the debounce timer for SW2
  TB0CCR2 = TB0_SEC_0_100; // CCR2
  TB0CCTL2 &= ~CCIFG;  //clear any possible pending interrupt flags
  TB0CCTL2 |= CCIE; // CCR2 enable interrupt

  TB0CTL |= TBIE ; // Enable Overflow Interrupt (used for IoT module reset release on startup)
  TB0CTL &= ~TBIFG ; // Clear Overflow Interrupt flag
  //When the TB0R = TB0CCR# rolls over from 65535 to 0,
  // a hardware generated interrupt happens, the TBIFG (Timer B0 Overflow Interrupt Flag)
}
//

//Timer uses: SMCLK
//TB1_CCR0 - HextoBCD conversions and displaying ADC values
//TB1_CCR1 - ADC measurements
//TB1_CCR2 -
void Init_Timer_B1(void){

  // SMCLK -> 8mHz / 8 / 8 / (1 / desired time)
  TB1CTL = TBSSEL__SMCLK;

  TB1CTL |= TBCLR ; // Resets TB0R , clock divider, count direction

  TB1CTL |= MC__CONTINOUS;

  TB1CTL |= ID__8;

  TB1EX0 = TBIDEX__8;

  TB1CCR0 = TB1_SEC_0_200; // 200 msec
  TB1CCTL0 &= ~CCIFG;
  TB1CCTL0 |= CCIE;

  TB1CCR1 = TB1_SEC_0_040; // 40 msec
  TB1CCTL1 &= ~CCIFG;
  TB1CCTL1 |= CCIE;

  TB1CCR2 = TB1_SEC_0_050;   // 50 msec
  TB1CCTL2 &= ~CCIFG;
  TB1CCTL2 |= CCIE;

  TB1CTL &= ~TBIE ;
  TB1CTL &= ~TBIFG ;

}

//Timer uses: (ACLK, to obtain seconds and greater)
//TB2_CCR0 -
//TB2_CCR1 -
//TB2_CCR2 -
void Init_Timer_B2(void){

  // ACLK -> 32,768 kHz
  TB2CTL = TBSSEL__ACLK;

  TB2CTL |= TBCLR ; // Resets TB2R , clock divider, count direction

  TB2CTL |= MC__CONTINOUS;

  TB2CTL |= ID__8;

  TB2EX0 = TBIDEX__8;

  TB2CCR0 = TB2_MIN_0_005; // Half second
  TB2CCTL0 &= ~CCIFG;
  TB2CCTL0 |= CCIE;

  TB2CCR1 = 512; // one second
  TB2CCTL1 &= ~CCIFG;
  TB2CCTL1 |= CCIE; // explicitly disable by &= ~

  TB2CCR2 = TB2_MIN_1_000;   // Minute
  TB2CCTL2 &= ~CCIFG;
  TB2CCTL2 |= CCIE;

  TB2CTL &= ~TBIE ;
  TB2CTL &= ~TBIFG ;
}

// TimerB3 will be for PWM
// CCR0 will be the wheel period to work with (about 50,000) (slow might be 30K)
void Init_Timer_B3(void){
  //------------------------------------------------------------------------------
  // SMCLK source, up count mode, PWM Right Side
  // TB3.1 P6.0 R_FORWARD
  // TB3.2 P6.1 L_FORWARD
  // TB3.3 P6.2 R_REVERSE
  // TB3.4 P6.3 L_REVERSE
  //------------------------------------------------------------------------------
  TB3CTL = TBSSEL__SMCLK; // SMCLK
  TB3CTL |= MC__UP; // Up Mode
  TB3CTL |= TBCLR; // Clear TAR
  TB3CCR0 = WHEEL_PERIOD; // PWM Period
  TB3CCTL1 = OUTMOD_7; // CCR1 reset/set
  RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM duty cycle
  TB3CCTL2 = OUTMOD_7; // CCR2 reset/set
  LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Left Forward PWM duty cycle
  TB3CCTL3 = OUTMOD_7; // CCR3 reset/set
  RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM duty cycle
  TB3CCTL4 = OUTMOD_7; // CCR4 reset/set
  LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Left Reverse PWM duty cycle
  //------------------------------------------------------------------------------

}//QUESTION: where do the ports actually change?