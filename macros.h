//------------------------------------------------------------------------------
//
//  Description: file that contains general defines and macros for overall program
//
//
//  Mark Rubianes
//  Jan 2021
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.11.2)
//------------------------------------------------------------------------------


#include "ports.h"

#define ZERO                    (0)
#define TRUE                    (1)
#define FALSE                   (0)
#define ALWAYS                  (1)
#define OFF                    (0)

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

#define BACKLIGHT_TOGGLE() (P6DIR ^= LCD_BACKLITE)

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

// When configuration is (SMCLK = 8,000,000 / ID = 2 / IDEX = 8 ) ==> 5Khz,
//  or 500,000
#define CCR0_05_MSEC_INTERVAL  (0x09C4) // 500,000/(1 / 5 msec) = d2,500
//use this interval to replace Time_Sequence()
#define CCR0_10_MSEC_INTERVAL  (0x1388) // 500,000/(1 / 10 msec) = d5,000
//use this interval to replace LCD Update
#define CCR0_200_MSEC_INTERVAL  (0x186A0) // 500,000/(1 / 200 msec) = d100,000
//Question: warning integer conversion resulted in truncation // is this even achieving 200 MSEC?
#define CCR0_16_MSEC_INTERVAL  (0x1F40) // 500,000/(1 / 16 msec) = d100,000

//TB1 SMCLK (div 8, div 8) intervals

#define TB1_SEC_0_025 (TB1_SEC_0_050/2)  // 25 msec for TB1 CCR1
#define TB1_SEC_0_030 (3750)  // 30 msec for TB1 CCR1
#define TB1_SEC_0_040 (5000)  // 40 msec for TB1 CCR1
#define TB1_SEC_0_050 (6250)
#define TB1_SEC_0_200 (TB1_SEC_0_050*4) // 25,000

//TB2 ALCK (div 8, div 8) intervals = 512/(1/t_sec)
#define TB2_MIN_1_000 (30720)  // 60 seconds
#define TB2_MIN_0_100 (TB2_MIN_1_000/6)  // = 5120 (ten seconds)
#define TB2_MIN_0_010 (TB2_MIN_1_000/60)  // = 512 (one second)
#define TB2_MIN_0_005 (TB2_MIN_1_000/120)  // = 256 (one half second)

//For HMWK 6
#define CCR0_50_MSEC_INTERVAL  (0x61A8) // d25,000

//TB0 - > SMCLK = 8,000,000 / ID = 2 / IDEX = 8 ) ==> 5Khz
// SECOND_3SIGFIGS
#define TB0_SEC_0_050 (0x61A8) // 50 milliseconds // 0x61A8 == 25,000
#define TB0_SEC_0_100 (0xC350) // 100 milliseconds

//these line below are wrong
//#define TB0_SEC_1_000 (1000/TB0_SEC_0_050) // 1 second
//#define TB0_SEC_0_750 (750/TB0_SEC_0_050) // 0.75 seconds
//#define TB0_SEC_0_500 (500/TB0_SEC_0_050) // 0.50 seconds
//#define TB0_SEC_0_250 (250/TB0_SEC_0_050) // 0.25 seconds

#define TIMER_B0_CCR_0_VECTOR           (TIMER0_B0_VECTOR)
#define TIMER_B0_CCR_1_2_OVFL_VECTOR    (TIMER0_B1_VECTOR)
#define TIMER_B1_CCR_0_VECTOR           (TIMER1_B0_VECTOR)
#define TIMER_B1_CCR_1_2_OVFL_VECTOR    (TIMER1_B1_VECTOR)
#define TIMER_B2_CCR_0_VECTOR           (TIMER2_B0_VECTOR)
#define TIMER_B2_CCR_1_2_OVFL_VECTOR    (TIMER2_B1_VECTOR)
#define TIMER_B3_CCR_0_VECTOR           (TIMER3_B0_VECTOR)
#define TIMER_B3_CCR_1_7_OVFL_VECTOR    (TIMER3_B1_VECTOR)
//TI's naming scheme is confusing: TIMER0_B1_VECTOR is still part of TimerB0,
// "TIMER0" denotes "TIMERB0". The "_B1" refers to the second interupt vector

//PWM
#define WHEEL_PERIOD    (50000)
#define WHEEL_OFF       (0)
#define SLOW            (12000)
#define PIVOT          (19000)
#define MEDIUM          (23333)
#define FAST            (36666)
#define TURBO           (49999)
#define SPEED_DIVISION  (13333)
#define HARD_TURN  (6666)
#define MEDIUM_TURN  (3333)
#define SOFT_TURN  (1666)
#define DEBUG_ARR_SIZE (16) //previously working with 64

#define STRAIGHT                 (0)
#define SOFT_LEFT_TURN           (1)
#define SOFT_RIGHT_TURN          (2)
#define MEDIUM_LEFT_TURN         (3)
#define MEDIUM_RIGHT_TURN        (4)
#define HARD_LEFT_TURN           (5)
#define HARD_RIGHT_TURN          (6)

#define RIGHT_FORWARD_SPEED (TB3CCR1)
#define LEFT_FORWARD_SPEED (TB3CCR2)
#define RIGHT_REVERSE_SPEED (TB3CCR3)
#define LEFT_REVERSE_SPEED (TB3CCR4)

#define LEFT_PIVOT (0)
#define RIGHT_PIVOT (1)

//ADC

