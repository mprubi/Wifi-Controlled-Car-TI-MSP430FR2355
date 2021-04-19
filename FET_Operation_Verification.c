//#include  "functions.h"
//#include  "msp430.h"
//#include <string.h>
//#include "macros.h"
//
//char motor_test_state = 0;
//char activate_fet_test = 0;
//extern char display_line[4][11];
//char perform_statement_once = 0;
//
//void fet_test_state_select(void){
//
//  motor_test_state++;
//  if(motor_test_state > RGHT_REV_ON) motor_test_state = ALL_OFF;
//
//  switch(motor_test_state){
//  case ALL_OFF:
//    strcpy(display_line[0], "ALL OFF   ");
//    perform_statement_once = 1;
//    break;
//  case LFT_FRD_ON:
//    strcpy(display_line[0], "LT_FRD_ON ");
//    perform_statement_once = 1;
//    break;
//  case RGHT_FRD_ON:
//    strcpy(display_line[0], "RT_FRD_ON ");
//    perform_statement_once = 1;
//    break;
//  case LFT_REV_ON:
//    strcpy(display_line[0], "LT_REV_ON ");
//    perform_statement_once = 1;
//    break;
//  case RGHT_REV_ON:
//    strcpy(display_line[0], "RT_REV_ON ");
//    perform_statement_once = 1;
//    break;
//  }
//  all_motors_OFF();
//}
//
//void fet_test_activate(void){
//  activate_fet_test++;
//  if(activate_fet_test > 1) activate_fet_test = 0;
//
//}
//
//void fet_test(void){
////1) Write functions to turn all movement off, and then individual functions to
////turn on specific forward movement and specific reverse movement. Before
////issuing a movement command make a call to the movement off function.
//
//  if(activate_fet_test == 1){
//    switch(motor_test_state){
//    case ALL_OFF:
//      all_motors_OFF();
//      break;
//    case LFT_FRD_ON:
//      //4) With Battery on, Check Test points TP5 and TP7. With L_FORWARD off, TP5 and TP7
//      //should equal battery voltage. With L_FORWARD on, TP5 should be half of the
//      //battery voltage or less and TP7 should be about at ground potential.
//
//      if(perform_statement_once){
//        all_motors_OFF();
//        L_FORWARD_ON();
//        perform_statement_once = 0;
//      }
//      break;
//    case RGHT_FRD_ON:
//      //2) With Battery on, Check Test points TP1 and TP3. With R_FORWARD off, TP1 and TP3
//      //should equal battery voltage. With R_FORWARD on, TP1 should be half of the
//      //battery voltage or less and TP3 should be about at ground potential.
//
//      if(perform_statement_once){
//        all_motors_OFF();
//        R_FORWARD_ON();
//        perform_statement_once = 0;
//      }
//      break;
//    case LFT_REV_ON:
//      //5) With Battery on, Check Test points TP6 and TP8. With L_REVERSE off, TP6 and TP8
//      //should equal battery voltage. With L_REVERSE on, TP6 should be half of the
//      //battery voltage or less and TP8 should be about at ground potential.
//      if(perform_statement_once){
//        all_motors_OFF();
//        L_REVERSE_ON();
//        perform_statement_once = 0;
//      }
//      break;
//    case RGHT_REV_ON:
//      //3) With Battery on, Check Test points TP2 and TP4. With R_REVERSE off, TP2 and TP4
//      //should equal battery voltage. With R_REVERSE on, TP2 should be half of the
//      //battery voltage or less and TP4 should be about at ground potential.
//
//      if(perform_statement_once){
//        all_motors_OFF();
//        R_REVERSE_ON();
//        perform_statement_once = 0;
//      }
//      break;
//    default:
//      all_motors_OFF();
//    }
//  }else{
//    all_motors_OFF();
//  }
//}