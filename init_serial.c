#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

volatile char USB_Rx_Ring_Buff[LARGE_RING_SIZE_32],
              IoT_Rx_Ring_Buff[VERY_LARGE_RING_SIZE],
              bang;
extern volatile char PID_control_enabled;
extern volatile char bang;

extern char current_state,
            previous_state;
char USB_Tx_Outgoing_Buff[LARGE_RING_SIZE_32],
     IoT_Tx_Outgoing_Buff[LARGE_RING_SIZE_32];
extern volatile char UCA1_index,
                     UCA0_index,
                     wr_wrap_around_rd,
                     wraparound_check_start,
                     time_seconds,
                     PID_enable_timer;
char command_argument_found,
      once_per_toggle = TRUE;

int black_line_edge_threshold = 550;
extern unsigned int speed_global;

unsigned int speed_correction = 10000;

//RX RING
volatile unsigned int usb_rx_ring_wr,
                      iot_rx_ring_wr, // used in ISR and Main() to place in buffer,
                      iot_process_buff_wr = 0;
unsigned int usb_rx_ring_rd, //rx_ring_rd mot used in interrupt
             iot_rx_ring_rd;
////TX RING
//unsigned int usb_tx_ring_wr,
//             IoT_tx_ring_wr;
//unsigned int usb_tx_ring_rd,
//             IoT_tx_ring_rd;

//for Rx process buffer
char USB_Process_Buff[NUM_BUFFERS][P_BUFF_SIZE_32],
     IoT_Process_Buff[NUM_BUFFERS][P_BUFF_SIZE_128]; // msgs from IoT are verbose
unsigned int usb_process_buff_wr = 0;

unsigned char current_command[2][LARGE_RING_SIZE];

extern volatile char startup_okay_to_TX,
                     activate_adc_readings;//
char usb_ring_to_process_buffer_complete = FALSE,
     iot_ring_to_process_buffer_complete = FALSE,
     ok_to_parse_recieved_iot_commands = FALSE,
     wifi_connected = FALSE;
//char programming_IoT = TRUE;
extern volatile char msg_recieved_from_IoT,
                     msg_recieved_from_PC;

void Init_Serial_UCA1(int baud_select){ // UCA1 connects to USB
  int Baud_Rate_Control_Word_Register,
      Modulation_Control_Word_Register;

  if(baud_select == BAUD_9600){
    Baud_Rate_Control_Word_Register = BRCWR_9600;
    Modulation_Control_Word_Register = MCWR_9600;
  }else if(baud_select == BAUD_115200){
    Baud_Rate_Control_Word_Register = BRCWR_115200;
    Modulation_Control_Word_Register = MCWR_115200_1;
  }else if(baud_select == BAUD_460800){
//    Baud_Rate_Control_Word_Register = 5;
//    Modulation_Control_Word_Register = 0x5551;
  }

  int i; //initialize entire USB Tx array (ring buffer) to NULL
  for(i = 0; i <LARGE_RING_SIZE_32; i++){
    USB_Rx_Ring_Buff[i] = NULL_CHAR; // USB Rx Buffer
  }
  usb_rx_ring_wr = RESET;
  usb_rx_ring_rd = RESET;

  //initialize entire USB Tx array (ring buffer) to NULL
  for(i = 0; i <LARGE_RING_SIZE_32; i++){ // May not use this
    USB_Tx_Outgoing_Buff[i] = NULL_CHAR; // USB Tx Buffer
  }
//  usb_tx_ring_wr = RESET;
//  usb_tx_ring_rd = RESET;

  // Configure UART 1
  UCA1CTLW0 = WORD_REGISTER; // Use word register
  UCA1CTLW0 |= UCSWRST; // Set Software reset enable
  UCA1CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as f_BRCLK

  UCA1BRW = Baud_Rate_Control_Word_Register;  // 9,600 Baud  // BRW - Baud Rate Control Word Register
  // UCA1MCTLW = UCSx + UCFx + UCOS16;    "+" means concatenate
  //         ==> 0x49 +  1   +   1;
  UCA1MCTLW = Modulation_Control_Word_Register ;   // MCTLW - Modulation Control Word Register
  UCA1CTLW0 &= ~ UCSWRST; // Set Software reset enable  //CTLW - Control Word Register
  UCA1IE |= UCRXIE; // Enable RX interrupt in __interrupt void eUSCI_A1_ISR
  // make sure ISR exists to handle the statement above

  //"prime the pump here (flag is set after this line)
  //UCA1IE |= UCTXIE;  // Enable Tx interrupt in __interrupt void eUSCI_A1_ISR
  //UCA1TXBUF = NULL_CHAR;

  startup_okay_to_TX = FALSE;
}

