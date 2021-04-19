#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"

char display_menu_cycle_through;
char display_menu_state = 0; //currently 0 through 13
extern volatile char SW1_Pressed;
extern volatile char SW2_Pressed;
extern unsigned int speed_global;
extern char display_line[4][11];
extern volatile unsigned char display_changed;


void display_menu_state_process(void){

  if((display_menu_state == DISPLAY_DEFAULT) && SW1_Pressed){
    display_menu_state = DISPLAY_MENU_DIRECTION;
    SW1_Pressed = FALSE;
  }

  switch(display_menu_state){
//  case DISPLAY_MAIN:
//    break;
  case DISPLAY_MENU_DIRECTION: //2
    //code for displaying
    {
      const char* line_1 = "> Dir Ctrl";
      const char* line_2 = "  Speed   ";
      const char* line_3 = "  Bcklight";
      const char* line_4 = "  IoT     ";
      my_display_process(line_1, line_2, line_3, line_4);

      if(SW1_Pressed){
        display_menu_state = DISPLAY_MENU_SPEED;
        SW1_Pressed = FALSE;
      }

      if(SW2_Pressed){
        display_menu_state = DISPLAY_MENU_DIRECTION_FORWARD;
        SW2_Pressed = FALSE;
      }
    }
    break;
  case DISPLAY_MENU_SPEED: //3
    {
      const char* line_1 = "  Dir Ctrl";
      const char* line_2 = "> Speed   ";
      const char* line_3 = "  Bcklight";
      const char* line_4 = "  IoT     ";
      my_display_process(line_1, line_2, line_3, line_4);

      if(SW1_Pressed){
        display_menu_state = DISPLAY_MENU_BACKLIGHT;
        SW1_Pressed = FALSE;
      }
      if(SW2_Pressed){
        display_menu_state = DISPLAY_MENU_SPEED_SLOW;
        SW2_Pressed = FALSE;
      }
    }
    break;
  case DISPLAY_MENU_BACKLIGHT: //4
    {
      const char* line_1 = "  Dir Ctrl";
      const char* line_2 = "  Speed   ";
      const char* line_3 = "> Bcklight";
      const char* line_4 = "  IoT     ";
      my_display_process(line_1, line_2, line_3, line_4);

      if(SW1_Pressed){
        display_menu_state = DISPLAY_MENU_IOT;
        SW1_Pressed = FALSE;
      }
      if(SW2_Pressed){
        BACKLIGHT_TOGGLE();
        display_menu_state = DISPLAY_DEFAULT;
        SW2_Pressed = FALSE;
      }
    }
    break;
  case DISPLAY_MENU_IOT: //5
    {
      const char* line_1 = "  Dir Ctrl";
      const char* line_2 = "  Speed   ";
      const char* line_3 = "  Bcklight";
      const char* line_4 = "> IoT     ";
      my_display_process(line_1, line_2, line_3, line_4);

      if(SW1_Pressed){
        display_menu_state = DISPLAY_MENU_DIRECTION;
        SW1_Pressed = FALSE;
      }
      if(SW2_Pressed){
        //GREEN_LED_TOGGLE();
        display_menu_state = DISPLAY_DEFAULT;
        SW2_Pressed = FALSE;
      }
    }
    break;
  case DISPLAY_MENU_DIRECTION_FORWARD: //6
    //code to display
    {
      const char* line_1 = "> FORWARD ";
      const char* line_2 = "  STOP    ";
      const char* line_3 = "  REVERSE ";
      const char* line_4 = "          ";
      my_display_process(line_1, line_2, line_3, line_4);

      if(SW1_Pressed){
        display_menu_state = DISPLAY_MENU_DIRECTION_STOP;
        SW1_Pressed = FALSE;
      }
      if(SW2_Pressed){
        display_menu_state = DISPLAY_DEFAULT;
        SW2_Pressed = FALSE;
        speed_global = SLOW;
        forward(speed_global);
      }
    }
    break;
  case DISPLAY_MENU_DIRECTION_STOP: //7
    {
      const char* line_1 = "  FORWARD ";
      const char* line_2 = "> STOP    ";
      const char* line_3 = "  REVERSE ";
      const char* line_4 = "          ";
      my_display_process(line_1, line_2, line_3, line_4);

      if(SW1_Pressed){
        display_menu_state = DISPLAY_MENU_DIRECTION_REVERSE;
        SW1_Pressed = FALSE;
      }
      if(SW2_Pressed){
        display_menu_state = DISPLAY_DEFAULT;
        SW2_Pressed = FALSE;
        speed_global = WHEEL_OFF;
        stop_car();
      }
    }
    break;
  case DISPLAY_MENU_DIRECTION_REVERSE: //8
    {
      const char* line_1 = "  FORWARD ";
      const char* line_2 = "  STOP    ";
      const char* line_3 = "> REVERSE ";
      const char* line_4 = "          ";
      my_display_process(line_1, line_2, line_3, line_4);

      if(SW1_Pressed){
        display_menu_state = DISPLAY_MENU_DIRECTION_FORWARD;
        SW1_Pressed = FALSE;
      }
      if(SW2_Pressed){
        display_menu_state = DISPLAY_DEFAULT;
        SW2_Pressed = FALSE;
        speed_global = SLOW;
        reverse(speed_global);
      }
    }
    break;
  case DISPLAY_MENU_SPEED_SLOW: //9
    {
      const char* line_1 = "> SLOW    ";
      const char* line_2 = "  MEDIUM  ";
      const char* line_3 = "  FAST    ";
      const char* line_4 = "  TURBO   ";
      my_display_process(line_1, line_2, line_3, line_4);

      if(SW1_Pressed){
        display_menu_state = DISPLAY_MENU_SPEED_MEDIUM;
        SW1_Pressed = FALSE;
      }
      if(SW2_Pressed){
        display_menu_state = DISPLAY_DEFAULT;
        SW2_Pressed = FALSE;
        speed_global = SLOW;
        forward(speed_global);
      }
    }
    break;
  case DISPLAY_MENU_SPEED_MEDIUM: //10
    {
      const char* line_1 = "  SLOW    ";
      const char* line_2 = "> MEDIUM  ";
      const char* line_3 = "  FAST    ";
      const char* line_4 = "  TURBO   ";
      my_display_process(line_1, line_2, line_3, line_4);

      if(SW1_Pressed){
        display_menu_state = DISPLAY_MENU_SPEED_FAST;
        SW1_Pressed = FALSE;
      }
      if(SW2_Pressed){
        display_menu_state = DISPLAY_DEFAULT;
        SW2_Pressed = FALSE;
        speed_global = FAST;
        forward(speed_global);
      }
    }
    break;
  case DISPLAY_MENU_SPEED_FAST: //11
    {
      const char* line_1 = "  SLOW    ";
      const char* line_2 = "  MEDIUM  ";
      const char* line_3 = "> FAST    ";
      const char* line_4 = "  TURBO   ";
      my_display_process(line_1, line_2, line_3, line_4);

      if(SW1_Pressed){
        display_menu_state = DISPLAY_MENU_SPEED_TURBO;
        SW1_Pressed = FALSE;
      }
      if(SW2_Pressed){
        display_menu_state = DISPLAY_DEFAULT;
        SW2_Pressed = FALSE;
        speed_global = FAST;
        forward(speed_global);
      }
    }
    break;
  case DISPLAY_MENU_SPEED_TURBO: //12
    {
      const char* line_1 = "  SLOW    ";
      const char* line_2 = "  MEDIUM  ";
      const char* line_3 = "  FAST    ";
      const char* line_4 = "> TURBO   ";
      my_display_process(line_1, line_2, line_3, line_4);

      if(SW1_Pressed){
        display_menu_state = DISPLAY_MENU_SPEED_SLOW;
        SW1_Pressed = FALSE;
      }
      if(SW2_Pressed){
        display_menu_state = DISPLAY_DEFAULT;
        SW2_Pressed = FALSE;
        speed_global = TURBO;
        forward(speed_global);
      }
    }
    break;
  default: //0
    {
      strcpy(display_line[0], "ADC VALUES");
      update_string(display_line[0], 0);
      strcpy(display_line[1], "Left Right");
      update_string(display_line[1], 1);

      display_changed = 1;
    }
    break;
  }
}

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