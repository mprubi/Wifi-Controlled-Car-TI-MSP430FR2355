//ADC conversions and timers suggestions: take a reading about every 10 mSec,
//and obtain running average every 80 Sec
#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"
#include <math.h>

// global variables
//TODO: to maximize memory usage consider utilizing 1 byte for 4 different bools, and bit shift to access each value
extern volatile char USB_Rx_Ring_Buff [LARGE_RING_SIZE_32],
                     IoT_Rx_Ring_Buff [VERY_LARGE_RING_SIZE];
extern char USB_Tx_Outgoing_Buff[LARGE_RING_SIZE_32],
            IoT_Tx_Outgoing_Buff[LARGE_RING_SIZE_32];
volatile char UCA1_index = 0,
             UCA0_index = 0,
             wr_wrap_around_rd = 0,
             wraparound_check_start = FALSE;
//RX RING
extern volatile unsigned int usb_rx_ring_wr,
                             iot_rx_ring_wr; // used in ISR and Main() to place in buffer,
extern unsigned int usb_rx_ring_rd,
                    iot_rx_ring_rd;
//usb_rx_ring_rd Rx is only actually used in main because you're only reading from main
////TX RING
//extern unsigned int usb_tx_ring_wr,
//                    IoT_tx_ring_wr;
//extern unsigned int usb_tx_ring_rd,
//                    IoT_tx_ring_rd;
volatile char startup_okay_to_TX = FALSE,
              msg_recieved_from_IoT = FALSE,//
              msg_recieved_from_PC = FALSE;
//extern volatile int time_seconds,
//                    Tx_message_timer;

//volatile int debug_ISR_iot_1 = 0,
//             debug_ISR_iot_2 = 0,
//             debug_ISR_iot_3 = 0,
//             debug_ISR_usb_1 = 0,
//             debug_ISR_usb_2 = 0,
//             debug_ISR_usb_3 = 0;

/* The purpose of this ISR is JUST to use the write signal to PUT data in the buffer,
where we read that same buffer later in main.
*/
#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){  // USB module
  unsigned int temp,
               char_check;
  switch(__even_in_range (UCA1IV, 0x08)){
  case 0: // Vector 0 - no interrupt
    break;
  case 2: // Vector 2 - RXIFG
    //important note: there is only one buffer register, so this should not appear more than once in program
    temp = usb_rx_ring_wr; // all the extra local variable does is remove error where compiler wants you to resolve two volatiles in ISR
     char_check = UCA1RXBUF;
    //USB_Rx_Ring_Buff[temp] = UCA1RXBUF; // RX --> USB_Rx_Ring_Buff character (triggered every time something is placed in UCA1RXBUF)

    //This was used to test software loopback from PC to PC. Replaced by hardware loopback from PC to PC(jumper on J9)
    //UCA1TXBUF = UCA1_loop_back; // hand this char straight back to the USB Tx

    //Tx_message_timer = time_seconds + 2;  //enables 2 second message queue

//    if(!startup_okay_to_TX){
//      UCA1_index = 0;
//      strcpy(USB_Tx_Outgoing_Buff, "READY\r\n");
////      USB_Tx_Outgoing_Buff[0] = 'O';
////      USB_Tx_Outgoing_Buff[1] = 'K';
////      USB_Tx_Outgoing_Buff[2] = '\r';
////      USB_Tx_Outgoing_Buff[3] = '\n';
//      startup_okay_to_TX = TRUE;
//      UCA1IE |= UCTXIE; // enable the Tx interupt
//      msg_recieved_from_PC = FALSE;
//      UCA1TXBUF = USB_Tx_Outgoing_Buff[UCA1_index++];  //NOTE: this takes care of the duplicate initial char
//      return;
//    }
//    debug_ISR_usb_1++;
    if(char_check != NULL_CHAR){
//      debug_ISR_usb_2++;
      USB_Rx_Ring_Buff[temp] = char_check;
      msg_recieved_from_PC = TRUE;
      if (++usb_rx_ring_wr >= (sizeof(USB_Rx_Ring_Buff))){ //when you get to the end of the array
//        debug_ISR_usb_3++;
        usb_rx_ring_wr = RESET; // Circular buffer back to RESET
      }
    }
    break;
  case 4: // Vector 4 - TXIFG
    // MY implementation
    if(USB_Tx_Outgoing_Buff[UCA1_index] != NULL_CHAR){
      UCA1TXBUF = USB_Tx_Outgoing_Buff[UCA1_index++];
      return;
    }else if(USB_Tx_Outgoing_Buff[UCA1_index] == NULL_CHAR){ //if this is the last character
      UCA1IE &= ~UCTXIE;
      UCA1_index = 0;
      return;// disable Tx ISR so it doesn't trigger after string is transmitted
    }
  default:
    break;
  }
}

#pragma vector=EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){ // Wifi Module
  unsigned int temp,
               char_check;
  switch(__even_in_range (UCA0IV, 0x08)){
  case 0: // Vector 0 - no interrupt
    break;
  case 2: // Vector 2 - RXIFG
    // UCA0RXBUF is what I'm getting FROM the Wifi Module
    temp = iot_rx_ring_wr;
    char_check = UCA0RXBUF;

    //debug_ISR_iot_1++;
    //IoT_Rx_Ring_Buff[temp] = UCA0RXBUF; // RX --> IoT_Rx_Ring_Buff character

    UCA1TXBUF = char_check; // hand this char over to USB Tx to receive it on the PC
    if(char_check != NULL_CHAR){
      //debug_ISR_iot_2++;
      IoT_Rx_Ring_Buff[temp] = char_check; //also receive here to inspect
      msg_recieved_from_IoT = TRUE; // set this false after processing
      if (++iot_rx_ring_wr >= (sizeof(IoT_Rx_Ring_Buff))){ //when you get to the end of the array
        //debug_ISR_iot_3++;
        iot_rx_ring_wr = RESET; // Circular buffer back to RESET
      }

      //check whether wr wraped around rd
      if(wraparound_check_start == TRUE){
        //every == is like a falling edge that triggers an increment
        if(iot_rx_ring_wr == iot_rx_ring_rd + 1){
          wr_wrap_around_rd++;
        }

      }

    }
    break;
  case 4: // Vector 4 - TXIFG
    if(startup_okay_to_TX){
      if(IoT_Tx_Outgoing_Buff[UCA0_index] != NULL_CHAR){
        UCA0TXBUF = IoT_Tx_Outgoing_Buff[UCA0_index++];
        return;
      }else if(IoT_Tx_Outgoing_Buff[UCA0_index] == NULL_CHAR){ //if this is the last character
        UCA0IE &= ~UCTXIE;
        UCA0_index = 0;
        return;// disable Tx ISR so it doesn't trigger after string is transmitted
      }
    }
    break;

  }
}