void Init_Serial_UCA0(int baud_select){ // UCA0 Connects to the IoT Module,
  int Baud_Rate_Control_Word_Register,
      Modulation_Control_Word_Register;

  if(baud_select == BAUD_9600){
    Baud_Rate_Control_Word_Register = BRCWR_9600;  // UCA0BRW = 52
    Modulation_Control_Word_Register = MCWR_9600;  // UCA0MCTLW = 0x4911
  }else if(baud_select == BAUD_115200){
    Baud_Rate_Control_Word_Register = BRCWR_115200;  //UCA0BRW = 4
    Modulation_Control_Word_Register = MCWR_115200_1;  //UCA0MCTLW = 0xAA51
  }else if(baud_select == BAUD_460800){
//    Baud_Rate_Control_Word_Register = 5;
//    Modulation_Control_Word_Register = 0x5551;
  }

  int i;
  for(i = 0; i <VERY_LARGE_RING_SIZE; i++){
    IoT_Rx_Ring_Buff[i] = NULL_CHAR; // USB Rx Buffer
  }
  iot_rx_ring_wr = RESET;
  iot_rx_ring_rd = RESET;

  for(i = 0; i <LARGE_RING_SIZE; i ++){ // May not use this
    IoT_Tx_Outgoing_Buff[i] = NULL_CHAR; // USB Tx Buffer
  }

//  IoT_tx_ring_wr = RESET;
//  IoT_tx_ring_rd = RESET;

  // Configure UART 0
  UCA0CTLW0 = 0; // Use word register
  UCA0CTLW0 |= UCSWRST; // Set Software reset enable
  UCA0CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as f_BRCLK

  UCA0BRW = Baud_Rate_Control_Word_Register;  // 115,200 Baud  // BRW - Baud Rate Control Word Register
  // UCA0MCTLW = UCSx + UCFx + UCOS16;    "+" means concatenate
  //         ==> 0x55 +  5   +   1;
  UCA0MCTLW = Modulation_Control_Word_Register;   // MCTLW - Modulation Control Word Register
  UCA0CTLW0 &= ~ UCSWRST; // Set Software reset enable  //CTLW - Control Word Register
  UCA0IE |= UCRXIE; // Enable RX interrupt in interrupt_rs232.c
  // make sure ISR exists to handle the statement above

  //"prime the pump here (flag is set after this line)
  //UCA0IE |= UCTXIE;  // Enable Tx interrupt in __interrupt void eUSCI_A1_ISR
  //UCA0TXBUF = 0x00;
}

