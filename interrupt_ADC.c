//ADC conversions and timers suggestions: take a reading about every 10 mSec,
//and obtain running average every 80 Sec
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"
#include <math.h>

extern volatile unsigned int ADC_Left_Sensor = 0;
extern volatile unsigned int ADC_Right_Sensor = 0;
extern volatile unsigned int ADC_Bat = 0;
extern volatile unsigned int ADC_Thumb = 0;
extern volatile unsigned int ADC_Channel = 0;
extern volatile char adc_char[4] = "";

//A variable defined and only used inside the interrupt tree need not be defined as a volatile.
//32 bit array?
volatile unsigned int Left_Sensor_Mov_Values[8];
volatile unsigned int Right_Sensor_Mov_Values[8];
volatile unsigned int Left_Sensor_Watch[32], Right_Sensor_Watch[32];  //for debug
volatile unsigned int left_sensor_mov_average, right_sensor_mov_average;

extern char display_line[4][11];



#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
  switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
    case ADCIV_NONE:
    break;
    case ADCIV_ADCOVIFG: // When a conversion result is written to the ADCMEM0
    // before its previous conversion result was read.
    break;
    case ADCIV_ADCTOVIFG: // ADC conversion time overflow
    break;
    case ADCIV_ADCHIIFG: // Window comparator interrupt flags
    break;
    case ADCIV_ADCLOIFG: // Window comparator interrupt flag
    break;
    case ADCIV_ADCINIFG: // Window comparator interrupt flag
    break;
    case ADCIV_ADCIFG: // ADCMEM0 memory register with the conversion result
    //this case statement is where code for how and what I want to sample occurs

    ADCCTL0 &= ~ADCENC; //disable the ENC bit to be able to change which port to read next

    switch (ADC_Channel++){  // post incrementing occurs after conditional statement uses original value to evaluate
      case ADC_LEFT_SENSOR: // Channel A3 Interrupt
      ADCMCTL0 &= ~ADCINCH_2; // Disable Last channel A2
      ADCMCTL0 |= ADCINCH_3; // Enable Next channel A3
      ADC_Left_Sensor = ADCMEM0; // Move result into Global
      // Old carlson code: ADC_Left_Sensor = ADC_Left_Detect >> 2; // Divide the result by 4 for 12 bit ADC

      ADC_Left_Sensor -= 6; // correction factor to position car in the center of black line

      //averaging function call goes here
      left_sensor_mov_average = 0;
      ADC_perform_moving_average(ADC_Left_Sensor, ADC_LEFT_SENSOR);

      //calls for hex conversions and updating display are in TB1 interrupt ISR

      ADCCTL0 |= ADCSC; // Start next sample. Kicks off the next ADC_ISR() interrupt

      //TODO: add re-enable ENC, hit the top of the switch, do calculations of next case
      // don't do this on last case, just start a timer, and the timer interrupt re-enables the ENC bit

      break;

      case ADC_RIGHT_SENSOR: // Channel A4 Interrupt
      ADCMCTL0 &= ~ADCINCH_3; // Disable Last channel A3
      ADCMCTL0 |= ADCINCH_4; // Enable Next channel A4
      ADC_Right_Sensor = ADCMEM0; // Move result into Global
      // Old carlson code: ADC_Right_Sensor = ADC_Left_Detect >> 2; // Divide the result by 4 12 bit ADC
      ADC_Right_Sensor += 6; // correction factor to position car in the center of black line

      //averaging function goes here
      right_sensor_mov_average = 0;
      ADC_perform_moving_average(ADC_Right_Sensor, ADC_RIGHT_SENSOR);

      //calls for hex conversions and updating display are in TB1 interrupt ISR

      ADCCTL0 |= ADCSC; // Start next sample. Kicks off the next ADC_ISR() interrupt
      break;

      case ADC_V_BAT: // Channel A5 Interrupt
      ADCMCTL0 &= ~ADCINCH_4; // Disable Last channel A4
      ADCMCTL0 |= ADCINCH_5; // Enable Next channel A5
      ADC_Bat = ADCMEM0; // Move result into Global

      ADCCTL0 |= ADCSC; // Start next sample. Kicks off the next ADC_ISR() interrupt
      break;

      case ADC_V_THUMB: //Channel A2 Interrupt
      ADCMCTL0 &= ~ADCINCH_5; // Disable Last channel A5
      ADCMCTL0 |= ADCINCH_2; // Enable Next channel A2
      ADC_Thumb = ADCMEM0; // Move result into Global


      //ENABLE THE 25 mSec TIMER ON LAST CASE
      // when timer is done, enable the ADC converstions again:
      //              ADCCTL0 |= ADCENC; // Enable Conversions
      //              ADCCTL0 |= ADCSC; // Start next sample. Kicks off the next ADC_ISR() interrupt

      ADC_Channel = 0;  //reset the channel to start from the first in series of desired reads

      ADCCTL0 |= ADCSC; // Start next sample. Kicks off the next ADC_ISR() interrupt
//      ADCCTL0 &= ~ADCSC; // disable until 25 mSec timer re-enables it?????
      break;

      default:
      break;
    }

    //lines below are part of case /* case ADCIV_ADCIFG: // ADCMEM0 memory register with the conversion result  */
    // they execute for every case statement above
    // Enable Conversions //TODO: move this line to 25 mSec Timer

    //disable ISR, leave it to a timer to re-enable 30 mSec later
    ADCCTL0 &= ~ADCENC;

    // Switch case replaces the 2 following lines, which was  repeatedly reading the
    //thumb wheel with no timer involved
    //    ADC_Thumb = ADCMEM0; // Channel A5
    //    ADCCTL0 |= ADCSC; // Start next sample. Kicks off the next ADC_ISR() interrupt


    //TODO: since it will take < 25 mSec to take a reading, you can disable the timer while readings occur
    // the next timer interrupt will kick off the next reading on its own
    break;
    default:
    break;
  }
}

