//------------------------------------------------------------------------------
//
//  Description: file that contains general defines and macros for overall program
//
//
//  Mark Rubianes
//  Jan 2021
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.11.2)
//------------------------------------------------------------------------------

extern volatile int project_5_timer;
extern volatile int project_5_Seconds;

#include "ports.h"

#define ZERO                    (0)
#define TRUE                    (1)
#define FALSE                   (0)
#define ALWAYS                  (1)

#define RESET_STATE             (0)

// Timing and Clock related Defines========================================================================
#define MCLK_FREQ_MHZ 8                     // MCLK = 8MHz
#define CLEAR_REGISTER     (0X0000)
#define RESET_TIME                (0)


//LED macro arguments
#define RED_LED_OFF()      (P1OUT &= ~RED_LED)
#define RED_LED_ON()       (P1OUT |= RED_LED)
#define RED_LED_TOGGLE()   (P1OUT ^= RED_LED)

#define GREEN_LED_OFF()    (P6OUT &= ~GRN_LED)
#define GREEN_LED_ON()     (P6OUT |= GRN_LED)
#define GREEN_LED_TOGGLE() (P6OUT ^= GRN_LED)

#define TIME_SEQ_250    (250)
#define TIME_SEQ_200    (200)
#define TIME_SEQ_150    (150)
#define TIME_SEQ_100    (100)
#define TIME_SEQ_50     (50)

#define LED_FAST_BLINK_DURATION        (5000)

//Motor FET control macro arguments

#define R_FORWARD_ON()   (P6OUT |= R_FORWARD)
#define L_FORWARD_ON()   (P6OUT |= L_FORWARD)
#define R_REVERSE_ON()   (P6OUT |= R_REVERSE)
#define L_REVERSE_ON()   (P6OUT |= L_REVERSE)

#define R_FORWARD_OFF()  (P6OUT &= ~R_FORWARD)
#define L_FORWARD_OFF()  (P6OUT &= ~L_FORWARD)
#define R_REVERSE_OFF()  (P6OUT &= ~R_REVERSE)
#define L_REVERSE_OFF()  (P6OUT &= ~L_REVERSE)

#define ALL_OFF          0
#define LFT_FRD_ON       1
#define RGHT_FRD_ON      2
#define LFT_REV_ON       3
#define RGHT_REV_ON      4


#define STOP            (0x0)
#define GO              (1)

//motion_state states
#define IDLE            (0)
#define WAIT            (1)
#define START           (2)
#define RUN             (3)
#define END             (4)

#define WHEEL_PERIOD    (60)  // 45 last
#define STRAIGHT    (0)
#define CIRCLE      (1)
#define FIGURE_8    (2)
#define TRIANGLE    (3)

//#define T_STRAIGHT_HALF              (60) // 30
//#define T_STRAIGHT_HALF_TRAVEL_DIST  (1)  // 2

#define T_STRAIGHT              (65)  // 35
#define T_STRAIGHT_TRAVEL_DIST  (1)  // 4

#define T_R_RIGHT_TURN          (0)  // 0
#define T_L_RIGHT_TURN          (65)  // 35
#define T_TURN_TRAVEL_DIST      (3) // 5

///////

#define CIRCLE_R_LEFT            (35)
#define CIRCLE_L_LEFT            (3)
#define CIRCLE_R_RIGHT           (3)
#define CIRCLE_L_RIGHT           (35)
#define TWO_CIRCLE_TRAVEL_DIST   (40)
#define ONE_CIRCLE_TRAVEL_DIST   (17)

#define F8_R_HALF_L_TURN          (38)
#define F8_L_HALF_L_TURN          (4)
#define F8_HALF_L_TURN_TRAVEL_DIST (11)


#define F8_HALF_L_TURN_TRAVEL_DIST_2 (13)

#define F8_R_FULL_R_TURN          (6)
#define F8_L_FULL_R_TURN          (38)
#define F8_FULL_R_TURN_TRAVEL_DIST (19)


/* State name suggestions from Carlson
#define NONE ('N')
#define STRAIGHT ('L')
#define CIRCLE ('C')
#define WAIT ('W')
#define START ('S')
#define RUN ('R')
#define END ('E')
*/
#define WAITING_TO_START        (300)
#define WHEEL_COUNT_TIME        (10)  //defined period
#define RIGHT_COUNT_TIME        (7)   // % period ON for right wheel
#define LEFT_COUNT_TIME         (8)  //% period ON for left wheel
#define TRAVEL_DISTANCE         (100)  //how many segments tp run

// Switches #defines
#define SWITCH1_INPUT_STATE() (P4IN & SW1)
#define SWITCH2_INPUT_STATE() (P2IN & SW2)
#define PRESSED (0)
#define RELEASED (1)
#define OKAY (1)
#define NOT_OKAY (0)
#define DEBOUNCE_RESTART (0x0)
#define DEBOUNCE_TIME (0x7FF8) //about 1/4 second derived from units of
                                 // complete instruction cycles
// When configuration is (SMCLK = 8,000,000 / ID = 2 / IDEX = 8 ) ==> 5Khz,
//  or 500,000
#define CCR0_05_MSEC_INTERVAL  (0x09C4) // 500,000/(1 / 5 msec) = d2,500
//use this interval to replace Time_Sequence()
#define CCR0_10_MSEC_INTERVAL  (0x1388) // 500,000/(1 / 10 msec) = d5,000
//use this interval to replace LCD Update
#define CCR0_200_MSEC_INTERVAL  (0x186A0) // 500,000/(1 / 200 msec) = d100,000
//Question: overflow doesn't matter here?

//For HMWK 6
#define CCR0_50_MSEC_INTERVAL  (0x61A8) // d25,000

#define Time_Sequence_Rate (0x61A8) // 50 millisecods
#define S1250 (1250/Time_Sequence_Rate) // 1.25 seconds
#define S1000 (1000/Time_Sequence_Rate) // 1 second
#define S750 (750/Time_Sequence_Rate) // 0.75 seconds
#define S500 (500/Time_Sequence_Rate) // 0.50 seconds
#define S250 (250/Time_Sequence_Rate) // 0.25 seconds

#define TIMERB0_CCR0_VECTOR     (TIMER0_B0_VECTOR)
#define TIMERB1_CCR0_VECTOR     (TIMER0_B1_VECTOR)

//QUESTION: WTF?? WHY???
// Nowhere in the example slides are we using TIMER_B1 !!
// And why use overflow to represent three of them ?!
#define TIMER_B0_0_VECTOR       (TIMER0_B0_VECTOR)
#define TIMER_B0_1_OVFL_VECTOR  (TIMER0_B1_VECTOR)

//define TIMER_B0-1-2OVFL_VECTOR





