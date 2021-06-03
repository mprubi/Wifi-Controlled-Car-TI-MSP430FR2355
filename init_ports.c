//------------------------------------------------------------------------------
//
//  Description: Source file that contains function definitions for
//                     initializing ports of MSP4300
//
//  Mark Rubianes
//  Jan 2021
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.11.2)
//------------------------------------------------------------------------------

#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"

void Init_Ports(void){  // Initialize Ports
  init_port_6();
  init_port_1();
  init_port_2();
  init_port_3();
  init_port_4();
  init_port_5();
}

void init_port_1(void){
  // Port 1 Pins
  // RED_LED    (0x01) // 0 RED LED 0
  // A1_SEEED   (0x02) // 1 A1_SEEED
  // V_DETECT_L (0x04) // 2 V_DETECT_L
  // V_DETECT_R (0x08) // 3 V_DETECT_R
  // A4_SEEED   (0x10) // 4 A4_SEEED
  // V_THUMB    (0x20) // 5 V_THUMB
  // UCA0RXD    (0x40) // 6 Back Channel UCA0RXD
  // UCA0TXD    (0x80) // 7 Back Channel UCA0TXD

  P1OUT = 0x00; // P1 set Low
  P1DIR = 0x00; // Set P1 direction to input

  //Pin 0
  P1SEL0 &= ~RED_LED; // P1_0 GPIO operation
  P1SEL1 &= ~RED_LED; // P1_0 GPIO operation
  P1OUT &= ~RED_LED; // Initial Value = Low / Off
  P1DIR |= RED_LED; // Direction = Output

  //Pin 1
//  P1SEL0 |= A1_SEEED; //
//  P1SEL1 |= A1_SEEED; //
  P1SELC |= A1_SEEED; //

  //Pin 2
//  P1SEL0 |= V_DETECT_L; //
//  P1SEL1 |= V_DETECT_L; //
  P1SELC |= V_DETECT_L; //

  //Pin 3
//  P1SEL0 |= V_DETECT_R; //
//  P1SEL1 |= V_DETECT_R; //
  P1SELC |= V_DETECT_R; //

  //Pin4
//  P1SEL0 |= A4_SEEED; //
//  P1SEL1 |= A4_SEEED; //
  P1SELC |= A4_SEEED; //

  //Pin 5
//  P1SEL0 |= V_THUMB; //
//  P1SEL1 |= V_THUMB; //
  P1SELC |= V_THUMB; //

  //Pin 6
  P1SEL0 |= UCA0RXD; // for serial communications to IoT module
  P1SEL1 &= ~UCA0RXD; // for serial communications to IoT module

  //Pin 7
  P1SEL0 |= UCA0TXD; //  for serial communications to IoT module
  P1SEL1 &= ~UCA0TXD; //  for serial communications to IoT module

}

void init_port_2(void){
   // Port 2 Pins
  // P2_0         (0x01) // 0
  // IR_LED       (0x02) // 1 IR LED on/off signal
  // P2_2         (0x04) // 2
  // SW2          (0x08) // 3 SW2
  // IOT_PROG_SEL (0x10) // 4 IOT_PROG_SEL
  // P2_5         (0x20) // 5
  // LFXOUT       (0x40) // 6 XOUTR
  // LFXIN        (0x80) // 7 XINR

  P2OUT = 0x00; // P2 set Low
  P2DIR = 0x00; // Set P2 direction to input

  //0
  P2SEL0 &= ~P2_0; // P2_0 GPIO operation
  P2SEL1 &= ~P2_0; // P2_0 GPIO operation
  P2DIR &= ~P2_0; // Direction = input

  //1
  P2SEL0 &= ~IR_LED; // IR_LED GPIO operation
  P2SEL1 &= ~IR_LED; // IR_LED GPIO operation
  P2OUT &= ~IR_LED; // Initial Value = Low / Off
  P2DIR |= IR_LED; // Direction = output

  //2
  P2SEL0 &= ~P2_2; // P2_2 GPIO operation
  P2SEL1 &= ~P2_2; // P2_2 GPIO operation
  P2DIR &= ~P2_2; // Direction = input

  //3
  P2SEL0 &= ~SW2; // SW2 Operation
  P2SEL1 &= ~SW2; // SW2 Operation
  P2OUT |= SW2 ; // Configure pullup resistor
  P2DIR &= ~SW2; // Direction = input
  P2REN |= SW2 ; // Enable pullup resistor
  P2IES |= SW2 ; // P2.0 Hi/Lo edge interrupt
  P2IFG &= ~SW2; // Clear all P2.6 interrupt flags
  P2IE |= SW2 ; // P2.6 interrupt enabled

  //4
  P2SEL0 &= ~IOT_PROG_SEL; // IOT_PROG_SEL GPIO operation
  P2SEL1 &= ~IOT_PROG_SEL; // IOT_PROG_SEL GPIO operation
  P2OUT &= ~IOT_PROG_SEL; // Initial Value = Low / Off
  P2DIR &= ~IOT_PROG_SEL; // Direction = input

  //5
  P2SEL0 &= ~P2_5; // P2_5 GPIO operation
  P2SEL1 &= ~P2_5; // P2_5 GPIO operation
  P2DIR &= ~P2_5; // Direction = input

  //6
  P2SEL0 &= ~LFXOUT; // LFXOUT Clock operation
  P2SEL1 |= LFXOUT; // LFXOUT Clock operation

  //7
  P2SEL0 &= ~LFXIN; // LFXIN Clock operation
  P2SEL1 |= LFXIN; // LFXIN Clock operation

}

