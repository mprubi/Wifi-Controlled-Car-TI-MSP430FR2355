//ADC conversions and timers suggestions: take a reading about every 10 mSec,
//and obtain running average every 80 Sec
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"
#include <math.h>

// global variables

volatile char USB_Char_Rx [SMALL_RING_SIZE];
volatile char USB_Char_Tx [SMALL_RING_SIZE];
volatile unsigned int usb_rx_ring_wr; // used in ISR and Main() to place in buffer,
unsigned int usb_rx_ring_rd;
//usb_rx_ring_rd Rx is only actually used in main because you're only reading from main
unsigned int usb_tx_ring_wr;
unsigned int usb_tx_ring_rd;

/* The purpose of this ISR is JUST to use the write signal to PUT data in the buffer,
where we read that same buffer later in main.
*/
#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
  unsigned int temp;
  switch(__even_in_range (UCA1IV, 0x08)){
  case 0: // Vector 0 - no interrupt
    break;
  case 2: // Vector 2 - RXIFG
    temp = usb_rx_ring_wr++; // all the extra temp variable does is remove error where compiler
                              // wants you to resolve two volatiles in ISR
    USB_Char_Rx[temp] = UCA1RXBUF; // RX --> USB_Char_Rx character
    // there is only one buffer register, so this should not appear more than once in program
    if (usb_rx_ring_wr >= (sizeof(USB_Char_Rx))){
      usb_rx_ring_wr = BEGINNING; // Circular buffer back to beginning

    }

/* Note: When CR/LF sequence detected in ring buffer, then start putting each character
in 0th location of process buffer at this point, the first actual character might
tell me that it's junk or not and I can make a decision then and there, then
wait for the next CR/LF sequence from the ring buffer
    */

    break;
  case 4: // Vector 4 - TXIFG
// MY implementation
    //    if(array index ++ != NULL){
//      //USA1TxBUFF = array index[?];
//      return;
//    }else if(array index ++ != NULL){
//      //if current array index is NULL, first disable the tx interrupt, then return
//    }

//    switch(UCA1_index++){
//    case 0: //
//    case 1: //
//    case 2: //
//    case 3: //
//    case 4: //
//    case 5: //
//    case 6: //
//    case 7: //
//    case 8: //
//      UCA1TXBUF = test_command[UCA1_ index];
//      break;
//    case 9: //
//      UCA1TXBUF = 0x0D; // CR - carriage return - return to beginning of the line you're on
//      break;
//    case 10: // Vector 0 no interrupt
//      UCA1TXBUF = 0x0A; // LF - line feed - go to the next line
//      break;
//    default:
//      UCA1IE &= ~UCTXIE; // Disable TX interrupt because we're at the end of the array we want to transmit
//      break;
//    }
//    break;
  default:
    break;
  }
}