#define ADC_LEFT_SENSOR         (0x00)
#define ADC_RIGHT_SENSOR        (0x01)
#define ADC_V_BAT               (0x02)
#define ADC_V_THUMB             (0x03)
#define ADC_CONVERSION_ENABLE()   (ADCCTL0 |= ADCENC)
#define ADC_CONVERSION_DISABLE()  (ADCCTL0 &= ~ADCENC)
#define ADC_CONVERSION_START()    (ADCCTL0 |= ADCSC)
#define IR_LED_OFF()              (P2OUT &= ~IR_LED)
#define IR_LED_ON()               (P2OUT |= IR_LED)
#define IR_LED_TOGGLE()           (P2OUT ^= IR_LED)

//menu display
#define DISPLAY_DEFAULT (0)
#define DISPLAY_MENU_DIRECTION (2)
#define DISPLAY_MENU_SPEED (3)
#define DISPLAY_MENU_BACKLIGHT (4)
#define DISPLAY_MENU_SERIAL_COMS (5)
#define DISPLAY_MENU_DIRECTION_FORWARD (6)
#define DISPLAY_MENU_DIRECTION_STOP (7)
#define DISPLAY_MENU_DIRECTION_REVERSE (8)
#define DISPLAY_MENU_DIRECTION_PID (9)
#define DISPLAY_MENU_SPEED_SLOW (10)
#define DISPLAY_MENU_SPEED_MEDIUM (11)
#define DISPLAY_MENU_SPEED_FAST (12)
#define DISPLAY_MENU_SPEED_TURBO (13)
#define DISPLAY_MENU_SERIAL_COMS_BAUD_SEL (14)
#define DISPLAY_MENU_SERIAL_COMS_UCA_SEL  (15)
#define DISPLAY_MENU_SERIAL_COMS_TX_MESSAGE  (16)

#define DISPLAY_MENU_LINE_1             (" Control  ")
#define DISPLAY_MENU_LINE_1_ARROW       (">Control  ")
#define DISPLAY_MENU_LINE_2             (" Speed    ")
#define DISPLAY_MENU_LINE_2_ARROW       (">Speed    ")
#define DISPLAY_MENU_LINE_3             (" Bcklight ") //" Bcklight"
#define DISPLAY_MENU_LINE_3_ARROW       (">Bcklight ") //">Bcklight"
#define DISPLAY_MENU_LINE_4             (" SerialCom")
#define DISPLAY_MENU_LINE_4_ARROW       (">SerialCom")

#define DISPLAY_MENU_SERIAL_LINE_1        (" Baud Rate")
#define DISPLAY_MENU_SERIAL_LINE_1_ARROW  (">Baud Rate")
#define DISPLAY_MENU_SERIAL_LINE_2        (" IoT/USB  ")
#define DISPLAY_MENU_SERIAL_LINE_2_ARROW  (">IoT/USB  ")
#define DISPLAY_MENU_SERIAL_LINE_3        (" Tx Next M")
#define DISPLAY_MENU_SERIAL_LINE_3_ARROW  (">Tx Next M")
#define DISPLAY_MENU_SERIAL_LINE_4        ("          ")
#define DISPLAY_MENU_SERIAL_LINE_4_ARROW  ("          ")

#define MENU_SEL_IoT (0)
#define MENU_SEL_USB (1)

#define DEFAULT_MENU_SERIAL (0)
#define DEFAULT_MENU_ADC (1)

//PID Control
#define LINE_CENTER     (650)
#define LINE_CENTER_THRESH     (630)
#define LINE_EDGE       (350)
#define OFF_LINE        (0)
#define ON_LINE         (1)
#define LEFT_OF_LINE    (2)
#define RIGHT_OF_LINE   (3)

//Serial
#define RESET (0)
#define STILL_PROCESSING (1)
#define SMALL_RING_SIZE (16)
#define LARGE_RING_SIZE (25)
#define LARGE_RING_SIZE_32 (32)
#define VERY_LARGE_RING_SIZE (128)
#define BAUD_9600 (1)
#define BAUD_115200 (2)
#define BAUD_460800 (3)
#define BRCWR_9600 (52)
#define MCWR_9600 (0x4911)
#define BRCWR_115200 (4)
#define MCWR_115200_1 (0xAA51)
#define MCWR_115200_2 (0x5551)
#define MCWR_115200_3 (0x6B51)

#define STILL_PROCESSING (1)
#define NOT_PROCESSING (0)
#define WORD_REGISTER   (0)
#define NULL_CHAR        (0x00)
#define LINE_FEED       ('\n')
#define CARRIAGE_RETURN ('\r')
#define P_BUFF_1 (0)
#define P_BUFF_2 (1)
#define P_BUFF_3 (2)

#define NUM_BUFFERS (2)
#define P_BUFF_SIZE_32 (32)
#define P_BUFF_SIZE_128 (128)


#define UCA1_TX (0)
#define UCA0_TX (1)



/* deriving timer values

INTERVAL = Clock / Input Divider [ID] / Input Divider Expansion [T?xEX0] / ( 1 / Desired Time )

Clock = 8,000,000 [SMCLK]

Input Divider [ID] = 1,2,4,8 [this example 2]

Input Divider Expansion [T?xEX0] = 1,2,3,4,5,6,7,8 [this example 8]

Desired INTERVAL =

                = 8,000,000 / 2 / 8 / ( 1 / 5msec )  // 1/(5/1000)

                = 8,000,000 / 2 / 8 / 200

                = 8,000,000 / 2 / 8 / 200

                = 4,000,000 / 8 / 200

                = 500,000 / 200

                = 2,500   == 0x09C4

Example for Timer A0 Capture Compare Register 0 for desired interrupt every 5 msec.
//#define TB0CCR0 _INTERVAL (2500) // 8,000,000 / 2 / 8 / ( 1 / 5msec )

*/