void init_port_3(void){

  // Port 3 Pins
  // TEST_PROBE   (0x01) // 0 TEST PROBE
  // OPT_INT      (0x02) // 1 OPT_INT
  // OA2N         (0x04) // 2 Photodiode Circuit
  // OA2P         (0x08) // 3 Photodiode Circuit
  // SMCLK_OUT    (0x10) // 4 SMCLK
  // DAC_CNTL     (0x20) // 5 DAC signal from Processor
  // IOT_LINK     (0x40) // 6 IOT_LINK
  // IOT_RESET    (0x80) // 7 IOT_RESET

  P3OUT = 0x00; // p3 set Low
  P3DIR = 0x00; // Set p3 direction to input

  //0
  P3SEL0 &= ~TEST_PROBE; //
  P3SEL1 |= TEST_PROBE; //
  P3DIR &= ~TEST_PROBE; // Direction = input

  //1
  P3SEL0 &= ~OPT_INT; // IR_LED GPIO operation
  P3SEL1 &= ~OPT_INT; // IR_LED GPIO operation
  P3OUT &= ~OPT_INT; // GPIO Input note: not sure

  //2
  P3SEL0 |= OA2N; //
  P3SEL1 |= OA2N; //

  //3
  P3SEL0 |= OA2P; //
  P3SEL1 |= OA2P; //

  //4   GPIO ( SEL0=0, SEL1=1 ) , SMCLK ( SEL0=1, SEL1=0 ) Lecture 7 Slides are not correct SEL bits fpr SMCLK
  P3SEL0 |= SMCLK_OUT; //
  P3SEL1 &= ~SMCLK_OUT; //
  P3DIR |= SMCLK_OUT;  //

  //5
  P3SEL0 |= DAC_CNTL; //
  P3SEL1 |= DAC_CNTL; //

  //6
  P3SEL0 &= ~IOT_LINK; //
  P3SEL1 &= ~IOT_LINK; //
  P3DIR &= ~IOT_LINK;

  //7
  P3SEL0 &= ~IOT_RESET; // CHANGED
  P3SEL1 &= ~IOT_RESET; //
  P3OUT &= ~IOT_RESET; // active low (start out active for at least 100 ms)
  P3DIR |= IOT_RESET; // INPUT

//  P3DIR |= IOT_RESET; // OUTPUT
//  Set as Output Active low for 50ms with pull up to 3.3VDC.
}

void init_port_4(void){
  // RESET_LCD   (0x01) // 0 LCD Reset
  // SW1         (0x02) // 1 SW1
  // UCA1RXD     (0x04) // 2 Back Channel UCA1RXD
  // UCA1TXD     (0x08) // 3 Back Channel UCA1TXD
  // UCB1_CS_LCD (0x10) // 4 Chip Select
  // UCB1CLK     (0x20) // 5 SPI modeclock output UCB1CLK
  // UCB1SIMO    (0x40) // 6 UCB1SIMO
  // UCB1SOMI    (0x80) // 7 UCB1SOMI

  P4OUT = 0x00; // P4 set Low
  P4DIR = 0x00; // Set P4 direction to input

  // P4 PIN 0
  P4SEL0 &= ~RESET_LCD; // RESET_LCD GPIO operation
  P4SEL1 &= ~RESET_LCD; // RESET_LCD GPIO operation
  P4OUT &= ~RESET_LCD; // Set RESET_LCD On [Low]
  P4DIR |= RESET_LCD; // Set RESET_LCD direction to output

  // P4 PIN 1
  P4SEL0 &= ~SW1; // SW1 GPIO operation
  P4SEL1 &= ~SW1; // SW1 GPIO operation
  P4DIR &= ~SW1; // Direction = input
  P4OUT |= SW1; // Configure pullup resistor
  P4REN |= SW1 ; // Enable pullup resistor
  P4IES |= SW1;
  // P2.0 Hi/Lo edge interrupt
  P4IFG &= ~SW1;
  // Clear all P2.6 interrupt flags
  P4IE |= SW1;
  // P2.6 interrupt enabled

  // P4 PIN 2
  P4SEL0 |= UCA1TXD; // USCI_A1 UART operation
  P4SEL1 &= ~UCA1TXD; // USCI_A1 UART operation

  // P4 PIN 3
  P4SEL0 |= UCA1RXD; // USCI_A1 UART operation
  P4SEL1 &= ~UCA1RXD; // USCI_A1 UART operation

  // P4 PIN 4
  P4SEL0 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
  P4SEL1 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
  P4OUT |= UCB1_CS_LCD; // Set SPI_CS_LCD Off [High]
  P4DIR |= UCB1_CS_LCD; // Set SPI_CS_LCD direction to output

  // P4 PIN 5
  P4SEL0 |= UCB1CLK ; // UCB1CLK SPI BUS operation
  P4SEL1 &= ~UCB1CLK; // UCB1CLK SPI BUS operation

  // P4 PIN 6
  P4SEL0 |= UCB1SIMO ; // UCB1SIMO SPI BUS operation
  P4SEL1 &= ~UCB1SIMO; // UCB1SIMO SPI BUS operation

  // P4 PIN 7
  P4SEL0 |= UCB1SOMI ; // UCB1SOMI SPI BUS operation
  P4SEL1 &= ~UCB1SOMI; // UCB1SOMI SPI BUS operation

}

