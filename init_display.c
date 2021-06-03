//------------------------------------------------------------------------------
//
//  Description: Initializations file
//
//
//  Mark Rubianes
//  Jan 2021
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.11.2)
//------------------------------------------------------------------------------

#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"

extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char update_display;
extern volatile unsigned char display_changed;

void Init_Display(void){
//------------------------------------------------------------------------------
  int i;

  for(i=0;i<11;i++){
    display_line[0][i] = RESET_STATE;
    display_line[1][i] = RESET_STATE;
    display_line[2][i] = RESET_STATE;
    display_line[3][i] = RESET_STATE;
  }
  display_line[0][10] = 0;
  display_line[1][10] = 0;
  display_line[2][10] = 0;
  display_line[3][10] = 0;

  display[0] = &display_line[0][0];
  display[1] = &display_line[1][0];
  display[2] = &display_line[2][0];
  display[3] = &display_line[3][0];
  update_display = 0;

  //Startup  display message
  strcpy(display_line[0], "          ");
  update_string(display_line[0], 0);
  strcpy(display_line[1], "          ");
  update_string(display_line[1], 1);
  strcpy(display_line[2], "          ");
  update_string(display_line[2], 2);
  strcpy(display_line[3], "          ");
  update_string(display_line[3], 3);

  display_changed = 1;

  enable_display_update();  //removed as per HW 6 suggestions ??
  //Display_Update(3,2,1,0);
//------------------------------------------------------------------------------

}