void RX_USB_Process_Buff(void){
  if(usb_rx_ring_rd != usb_rx_ring_wr){//new character ready from ring buffer

    //grab the character, and null the ring buffer
    USB_Process_Buff[P_BUFF_1][usb_process_buff_wr++] = USB_Rx_Ring_Buff[usb_rx_ring_rd];
    USB_Rx_Ring_Buff[usb_rx_ring_rd++] = NULL_CHAR;

  }else if (usb_rx_ring_rd == usb_rx_ring_wr){ // 
//    USB_Process_Buff[P_BUFF_1][usb_process_buff_wr] = USB_Rx_Ring_Buff[usb_rx_ring_rd];  //NOTE: MISTAKE: By this time, last character is already read from ring buffer. This line copied an unnecessary extra character
    USB_Rx_Ring_Buff[usb_rx_ring_rd] = NULL_CHAR;
    usb_process_buff_wr = RESET;

    //COPY to BUFF 2 once at the end
    for(int i = 0; i<P_BUFF_SIZE_32; i++){
      USB_Process_Buff[P_BUFF_2][i] = USB_Process_Buff[P_BUFF_1][i];  //copying EVERYTHING to TWO buffers
      //USB_Process_Buff[P_BUFF_1][i] = NULL_CHAR; //
    }

    usb_ring_to_process_buffer_complete = TRUE;
  }
  //            check boundaries of ring and process rd ptrs
  //       NOT checking if wr wrapped around already
  if (usb_rx_ring_rd >= (sizeof(USB_Rx_Ring_Buff))){
    usb_rx_ring_rd = RESET;
  }
  //by the end of this function, expect P_BUFF_1 to be cleared
// decide if what I have is meant for IoT or PC
  if(usb_ring_to_process_buffer_complete){
    usb_ring_to_process_buffer_complete = FALSE;
    if(USB_Process_Buff[P_BUFF_1][0] == '^'){ //incomming command for IoT
      strip_carat_before_IoT();
      load_TX_Out_buff_clr_P_Buff_1_and_kick_off(UCA0_TX); // send to IoT
      clear_process_buffer(UCA1_TX, P_BUFF_1, P_BUFF_SIZE_32); //still clearing the USB process buffer here before msg reaches IoT
    }else{ // passthrough PC

      load_TX_Out_buff_clr_P_Buff_1_and_kick_off(UCA1_TX); // loop back around to PC
      clear_process_buffer(UCA1_TX, P_BUFF_1, P_BUFF_SIZE_32);
    }
  }
}
void strip_carat_before_IoT(void){
  for(int i = 0; i <= P_BUFF_SIZE_32-2; i++){
    USB_Process_Buff[P_BUFF_1][i] = USB_Process_Buff[P_BUFF_1][i+1];
  }
  //expect: outgoing TX buff == process buff - 1st carat, and all process buff 1 == 0x00
//assumption: don't need to worry about null chars after last char
}
void clear_process_buffer(char process_buffer, int row_number, int buffer_size){
  if(process_buffer == UCA1_TX){
    for(int i = 0; i < buffer_size; i++){
      USB_Process_Buff[row_number][i] = NULL_CHAR;
    }
  }else if(process_buffer == UCA0_TX){
    for(int i = 0; i < buffer_size; i++){
      IoT_Process_Buff[row_number][i] = NULL_CHAR;
    }
  }
}
void load_TX_Out_buff_clr_P_Buff_1_and_kick_off(char outgoing_ISR){
  //assumption: always copying from process buffer (32) to outgoing buffer (32)

    if(outgoing_ISR == UCA1_TX){
      for(int i = 0; i < LARGE_RING_SIZE; i++){ //copy to outgoing and null buffer ONE  //NOTE: MISTAKE: incorrect length of for loop overwrote second buffer
        USB_Tx_Outgoing_Buff[i] = USB_Process_Buff[P_BUFF_1][i];
      }
      msg_recieved_from_PC = FALSE;
      UCA1_index = 0;
      UCA1IE |= UCTXIE; // enable the Tx interupt
      UCA1TXBUF = USB_Tx_Outgoing_Buff[UCA1_index++];
    }else if(outgoing_ISR == UCA0_TX){
      for(int i = 0; i < LARGE_RING_SIZE_32; i++){ //copy to outgoing and null buffer ONE
        IoT_Tx_Outgoing_Buff[i] = USB_Process_Buff[P_BUFF_1][i];
      }
      msg_recieved_from_PC = FALSE;
      UCA0_index = 0;
      UCA0IE |= UCTXIE; // enable the Tx interupt
      UCA0TXBUF = IoT_Tx_Outgoing_Buff[UCA0_index++];
    }
}

////// functions below are for messages comming back to IoT

