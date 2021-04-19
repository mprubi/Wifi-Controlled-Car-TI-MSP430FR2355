#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"
#include <math.h>

volatile char USB_Char_Rx [SMALL_RING_SIZE];
volatile char USB_Char_Tx [SMALL_RING_SIZE];
volatile unsigned int usb_rx_ring_wr; // used in ISR and Main() to place in buffer,
unsigned int usb_rx_ring_rd;
//usb_rx_ring_rd Rx is only actually used in main because you're only reading from main
unsigned int usb_tx_ring_wr;
unsigned int usb_tx_ring_rd;

void Init_Serial_UCA1(void){ //Connects to the IoT Module, UCA1 connects to USB

  int i;
  for(i = 0; i <SMALL_RING_SIZE; i++){
    USB_Char_Rx[i] = 0x00; // USB Rx Buffer
  }
  usb_rx_ring_wr = BEGINNING;
  usb_rx_ring_rd = BEGINNING;

  for(i = 0; i <LARGE_RING_SIZE; i ++){ // May not use this
    USB_Char_Tx[i] = 0x00; // USB Tx Buffer
  }

  usb_tx_ring_wr = BEGINNING;
  usb_tx_ring_rd = BEGINNING;

  // Configure UART 0
  UCA1CTLW0 = 0; // Use word register
  UCA1CTLW0 |= UCSWRST; // Set Software reset enable
  UCA1CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as f_BRCLK

  /*
  9,600 Baud Rate
  1. Calculate N = fBRCLK / Baudrate
  N = SMCLK / 9,600 => 8,000,000 / 9,600 = 833.3333333
  if N > 16 continue with step 3, otherwise with step 2

  2. OS16 = 0, UCBRx = INT(N)

  continue with step 4

  3. OS16 = 1,
      UCx           = INT(N/16)
                    = INT(N/16)
                    = 833.333/16
                    => 52
      UCFx          = INT([(N/16) - INT(N/16)] ×16)
                    = ([833.333/16 - INT(833.333/16)]*16)
                    = (52.08333333 - 52)*16
                    = 0.083*16
                    => 1

  4. UCSx is found by looking up the fractional part of N (= N INT(N)) in table Table 18-4
      Decimal of SMCLK / 8,000,000 / 9,600 = 833.3333333 => 0.333 yields 0x49 [Table]
  5. If OS16 = 0 was chosen, a detailed error calculation is recommended to be performed

                    TX error (%) RX error (%)

  BRCLK   Baudrate UCOS16 UCBRx UCFx UCSx  neg  pos  neg  pos
  8000000  9600       1     52    1  0x49 0.08 0.04 0.10 0.14

  */

  UCA1BRW = 52;  // 9,600 Baud  // BRW - Baud Rate Control Word Register
  // UCA1MCTLW = UCSx + UCFx + UCOS16;    "+" means concatenate
  //         ==> 0x49 +  1   +   1;
  UCA1MCTLW = 0x4911 ;   // MCTLW - Modulation Control Word Register
  UCA1CTLW0 &= ~ UCSWRST; // Set Software reset enable  //CTLW - Control Word Register
  UCA1IE |= UCRXIE; // Enable RX interrupt
  // make sure ISR exists to handle the statement above

  //"prime the pump here (flag is set after this line)
  // USA1TxBUFF = 0x00;
}