/*helper function for ADC converstion ISR: This is simply moving what would have
  been inside each case statement of the ISR
 takes result from ADC and performs operations on the relevant mov average array
*/
void ADC_perform_moving_average(unsigned int volatile ADC_result, char specific_sensor){
  //TODO: opportunity to implement a queue data structure from scratch
  //TODO: less logic if implemented with pointer to specific array
  unsigned int ADC_result_temp = ADC_result;

  switch(specific_sensor){
  case ADC_LEFT_SENSOR:
    //shift array, add new value, and take the average (sum values and divide by 8)
    for(int i = 7; i >= 0; i--){
      if(i == 0){
        Left_Sensor_Mov_Values[0] = ADC_result_temp;
        left_sensor_mov_average += ADC_result_temp;
      }else{
        Left_Sensor_Mov_Values[i] = Left_Sensor_Mov_Values[i-1];
        left_sensor_mov_average += Left_Sensor_Mov_Values[i];
      }
    }
    left_sensor_mov_average >>= 3;
    break;
  case ADC_RIGHT_SENSOR:
    for(int i = 7; i >= 0; i--){
      if(i == 0){
        Right_Sensor_Mov_Values[0] = ADC_result_temp;
        right_sensor_mov_average += ADC_result_temp;
      }else{
        Right_Sensor_Mov_Values[i] = Right_Sensor_Mov_Values[i-1];
        right_sensor_mov_average += Right_Sensor_Mov_Values[i];
      }
    }
    right_sensor_mov_average >>= 3;
    break;
  }

  //debug / watch array
  for(int i = 31; i >= 0; i--){
    if(i == 0){
      Left_Sensor_Watch[0] = left_sensor_mov_average;
      Right_Sensor_Watch[0] = right_sensor_mov_average;
    }else{
      Left_Sensor_Watch[i] = Left_Sensor_Watch[i-1];
      Right_Sensor_Watch[i] = Right_Sensor_Watch[i-1];
    }
  }
}


/*helper function for conversion to characters for display:

*/
void HEXtoBCD(int hex_value){
  int divisor = 1;

  for(int i=0; i<4; i++){
//    = ascii offset plus each decimal digit of the hex value
//    example: 1123
//    1234 % 10 = 4, 4/1 = 4 -> one's place = 4
//    1234 % 100 = 34, 34/10 = 3 -> one's place = 3
//    1234 % 1000 = 234, 234/100 = 2 -> one's place = 2
//    1234 % 10000 = 1234, 1234/1000 = 1 -> one's place = 1

    adc_char[i] = 0x30 + (hex_value % (divisor*10) / (divisor) );
    divisor = divisor * 10;
  }
}

void adc_line(char display_line_position, char display_character_position){

  for(char i=3, j=display_character_position; j < (display_character_position+4); i--, j++){
    //note: pointless comparison warning broke refresh of entire display
    display_line[display_line_position][j] = adc_char[i];
  }
  display_line[display_line_position][4] = ' '; //fill the next element with a space
  display_line[display_line_position][5] = ' '; //fill the next element with a space
}

/* Carlson's bloated function for hex -> BCD conversion
void HEXtoBCD(int hex_value){

  if(hex_value >= 1000){
    hex_value = hex_value - 1000;
    thousands = 1;
  }

  adc_char[0] = '0';
  while (hex_value > 999){
    hex_value = hex_value - 1000;
    value = value + 1;
    adc_char[0] = 0x30 + value;
    hundreds_place = 1;
  }
  if((adc_char[0] == '1') && !hundreds_place){
    adc_char[1] = '0';
  }
  value = 0;
  while (hex_value > 99){
    hex_value = hex_value - 100;
    value = value + 1;
    adc_char[1] = 0x30 + value;
  }
  value = 0;
  while (hex_value > 9){
    hex_value = hex_value - 10;
    value = value + 1;
    adc_char[2] = 0x30 + value;
  }
  adc_char[3] = 0x30 + hex_value;
}
*/