void RX_IoT_Process_Buff(void){

  if(iot_rx_ring_rd != iot_rx_ring_wr){//new character ready from ring buffer

     wraparound_check_start = TRUE;

    //grab the character, and null the ring buffer
    IoT_Process_Buff[P_BUFF_1][iot_process_buff_wr++] = IoT_Rx_Ring_Buff[iot_rx_ring_rd];
    IoT_Rx_Ring_Buff[iot_rx_ring_rd++] = NULL_CHAR;

  }else if ((iot_rx_ring_rd == iot_rx_ring_wr) && (wr_wrap_around_rd == 0)){ //reached the end of the
    wraparound_check_start = FALSE;
    IoT_Process_Buff[P_BUFF_1][iot_process_buff_wr] = IoT_Rx_Ring_Buff[iot_rx_ring_rd];
    IoT_Rx_Ring_Buff[iot_rx_ring_rd] = NULL_CHAR;
    iot_process_buff_wr = RESET;

    //COPY EVERYTHING TO BUFF 2
    for(int i = 0; i<P_BUFF_SIZE_128; i++){
      IoT_Process_Buff[P_BUFF_2][i] = IoT_Process_Buff[P_BUFF_1][i];  //copying EVERYTHING to TWO buffers
    }
    iot_ring_to_process_buffer_complete = TRUE;
  }

  //cycle rd around ring
  if (iot_rx_ring_rd >= (sizeof(IoT_Rx_Ring_Buff))){
      iot_rx_ring_rd = RESET;
    }

   //decrement wrap around valye each time rd overtakes wr
  if((wr_wrap_around_rd > 0) && (iot_rx_ring_rd == (iot_rx_ring_wr + 1))){
      wr_wrap_around_rd--;
    }

  if(iot_ring_to_process_buffer_complete){
    //send straight back to PC
    //use this just to see what IoT is seeing
    iot_ring_to_process_buffer_complete = FALSE;
    ok_to_parse_recieved_iot_commands = TRUE;
    msg_recieved_from_IoT = FALSE;

    if((IoT_Process_Buff[P_BUFF_2][0] == 'A') && (IoT_Process_Buff[P_BUFF_2][1] == 'T')){ //IoT acked AT+NSTCP command
      if((IoT_Process_Buff[P_BUFF_2][18] == 'C') && (IoT_Process_Buff[P_BUFF_2][19] == 'O')){
        wifi_connected = TRUE;
      }
    }

    //            check boundaries of ring and process rd ptrs
    //       NOT checking if wr wrapped around already
  }
}

//this is JUST receiving from IoT process buffer
void parse_commands_from_IoT(void){

  //list of commands, parse what's recieved FROM IoT module here
  // will be returning from IoT module(my commands that I send to the car)
  //if(IoT_Process_Buff[P_BUFF_2][0] == '^'){
  for(int i = 0; i <P_BUFF_SIZE_128; i++){
    if(IoT_Process_Buff[P_BUFF_1][i] == '^'){
      for (int j = i+1, k = 0; j < P_BUFF_SIZE_128; j++, k++){
        if(IoT_Process_Buff[P_BUFF_1][j] != CARRIAGE_RETURN){
          if(IoT_Process_Buff[P_BUFF_1][j] != '$'){
            if(command_argument_found == FALSE){
              current_command[0][k] = IoT_Process_Buff[P_BUFF_1][j];
            }else if(command_argument_found == TRUE){
              current_command[1][k] = IoT_Process_Buff[P_BUFF_1][j];
            }
          }else if(IoT_Process_Buff[P_BUFF_1][j] == '$'){
            command_argument_found = TRUE;
            j = j +1;
            k = 0;
            current_command[1][k] = IoT_Process_Buff[P_BUFF_1][j];
          }
        }else if(IoT_Process_Buff[P_BUFF_1][j] == CARRIAGE_RETURN){
          command_argument_found = FALSE;
          break;
        }
      }
      break;
    }
  }

  //list of commands starts here
  //parse two commands
  if(!(strcmp(current_command[0], "Green"))){
    GREEN_LED_TOGGLE();
  }
  if(!(strcmp(current_command[0], "Red"))){
    RED_LED_TOGGLE();
  }
  if(!(strncmp(current_command[0], "Forward", 7))){
//    if(current_command[1][0] != '\0'){
      forward(current_command[1][0], STRAIGHT, 0); //indefinite for now
//    }else if (current_command[1][0] == '\0'){
//      forward(MEDIUM, STRAIGHT, 0); //indefinite for now

  }
  if(!(strcmp(current_command[0], "Reverse"))){
    reverse(MEDIUM);
  }
  if(!(strncmp(current_command[0], "PL", 2))){
    if(current_command[1][0] == 'S'){
      pivot(LEFT_PIVOT, PIVOT);
    }else if(current_command[1][0] == 'H'){
    pivot(LEFT_PIVOT, MEDIUM);}
  }
  if(!(strncmp(current_command[0], "PR", 2))){
    if(current_command[1][0] == 'S'){
      pivot(RIGHT_PIVOT, PIVOT);
    }else if(current_command[1][0] == 'H'){
    pivot(RIGHT_PIVOT, MEDIUM);}
  }

  if(!(strcmp(current_command[0], "STOP"))){
    stop_car();
  }
  if(!(strcmp(current_command[0], "Backlight"))){
    BACKLIGHT_TOGGLE();
  }
  if(!(strcmp(current_command[0], "ADC"))){
    if(activate_adc_readings == TRUE){
      activate_adc_readings = FALSE;
      IR_LED_OFF();
      GREEN_LED_OFF();
      RED_LED_OFF();
    }else if (activate_adc_readings == FALSE){
      activate_adc_readings = TRUE;
      IR_LED_ON();
    }
  }
  if(!(strcmp(current_command[0], "Bang"))){
    if(bang == TRUE){
      bang = FALSE;
      once_per_toggle = TRUE;
      speed_global = 0;
      stop_car();
    }else if (bang == FALSE){
      bang = TRUE;
      if(once_per_toggle){
        once_per_toggle = FALSE;
        speed_global = SLOW;
        current_state = ON_LINE;
        previous_state = ON_LINE;
      }
    }
  }

  if(!(strcmp(current_command[0], "ThreshUP"))){
    if(black_line_edge_threshold <= 670){
      black_line_edge_threshold = black_line_edge_threshold + 20;
    }
  }
  if(!(strcmp(current_command[0], "ThreshDOWN"))){
    if(black_line_edge_threshold >= 0){
      black_line_edge_threshold = black_line_edge_threshold - 20;
    }
  }
  if(!(strcmp(current_command[0], "G_speedUP"))){
    if(speed_global < 50000){
      speed_global = speed_global + 5000;
    }
  }
  if(!(strcmp(current_command[0], "G_speedDOWN"))){
    if(speed_global > 0){
      speed_global = speed_global - 5000;
    }
  }

  if(!(strcmp(current_command[0], "s_correctUP"))){
    if(speed_correction <= 50000){
      speed_correction = speed_correction + 2000;
    }
  }
  if(!(strcmp(current_command[0], "s_correctDOWN"))){
    if(speed_correction >= 0){
      speed_correction = speed_correction - 2000;
    }
  }

  if(!(strcmp(current_command[0], "PID"))){
    if(PID_control_enabled == TRUE){
      PID_control_enabled = FALSE;
      stop_car();
    }else if (PID_control_enabled == FALSE){
      PID_control_enabled = TRUE;
    }
  }

  //clear current command and process buffer////
  for(int i = 0; i < LARGE_RING_SIZE; i++){
    current_command[0][i] = NULL_CHAR;
    current_command[1][i] = NULL_CHAR;
  }
  ok_to_parse_recieved_iot_commands = FALSE;
  //FIX THIS???
  clear_process_buffer(UCA0_TX, P_BUFF_1, P_BUFF_SIZE_128);
}
//}

