// New code: from "hints from project 3a

//------------------------------------------------------------------------------
//
//  Description: This file contains functions definitions for hardware switches on MSP430
//
//
//  Mark Rubianes
//  Jan 2021
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.11.2)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
      //NOT ADDED TO PROJECT YET. ADD TO PROJECT, THEN DELETE .R43 FILE!!!
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"

char okay_to_look_at_switch1 = OKAY;
char okay_to_look_at_switch2 = OKAY;
char sw1_position = RELEASED;
char sw2_position = RELEASED;
unsigned int count_debounce_SW1 = DEBOUNCE_TIME;
unsigned int count_debounce_SW2 = DEBOUNCE_TIME;

/*
count_debounce_SW1 and count_debounce_SW2 where defined as chars,
error: Warning[Pa084]: pointless integer comparison, the result is always true

I was comparing a char (1 byte) against an int (2 bytes)

Debouncing code: debouncing is managing the hundreds of extra contacts that
are made when a mechanical switch is pressed because the clock is running at
the speed of thousands of times a second (Mhz range). This code is the software
way of handling this problem, although it uses up clock cycles. A better solution
is through hardware debouncing and filtering the contacts so it's a smooth
transition.

*/



// Using interrupts

/*Starting Template
#
pragma vector = {Vector}
// do not forget the header
__interrupt
void my_isr (void)
// do whatever must be done
}

Using  #pragma vector = {vector_name}
Tells the compiler it’s an ISR. This is important  because
the ISR has a different stack frame compared with subroutine. This means
it 1) Saves all registers and Flag register is also saved, and 3)
Different return needed (REIT vs. EXITD)


Latency is six cycles starting with the acceptance of an interrupt request
and lasting until the start of execution of the first instruction of the
service routine: GIE (Global Interrupt Enable) bit is set/cleared here.

RETI (Return from Interrupt) takes five cycles

Due to the pipelined CPU architecture, the instruction following
the enable interrupt instruction ( is always executed, even if
an interrupt service request is pending when the interrupts are
enabled
•
If the enable interrupt instruction ( is immediately followed
by a disable interrupt instruction ( a pending interrupt
might not be serviced Further instructions after DINT might
execute incorrectly and result in unexpected CPU execution It is
recommended to always insert at least one instruction between
EINT and DINT Note that any alternative instruction use that sets
and immediately clears the CPU status register GIE bit must be
considered in the same fashion


Configure MCU:
Enable interrupts for system  ?????

*/

/*
init timer b0 is only global setup needed for timers
lcd won't work if DIV from hmwk 5 is still in place
*/

#pragma vector = PORT4_VECTOR
__interrupt void switchP4_interrupt(void){
  // Switch 1
  if (P4IFG & SW1) {
    /*
          1. Disable Switch 1
          2. Clear Switch 1 Flag  (only clears current interrup, but pending one will be irrelevant by the time we're done debouncing anyway)
          3. Clear TimerB 0 Interrupt Flag for Capture Compare Register 1
          4. Add Interval to TB 0 R for TB 0 CCR 1 (implying that the using CCR1 is suggested to use for debouncing SW1, and CCR2 for SW2)
          5. Enable TimerB 0 1
    */

// (IFregister) 1 bit that will get masked in and out

//clearing faults means clearing pending flags

    // CCIE:

    // Sepparate Question from HWK 6: what is interrupt_ports.c suposed to be for???

    //Question: What does disable Switch 1 interrupt mean? This below??
    P4IE &= ~SW1;  // 1) "Interrupt Enable" bit
    P4IFG &= ~SW1; // 2) IFG SW1 cleared


    //P6OUT &= ~LCD_BACKLITE;  //this line was just testing to see if the interrupt worked at all

    TB0CCTL1 &= ~CCIFG; // 3) clear any possible pending interrupt flags
    TB0CCR1 += CCR0_200_MSEC_INTERVAL; // 4) Note: needs to be for 1 second for safe debounce

    TB0CCR1 = TB0R

    TB0CCTL1 |= CCIE; // CCR1 enable interrupt

    //Question: "at some time later, the timer's gunna happen. What do you do in the timer? You do the reverse.
    // you disable the timer, you clear your flags (both in your switch and timer), and
    // then you enable your switch interrupt"
  }
}
#pragma vector = PORT2_VECTOR
__interrupt void switchP2_interrupt(void){
  // Switch 2
  if (P2IFG & SW2) {
    P2IFG &= ~SW2; // IFG SW2 cleared
    P6OUT |= LCD_BACKLITE;
  }
}

/////// OLD SWITCh PROCESS CODE ///////////////////////////////////////////////////////////////
/*

void Switches_Process(void) {
//
// This function calls the individual Switch Functions
//
  Switch1_Process();
  Switch2_Process();
}
//

void Switch1_Process(void){
//
// Switch 1 Configurations
//
  if(okay_to_look_at_switch1 && sw1_position){
    if(!(SWITCH1_INPUT_STATE())){  // !(P4IN & SW1) means switch is LOW // & here is bitwise
      //AND (if T, switch is high, default state)

      // P4IN & SW1 is how you check the in state of switch 1, the P4IN
      //special purpose register is multiplexed with the pin SW1
      sw1_position = PRESSED;
      okay_to_look_at_switch1 = NOT_OKAY; // not okay until debounce count down
      count_debounce_SW1 = DEBOUNCE_RESTART; //reset here, switch hit for first time
      //my button press code
      motion_state_select();

      //fet_test_state_select();

      //all_motors_OFF();
      //reverse_ON();

    }
  }
// this is a sepparate if statement
  if(count_debounce_SW1 < DEBOUNCE_TIME){
    //RED_LED_TOGGLE();  uncomment to visualize length of debounce period
    count_debounce_SW1++;
  }else{  //enough time has incremented to be safe to call switch released
    okay_to_look_at_switch1 = OKAY; //flags switch as okay to look at
    if(SWITCH1_INPUT_STATE()){
      sw1_position = RELEASED;
    }
 }
}
  //

void Switch2_Process(void){
  //
  // Switch 2 Configurations
  //
  if(okay_to_look_at_switch2 && sw2_position){
    if(!(SWITCH2_INPUT_STATE())){
      sw2_position = PRESSED;
      okay_to_look_at_switch2 = NOT_OKAY;
      count_debounce_SW2 = DEBOUNCE_RESTART;

      //my button press code not implemented yet
      GREEN_LED_OFF();
      RED_LED_TOGGLE();
      //stop();
      //shape_state_select();

      //fet_test_activate();

      //all_motors_OFF();
      //reverse_OFF();
    }
  }
  if(count_debounce_SW2 < DEBOUNCE_TIME){
    //GREEN_LED_TOGGLE();  uncomment to visualize length of debounce period
    count_debounce_SW2++;
  }else{
    okay_to_look_at_switch2 = OKAY;
    if(SWITCH2_INPUT_STATE()){
      sw2_position = RELEASED;
    }
  }
}
//

*/