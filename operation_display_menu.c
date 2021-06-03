#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"

char display_menu_cycle_through;
char display_menu_state = 0; //currently 0 through 13
extern volatile char SW1_Pressed;
extern volatile char SW2_Pressed;
extern unsigned int speed_global;
extern char display_line[4][11],
            wifi_connected;
extern volatile unsigned char display_changed;
char default_menu_toggle,
     default_menu_select = DEFAULT_MENU_SERIAL;
extern volatile char PID_enable_timer;
extern volatile int time_seconds,
                    time_half_seconds;

extern volatile char USB_Rx_Ring_Buff[LARGE_RING_SIZE],
                     IoT_Rx_Ring_Buff[LARGE_RING_SIZE];
extern char USB_Tx_Outgoing_Buff[LARGE_RING_SIZE],
            IoT_Tx_Outgoing_Buff[LARGE_RING_SIZE];

char baud_toggle = BAUD_9600,
     UCA_toggle = MENU_SEL_IoT;

extern volatile char activate_adc_readings;//

//const char *Tx_queue_USB[6] = {"UCA1_1\r\n",
//                               "UCA1_2\r\n",
//                               "UCA1_3\r\n",
//                               "UCA1_4\r\n",
//                               "UCA1_5\r\n",
//                               "UCA1_6\r\n"};
//const char *Tx_queue_IoT[6] = {"IoT_1\r\n",
//                               "IoT_2\r\n",
//                               "IoT_3\r\n",
//                               "IoT_4\r\n",
//                               "IoT_5\r\n",
//                               "IoT_6\r\n"};
char usb_msg_queue_incrementer = 0,
      iot_msg_queue_incrementer = 0;

extern char USB_Process_Buff[3][32];
extern char IoT_Process_Buff[3][32];
int display_scroll,
    display_scroll_timer;