//char str1[20] = "C programming";
//char str2[20];
//
//// copying str1 to str2
//strcpy(str2, str1);
//
//const char *message

void TX_message_USB_UCA1(volatile char message[]){
  // The while loop will stall as long as the Flag is not set (pump wasn't primed)[port is busy]
//  while (!(UCA1IFG & UCTXIFG)); // USCI_A1 TX buffer ready?
  //  UCA1TXBUF = character;
  //if(startup_okay_to_TX){
    UCA1_index = 0;
    
    for(int i = 0; i < sizeof(message); i++){
      USB_Tx_Outgoing_Buff[i] = message[i];
    }
    
    for(int i = sizeof(message); i < sizeof(USB_Tx_Outgoing_Buff); i++){
      USB_Tx_Outgoing_Buff[i] = NULL_CHAR;
    }
    
//    strncpy(USB_Tx_Outgoing_Buff, message, 10);
    UCA1IE |= UCTXIE; // enable the Tx interupt
    UCA1TXBUF = USB_Tx_Outgoing_Buff[UCA1_index++];
  //}
}

void TX_message_IoT_UCA0(const char *message){
  // The while loop will stall as long as the Flag is not set (pump wasn't primed)[port is busy]
  //  while (!(UCA1IFG & UCTXIFG)); // USCI_A1 TX buffer ready?
  //  UCA1TXBUF = character;
  //if(startup_okay_to_TX){ // prevent transmitting until something is recieved in RXBUF
    UCA0_index = 0;
    for(int i = 0; i<17; i++){
      IoT_Tx_Outgoing_Buff[i] = *message;
      message++;
    }
    //strncpy(IoT_Tx_Outgoing_Buff, message, sizeof(message));
    UCA0IE |= UCTXIE; // enable the Tx interupt
    UCA0TXBUF = IoT_Tx_Outgoing_Buff[UCA0_index++];
  //}
}