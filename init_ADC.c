#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"
#include <math.h>

void Init_ADC (void){
  //
  // V_DETECT_L (0x04) // Pin 2 A2    ADCINCH_2
  // V_DETECT_R (0x08) // Pin 3 A3    ADCINCH_3
  // V_THUMB (0x20) // Pin 5 A5    ADCINCH_5
  //

  // ADCCTL0 Register
  ADCCTL0 = 0; // Reset
  ADCCTL0 |= ADCSHT_2; // 16 ADC clocks
  ADCCTL0 |= ADCMSC; // MSC
  ADCCTL0 |= ADCON; // ADC ON

  // ADCCTL1 Register
  ADCCTL2 = 0; // Reset
  ADCCTL1 |= ADCSHS_0; // 00b = ADCSC bit
  ADCCTL1 |= ADCSHP; // ADC sample and hold SAMPCON signal from sampling timer.
  ADCCTL1 &= ~ADCISSH; // ADC invert signal sample and hold.
  ADCCTL1 |= ADCDIV_0; // ADC clock divider 000b = Divide by 1
  ADCCTL1 |= ADCSSEL_0; // ADC clock MODCLK
  ADCCTL1 |= ADCCONSEQ_0; // ADC conversion sequence 00b = Single channel single conversion
  // ADCCTL1 & ADCBUSY identifies a conversion is in process

  // ADCCTL2 Register
  ADCCTL2 = 0; // Reset
  ADCCTL2 |= ADCPDIV0; // ADC predivider 00b = Pre divide by 1
  ADCCTL2 |= ADCRES_1; // ADC resolution 10b  // OLD carlson notes: = 12 bit (14 clock cycle conversion time)

  // TODO: change above to 10 bit instead of 12, and remove >>2 in ISR
 //audio lecture 3/11 when do I get a stable reading??

  ADCCTL2 &= ~ADCDF; // ADC data read back format 0b = Binary unsigned.
  ADCCTL2 &= ~ADCSR; // ADC sampling rate 0b = ADC buffer supports up to 200 ksps

  // ADCMCTL0 Register
  ADCMCTL0 |= ADCSREF_0; // VREF 000b = {VR+ = AVCC and VR ––= AVSS
  ADCMCTL0 |= ADCINCH_2; // // start with left detect,  pin 2 A2  // previously V_THUMB (0x20) Pin 5 A5

  //V_DETECT_L (0x04) // Pin 2 A2    ADCINCH_2
  // V_DETECT_R (0x08) // Pin 3 A3    ADCINCH_3
  // Line above is which external input we're reading

  ADCIE |= ADCIE0; // Enable ADC conv complete interrupt
  ADC_CONVERSION_DISABLE();
//  ADCCTL0 |= ADCENC; // ADC enable conversion.
  ADC_CONVERSION_START();
//  ADCCTL0 |= ADCSC; // ADC start conversion.
}