void display_menu_state_process(void){

  if((display_menu_state == DISPLAY_DEFAULT) && SW1_Pressed){
    display_menu_state = DISPLAY_MENU_DIRECTION;
    SW1_Pressed = FALSE;
  }
  /*
  //  switch(display_menu_state){
  ////  case DISPLAY_MAIN:
  ////    break;
  //  case DISPLAY_MENU_DIRECTION: //2
  //    //code for displaying
  //    {
  //      const char* line_1 = DISPLAY_MENU_LINE_1_ARROW;
  //      const char* line_2 = DISPLAY_MENU_LINE_2;
  //      const char* line_3 = DISPLAY_MENU_LINE_3;
  //      const char* line_4 = DISPLAY_MENU_LINE_4;
  //      my_display_process(line_1, line_2, line_3, line_4);
  //
  //      if(SW1_Pressed){
  //        display_menu_state = DISPLAY_MENU_SPEED;
  //        SW1_Pressed = FALSE;
  //      }
  //      if(SW2_Pressed){
  //        display_menu_state = DISPLAY_MENU_DIRECTION_FORWARD;
  //        SW2_Pressed = FALSE;
  //      }
  //    }
  //    break;
  //  case DISPLAY_MENU_SPEED: //3
  //    {
  //      const char* line_1 = DISPLAY_MENU_LINE_1;
  //      const char* line_2 = DISPLAY_MENU_LINE_2_ARROW;
  //      const char* line_3 = DISPLAY_MENU_LINE_3;
  //      const char* line_4 = DISPLAY_MENU_LINE_4;
  //      my_display_process(line_1, line_2, line_3, line_4);
  //
  //      if(SW1_Pressed){
  //        display_menu_state = DISPLAY_MENU_BACKLIGHT;
  //        SW1_Pressed = FALSE;
  //      }
  //      if(SW2_Pressed){
  //        display_menu_state = DISPLAY_MENU_SPEED_SLOW;
  //        SW2_Pressed = FALSE;
  //      }
  //    }
  //    break;
  //  case DISPLAY_MENU_BACKLIGHT: //4
  //    {
  //      const char* line_1 = DISPLAY_MENU_LINE_1;
  //      const char* line_2 = DISPLAY_MENU_LINE_2;
  //      const char* line_3 = DISPLAY_MENU_LINE_3_ARROW;
  //      const char* line_4 = DISPLAY_MENU_LINE_4;
  //      my_display_process(line_1, line_2, line_3, line_4);
  //
  //      if(SW1_Pressed){
  //        display_menu_state = DISPLAY_MENU_SERIAL_COMS;
  //        SW1_Pressed = FALSE;
  //      }
  //      if(SW2_Pressed){
  //        BACKLIGHT_TOGGLE();
  //        display_menu_state = DISPLAY_DEFAULT;
  //        SW2_Pressed = FALSE;
  //      }
  //    }
  //    break;
  //  case DISPLAY_MENU_SERIAL_COMS: //5
  //    {
  //      const char* line_1 = DISPLAY_MENU_LINE_1;
  //      const char* line_2 = DISPLAY_MENU_LINE_2;
  //      const char* line_3 = DISPLAY_MENU_LINE_3;
  //      const char* line_4 = DISPLAY_MENU_LINE_4_ARROW;
  //      my_display_process(line_1, line_2, line_3, line_4);
  //
  //      if(SW1_Pressed){
  //        display_menu_state = DISPLAY_MENU_DIRECTION;  //cycle back to first menu item
  //        SW1_Pressed = FALSE;
  //      }
  //      if(SW2_Pressed){
  //        display_menu_state = DISPLAY_MENU_SERIAL_COMS_BAUD_SEL; //proceed to sub menu item
  //        SW2_Pressed = FALSE;
  //      }
  //    }
  //    break;
  //  case DISPLAY_MENU_DIRECTION_FORWARD: //6
  //    //code to display
  //    {
  //      const char* line_1 = "> FORWARD ";
  //      const char* line_2 = "  STOP    ";
  //      const char* line_3 = "  REVERSE ";
  //      const char* line_4 = "  PID Togl";
  //      my_display_process(line_1, line_2, line_3, line_4);
  //
  //      if(SW1_Pressed){
  //        display_menu_state = DISPLAY_MENU_DIRECTION_STOP;
  //        SW1_Pressed = FALSE;
  //      }
  //      if(SW2_Pressed){
  //        display_menu_state = DISPLAY_DEFAULT;
  //        SW2_Pressed = FALSE;
  //        speed_global = SLOW;
  //        forward(speed_global, SOFT_LEFT_TURN, 0);
  //      }
  //    }
  //    break;
  //  case DISPLAY_MENU_DIRECTION_STOP: //7
  //    {
  //      const char* line_1 = "  FORWARD ";
  //      const char* line_2 = "> STOP    ";
  //      const char* line_3 = "  REVERSE ";
  //      const char* line_4 = "  PID Togl";
  //      my_display_process(line_1, line_2, line_3, line_4);
  //
  //      if(SW1_Pressed){
  //        display_menu_state = DISPLAY_MENU_DIRECTION_REVERSE;
  //        SW1_Pressed = FALSE;
  //      }
  //      if(SW2_Pressed){
  //        display_menu_state = DISPLAY_DEFAULT;
  //        SW2_Pressed = FALSE;
  //        speed_global = WHEEL_OFF;
  //        stop_car();
  //      }
  //    }
  //    break;
  //  case DISPLAY_MENU_DIRECTION_REVERSE: //8
  //    {
  //      const char* line_1 = "  FORWARD ";
  //      const char* line_2 = "  STOP    ";
  //      const char* line_3 = "> REVERSE ";
  //      const char* line_4 = "  PID Togl";
  //      my_display_process(line_1, line_2, line_3, line_4);
  //
  //      if(SW1_Pressed){
  //        display_menu_state = DISPLAY_MENU_DIRECTION_PID;
  //        SW1_Pressed = FALSE;
  //      }
  //      if(SW2_Pressed){
  //        display_menu_state = DISPLAY_DEFAULT;
  //        SW2_Pressed = FALSE;
  //        speed_global = SLOW;
  //        reverse(speed_global);
  //      }
  //    }
  //    break;
  //  case DISPLAY_MENU_DIRECTION_PID: //9
  //    {
  //      const char* line_1 = "  FORWARD ";
  //      const char* line_2 = "  STOP    ";
  //      const char* line_3 = "  REVERSE ";
  //      const char* line_4 = "> PID Togl";
  //      my_display_process(line_1, line_2, line_3, line_4);
  //
  //      if(SW1_Pressed){
  //        display_menu_state = DISPLAY_MENU_DIRECTION_FORWARD;
  //        SW1_Pressed = FALSE;
  //      }
  //      if(SW2_Pressed){
  //        display_menu_state = DISPLAY_DEFAULT;
  //        SW2_Pressed = FALSE;
  //        speed_global = SLOW;
  //
  //        // start one second timer before toggling the PID_control_enabled
  //        PID_enable_timer += time_seconds;
  //      }
  //    }
  //    break;
  //  case DISPLAY_MENU_SPEED_SLOW: //10
  //    {
  //      const char* line_1 = "> SLOW    ";
  //      const char* line_2 = "  MEDIUM  ";
  //      const char* line_3 = "  FAST    ";
  //      const char* line_4 = "  TURBO   ";
  //      my_display_process(line_1, line_2, line_3, line_4);
  //
  //      if(SW1_Pressed){
  //        display_menu_state = DISPLAY_MENU_SPEED_MEDIUM;
  //        SW1_Pressed = FALSE;
  //      }
  //      if(SW2_Pressed){
  //        display_menu_state = DISPLAY_DEFAULT;
  //        SW2_Pressed = FALSE;
  //        speed_global = SLOW;
  //        forward(speed_global, STRAIGHT, 0);
  //      }
  //    }
  //    break;
  //  case DISPLAY_MENU_SPEED_MEDIUM: //11
  //    {
  //      const char* line_1 = "  SLOW    ";
  //      const char* line_2 = "> MEDIUM  ";
  //      const char* line_3 = "  FAST    ";
  //      const char* line_4 = "  TURBO   ";
  //      my_display_process(line_1, line_2, line_3, line_4);
  //
  //      if(SW1_Pressed){
  //        display_menu_state = DISPLAY_MENU_SPEED_FAST;
  //        SW1_Pressed = FALSE;
  //      }
  //      if(SW2_Pressed){
  //        display_menu_state = DISPLAY_DEFAULT;
  //        SW2_Pressed = FALSE;
  //        speed_global = FAST;
  //        forward(speed_global, STRAIGHT, 0);
  //      }
  //    }
  //    break;
  //  case DISPLAY_MENU_SPEED_FAST: //12
  //    {
  //      const char* line_1 = "  SLOW    ";
  //      const char* line_2 = "  MEDIUM  ";
  //      const char* line_3 = "> FAST    ";
  //      const char* line_4 = "  TURBO   ";
  //      my_display_process(line_1, line_2, line_3, line_4);
  //
  //      if(SW1_Pressed){
  //        display_menu_state = DISPLAY_MENU_SPEED_TURBO;
  //        SW1_Pressed = FALSE;
  //      }
  //      if(SW2_Pressed){
  //        display_menu_state = DISPLAY_DEFAULT;
  //        SW2_Pressed = FALSE;
  //        speed_global = FAST;
  //        forward(speed_global, STRAIGHT, 0);
  //      }
  //    }
  //    break;
  //  case DISPLAY_MENU_SPEED_TURBO: //13
  //    {
  //      const char* line_1 = "  SLOW    ";
  //      const char* line_2 = "  MEDIUM  ";
  //      const char* line_3 = "  FAST    ";
  //      const char* line_4 = "> TURBO   ";
  //      my_display_process(line_1, line_2, line_3, line_4);
  //
  //      if(SW1_Pressed){
  //        display_menu_state = DISPLAY_MENU_SPEED_SLOW;
  //        SW1_Pressed = FALSE;
  //      }
  //      if(SW2_Pressed){
  //        display_menu_state = DISPLAY_DEFAULT;
  //        SW2_Pressed = FALSE;
  //        speed_global = TURBO;
  //        forward(speed_global, STRAIGHT, 0);
  //      }
  //    }
  //    break;
  //  case DISPLAY_MENU_SERIAL_COMS_BAUD_SEL: //13
  //    {
  //      const char* line_1 = DISPLAY_MENU_SERIAL_LINE_1_ARROW;
  //      const char* line_2 = DISPLAY_MENU_SERIAL_LINE_2;
  //      const char* line_3 = DISPLAY_MENU_SERIAL_LINE_3;
  //      const char* line_4 = DISPLAY_MENU_SERIAL_LINE_4;
  //      my_display_process(line_1, line_2, line_3, line_4);
  //
  //      if(SW1_Pressed){
  //        display_menu_state = DISPLAY_MENU_SERIAL_COMS_UCA_SEL;
  //        SW1_Pressed = FALSE;
  //      }
  //      if(SW2_Pressed){
  //        display_menu_state = DISPLAY_DEFAULT;
  //        SW2_Pressed = FALSE;
  //        //actuating code
  //        if(baud_toggle == BAUD_9600){
  //          Init_Serial_UCA1(BAUD_115200);
  //          Init_Serial_UCA0(BAUD_115200);
  //          baud_toggle = BAUD_115200;
  //        }else if(baud_toggle == BAUD_115200){
  //          Init_Serial_UCA1(BAUD_9600);
  //          Init_Serial_UCA0(BAUD_9600);
  //          baud_toggle = BAUD_9600;
  //        }
  //      }
  //    }
  //    break;
  //  case DISPLAY_MENU_SERIAL_COMS_UCA_SEL: //13
  //    {
  //      const char* line_1 = DISPLAY_MENU_SERIAL_LINE_1;
  //      const char* line_2 = DISPLAY_MENU_SERIAL_LINE_2_ARROW;
  //      const char* line_3 = DISPLAY_MENU_SERIAL_LINE_3;
  //      const char* line_4 = DISPLAY_MENU_SERIAL_LINE_4;
  //      my_display_process(line_1, line_2, line_3, line_4);
  //
  //      if(SW1_Pressed){
  //        display_menu_state = DISPLAY_MENU_SERIAL_COMS_TX_MESSAGE;
  //        SW1_Pressed = FALSE;
  //      }
  //      if(SW2_Pressed){
  //        display_menu_state = DISPLAY_DEFAULT;
  //        SW2_Pressed = FALSE;
  //        //actuating code
  //        if(UCA_toggle == MENU_SEL_USB){
  //          UCA_toggle = MENU_SEL_IoT;
  //          //note: more actuating code needed
  //        }else if(UCA_toggle == MENU_SEL_IoT){
  //          UCA_toggle = MENU_SEL_USB;
  //        }
  //      }
  //    }
  //    break;
  //  case DISPLAY_MENU_SERIAL_COMS_TX_MESSAGE: //13
  //    {
  //      const char* line_1 = DISPLAY_MENU_SERIAL_LINE_1;
  //      const char* line_2 = DISPLAY_MENU_SERIAL_LINE_2;
  //      const char* line_3 = DISPLAY_MENU_SERIAL_LINE_3_ARROW;
  //      const char* line_4 = DISPLAY_MENU_SERIAL_LINE_4;
  //      my_display_process(line_1, line_2, line_3, line_4);
  //
  //      if(SW1_Pressed){
  //        display_menu_state = DISPLAY_MENU_SERIAL_COMS_BAUD_SEL;
  //        SW1_Pressed = FALSE;
  //      }
  //      if(SW2_Pressed){
  //        display_menu_state = DISPLAY_DEFAULT;
  //        SW2_Pressed = FALSE;
  //        //actuating code
  ////        if(UCA_toggle == MENU_SEL_USB){
  ////          TX_message_USB_UCA1(Tx_queue_USB[usb_msg_queue_incrementer++]);
  ////          if(usb_msg_queue_incrementer > 5){
  ////            usb_msg_queue_incrementer = 0;
  ////          }
  ////        }else if(UCA_toggle == MENU_SEL_IoT){
  ////          TX_message_IoT_UCA0(Tx_queue_IoT[iot_msg_queue_incrementer++]);
  ////          if(iot_msg_queue_incrementer > 5){
  ////            iot_msg_queue_incrementer = 0;
  ////            // add \r\n next
  ////          }
  ////        }
  //      }
  //    }
  //    break;
  //  default: //0
  //    {
  //
  //      //This encapsulates the toggling mechanism for two different default displays
  //      // within a SW2 press while display_menu_state == default
  //      if(SW2_Pressed){
  //        SW2_Pressed = FALSE;
  //        if(default_menu_toggle){
  //          default_menu_toggle = FALSE;
  //          default_menu_select = DEFAULT_MENU_SERIAL;
  //        }else if(!default_menu_toggle){
  //          default_menu_toggle = TRUE;
  //          default_menu_select = DEFAULT_MENU_ADC;
  //        }
  //      }
  //
  //      //
  //      switch(default_menu_select){
  //      case DEFAULT_MENU_SERIAL:
  //        //display line 0
  //        if(UCA1BRW == BRCWR_9600 && UCA1MCTLW == MCWR_9600){  // only based on UCA1
  //          strcpy(display_line[0], "Baud: 9600");
  //        }else if(UCA1BRW == BRCWR_115200 && UCA1MCTLW == MCWR_115200_1){
  //          strcpy(display_line[0], "Baud: 115K");
  //        }
  //
  //
  //        //display line 1 (Baud), 2(Tx), and 3 (Rx)
  //        if(UCA_toggle == MENU_SEL_USB){
  //          strcpy(display_line[1], "IoT P_buff");
  //          if(USB_Process_Buff[1][0] == 0x00){ // nothing to display so ensure screen is clear
  //            strcpy(display_line[2], "          ");
  //            strcpy(display_line[3], "          ");
  //          }else{
  //            for(int i = 0; i < 10; i++){ // if USB, display what arrived in UCA1
  //              display_line[2][i] = USB_Tx_Outgoing_Buff[i];
  //              display_line[3][i] = USB_Process_Buff[1][i];
  //            }
  //          }
  //        }else if(UCA_toggle == MENU_SEL_IoT){
  //          strcpy(display_line[1], "IoT P_buff");
  //
  //          //if(IoT_Process_Buff[P_BUFF_1][0] == 0x00){ // nothing to display so ensure screen is clear
  //            //strcpy(display_line[2], "          ");
  //            //strcpy(display_line[3], "          ");
  //          //}else{
  //          if(wifi_connected){
  //            strcpy(display_line[2], "CONNECTED ");
  //          }
  //
  //          if(display_scroll_timer != time_half_seconds){
  //            display_scroll_timer = time_half_seconds;
  //
  //            for(int i = 0; i <= 10; i++){ // scroll process buffer around display
  //              //              display_line[2][i] = IoT_Tx_Outgoing_Buff[];
  //              display_line[3][i] = IoT_Process_Buff[P_BUFF_1][display_scroll];
  //              if(display_scroll >= sizeof(IoT_Process_Buff[P_BUFF_1])){
  //                display_scroll = 0;
  //              }
  //              display_scroll++;
  //            }
  //          }
  //
  //          //}
  //        }
  //        break;
  //      case DEFAULT_MENU_ADC:
  //        break;
  //      default:
  //        break;
  //      }
  //
  //      update_string(display_line[0], 0);
  //      update_string(display_line[1], 1);
  //      update_string(display_line[2], 2);
  //      update_string(display_line[3], 3);
  //
  //      display_changed = 1;
  //    }
  //    break;
  //  }
  */

  /*
  switch(default_menu_select){
  case DEFAULT_MENU_SERIAL:
    //display line 0
    if(UCA1BRW == BRCWR_9600 && UCA1MCTLW == MCWR_9600){  // only based on UCA1
      strcpy(display_line[0], "Baud: 9600");
    }else if(UCA1BRW == BRCWR_115200 && UCA1MCTLW == MCWR_115200_1){
      strcpy(display_line[0], "Baud: 115K");
    }


    //display line 1 (Baud), 2(Tx), and 3 (Rx)
    if(UCA_toggle == MENU_SEL_USB){
      strcpy(display_line[1], "IoT P_buff");
      if(USB_Process_Buff[1][0] == 0x00){ // nothing to display so ensure screen is clear
        strcpy(display_line[2], "          ");
        strcpy(display_line[3], "          ");
      }else{
        for(int i = 0; i < 10; i++){ // if USB, display what arrived in UCA1
          display_line[2][i] = USB_Tx_Outgoing_Buff[i];
          display_line[3][i] = USB_Process_Buff[1][i];
        }
      }
    }else if(UCA_toggle == MENU_SEL_IoT){
      strcpy(display_line[1], "IoT P_buff");

      //if(IoT_Process_Buff[P_BUFF_1][0] == 0x00){ // nothing to display so ensure screen is clear
      //strcpy(display_line[2], "          ");
      //strcpy(display_line[3], "          ");
      //}else{
      if(wifi_connected){
        strcpy(display_line[2], "CONNECTED ");
      }

      if(display_scroll_timer != time_half_seconds){
        display_scroll_timer = time_half_seconds;

        for(int i = 0; i <= 10; i++){ // scroll process buffer around display
          //              display_line[2][i] = IoT_Tx_Outgoing_Buff[];
          display_line[3][i] = IoT_Process_Buff[P_BUFF_1][display_scroll];
          if(display_scroll >= sizeof(IoT_Process_Buff[P_BUFF_1])){
            display_scroll = 0;
          }
          display_scroll++;
        }
      }

      //}
    }
    break;
  case DEFAULT_MENU_ADC:
    break;
  default:
    break;
  }
  */

//  if(wifi_connected){
//    strcpy(display_line[2], "CONNECTED ");
//  }

    if(activate_adc_readings == TRUE){
      display_line[1][5] = 'T';
      display_line[1][6] = 'h';
      display_line[1][7] = 'r';
      display_line[1][8] = 's';
      display_line[1][9] = 'h';

      display_line[2][5] = 'S';
      display_line[2][6] = 'p';
      display_line[2][7] = 'd';
      display_line[2][8] = 'G';
      display_line[2][9] = 'l';

      display_line[3][5] = 'S';
      display_line[3][6] = 'C';
      display_line[3][7] = 'o';
      display_line[3][8] = 'r';
      display_line[3][9] = 'r';

    }else{
      if(display_scroll_timer != time_half_seconds){
        display_scroll_timer = time_half_seconds;

        for(int i = 0; i <= 10; i++){ // scroll process buffer around display
          //              display_line[2][i] = IoT_Tx_Outgoing_Buff[];
          display_line[3][i] = IoT_Process_Buff[P_BUFF_1][display_scroll];
          if(display_scroll >= sizeof(IoT_Process_Buff[P_BUFF_1])){
            display_scroll = 0;
          }
          display_scroll++;
        }
      }

    }

  update_string(display_line[0], 0);
  update_string(display_line[1], 1);
  update_string(display_line[2], 2);
  update_string(display_line[3], 3);

  display_changed = 1;
}


//void clear_display(void){
//  strcpy(display_line[0], "          ");
//  update_string(display_line[0], 0);
//  strcpy(display_line[1], "          ");
//  update_string(display_line[1], 1);
//  strcpy(display_line[2], "          ");
//  update_string(display_line[2], 2);
//  strcpy(display_line[3], "          ");
//  update_string(display_line[3], 3);
//
//  display_changed = 1;
//}
//
void my_display_process(const char* line1, const char* line2, const char* line3, const char* line4){
  strcpy(display_line[0], line1);
  update_string(display_line[0], 0);
  strcpy(display_line[1], line2);
  update_string(display_line[1], 1);
  strcpy(display_line[2], line3);
  update_string(display_line[2], 2);
  strcpy(display_line[3], line4);
  update_string(display_line[3], 3);

  display_changed = 1;
}