void init_port_5(void){
// CHECK_BAT     (0x01) // 0
// V_BAT         (0x02) // 1
// V_DAC         (0x04) // 2
// V_3_3         (0x08) // 3
// IOT_PROG_MODE (0x10) // 4

  P5OUT = 0x00; // P2 set Low
  P5DIR = 0x00; // Set P5 direction to input

  //0
  P5SEL0 &= ~CHECK_BAT; //  Note: GPIO for manually checking
  P5SEL1 &= ~CHECK_BAT; //
  P5DIR &= ~CHECK_BAT; //

  //1
  P5SEL0 |= V_BAT; // ADC configuration
  P5SEL1 |= V_BAT; // ADC configuration

  //2
  P5SEL0 |= V_DAC; //
  P5SEL1 |= V_DAC; //

  //3
  P5SEL0 |= V_3_3; //
  P5SEL1 |= V_3_3; //

  //4
  P5SEL0 &= ~IOT_PROG_MODE; // IOT_PROGRAM_MODE
  P5SEL1 &= ~IOT_PROG_MODE; //
  P5OUT &= ~IOT_PROG_MODE; // Initial Value = Low / Off
  P5DIR &= ~IOT_PROG_MODE; // Input

  //5 not on schematic, not used, GPIO INPUT
  P5SEL0 &= ~P5_5;
  P5SEL1 &= ~P5_5;
  P5DIR &= ~P5_5;

  //6 not on schematic, not used, GPIO INPUT
  P5SEL0 &= ~P5_6;
  P5SEL1 &= ~P5_6;
  P5DIR &= ~P5_6;

  //7 not on schematic, not used, GPIO INPUT
  P5SEL0 &= ~P5_7;
  P5SEL1 &= ~P5_7;
  P5DIR &= ~P5_7;

}

void init_port_6(void){ //ALL PINS ARE GPIO OUT
// R_FORWARD     (0x01) // 0
// L_FORWARD     (0x02) // 1
// R_REVERSE     (0x04) // 2
// L_REVERSE     (0x08) // 3
// LCD_BACKLITE  (0x10) // 4
// P6_5          (0x20) // 5
// GRN_LED       (0x40) // 6

  P6OUT = 0x00; // P6 set Low
  P6DIR = 0x00; // Set P6 direction to input

  //These are TB3 CCR_0-3
  RIGHT_FORWARD_SPEED = OFF;
  LEFT_FORWARD_SPEED = OFF;
  RIGHT_REVERSE_SPEED = OFF;
  LEFT_REVERSE_SPEED = OFF;

  // Configured as TimerB3.1
  P6SEL0 |= R_FORWARD; //
  P6SEL1 &= ~R_FORWARD; //
  //P6OUT &= ~R_FORWARD; //
  P6DIR |= R_FORWARD; //

  // TimerB3.2
  P6SEL0 |= L_FORWARD; //
  P6SEL1 &= ~L_FORWARD; //
  //P6OUT &= ~L_FORWARD; //
  P6DIR |= L_FORWARD; //

  // TimerB3.3
  P6SEL0 |= ~R_REVERSE; //
  P6SEL1 &= ~R_REVERSE; //
  //P6OUT &= ~R_REVERSE; //
  P6DIR |= R_REVERSE; //

  // TimerB3.4
  P6SEL0 |= ~L_REVERSE; //
  P6SEL1 &= ~L_REVERSE; //
  //P6OUT &= ~L_REVERSE; //
  P6DIR |= L_REVERSE; //

  //4
  P6SEL0 &= ~LCD_BACKLITE; //
  P6SEL1 &= ~LCD_BACKLITE; //
  P6OUT |= LCD_BACKLITE; //
  P6DIR |= LCD_BACKLITE;  // LCD backlight is an output

  //5
  P6SEL0 &= ~P6_5; //
  P6SEL1 &= ~P6_5; //
  P6DIR &= ~P6_5;

  //6  Question: this was undefined but has been compiling, not sure why
  P6SEL0 &= ~GRN_LED; //
  P6SEL1 &= ~GRN_LED; //
  P6OUT &= ~GRN_LED; // Keep LED low until needed
  P6DIR |= GRN_LED; //LED is an output

  //7 not on schematic, not used, GPIO INPUT
  P6SEL0 &= ~P6_7; //
  P6SEL1 &= ~P6_7; //
  P6DIR &= ~P6_7; //

}