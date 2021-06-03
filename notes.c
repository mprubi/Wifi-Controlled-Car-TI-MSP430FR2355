//____________________


/*  //This is my first code to start and stop the motor for predefined time, defined in MAIN
    my_timer++;
    if(my_timer > 0xC350) { //about one second of time
      stop_flag = 1;
      my_timer = RESET_STATE;
    }
    if((my_timer < 0xC350) && (stop_flag == 0)){
      turn_on++;  //move forward/motor power
        if(turn_on == 75){
          //GREEN_LED_ON();
          R_FORWARD_ON();
          L_FORWARD_ON();
        }
        if(turn_on == 150){
          //GREEN_LED_OFF()
          R_FORWARD_OFF();
          L_FORWARD_OFF();
          turn_on = RESET_STATE;
        }
    }
    if(stop_flag == 1){
      //GREEN_LED_OFF();
          R_FORWARD_OFF();
          L_FORWARD_OFF();
          //my_timer = ZERO;
     }

*/

/*
//Carlsons example code for project 3, getting car to briefly move, no need to use this. Just keeping to look back

    if(Time_Sequence > 250){
      Time_Sequence = ZERO;

      if(my_start_moving == 1){
        my_moving++;
      }
      if(my_start_moving >= 2){
        forward();
      }
      if(my_start_moving >= 4){
        stop();
        my_moving = ZERO;
        my_start_moving = ZERO;
      }
    }


  }



//------------------------------------------------------------------------------
}

*/

    //--------------------------------------------------------------
    /*code for basic bare bones custom timing derived from Time_sequence
        the importance of this code is to be able to use timing without access
        to advanced clock tools (like the constraint imposed by an arduino)
      */
    /*

      extern unsigned int old_time_sequence = 0;
      extern unsigned int my_time;
      extern unsigned int period_count = 0;

      if(Time_Sequence != old_time_sequence){ // if timer moved forward
      my_time++;                            // derive new timing variable
      old_time_sequence = Time_Sequence;    // reset timer for new reference
      period_count++;  // "poor man's" PWM
      time_changed = 1;
    }
    //----------------------------------------------------------------
*/


/*
void wheels_straight_control(void){
switch(operation){ //switch control should determine whether this operation is true
    case IDLE:  //does nothing but stay in this state until button is pressed
//will have to check everything in idle every time, but other cases need a
//one_time varible, polling for wait state

    case WAIT:  // just for moving away from button, NOT for switch debouncing
// use timing to wait until safe to check button state
// wait for a portion of the period (a few count cycles)
// commence counting of some count values using myTime
//myTime = 0;
//if(myTime == some value ){swich to next case, no break statement}

    case OFF:
//turn wheels off;
break;
    case ON:
if(period_count >= DEFINED_PERIOD){
period_count = 0;
//turn both wheels on
      }
if(period_count >= TOO_FAST){
//turn fast wheel off
      }
break;
default: break;
  }
}

*/

/*
switch(STRAIGHT){
        case IDLE:
break;
        case WAIT:
//count some time off your my_time value
break;
        case START:
//this is where you place your wheel control
break;
        case END:
break;
default: break;
      }

switch(figure8_state){
        case IDLE:
break;
        case WAIT:
//count some time off your my_time value
break;
        case START:
//this is where you place your wheel control
break;
        case END:
break;
default: break;
      }

*/
/*
//my countdown timer code

if(this_timer != my_time){
      wait_count++;
      this_timer = my_time;
      GREEN_LED_OFF();
      RED_LED_TOGGLE();
    }
    if(wait_count == WAIT_THIS_LONG) {
      motion_state = START;
      wait_count = RESET_STATE;
    }
*/

/*

A few things to remember this week.
There is a set of Slides under Lecture Slides that discuss button configuration.
You can use your time to get that to work. A good test is to comment out the LED
control for the Switch Statement in Main and use a switch to turn on an Led and
Turn off an LED.
We covered extern and globals in class, but I am sure that likely is one of those
discussions that did not get fully absorbed.

In one of your files where you first require a variable, you will define a global
[unless it is a variable only needed in a specific function]. Defining that global
will be in the form of:

unsigned int period_count;

Since it is not assigned an initial value, it will be placed in the .bss section
[This is more to help with your first test].

As you continue coding and you discover you need to use that same global in another
file, then you will identify to that file that it has already been defined by giving
it the global definition in the firm:

extern unsigned int period_count;

*/
//ctrl A, ctrl T  re-does indentation as configured in settings


/*
//NOTE: This code given to me does not work
  //BASIC FET BOARD TEST: To make sure forward and reverse are not on simultaneously
  // place this in main before while loop
  if(((P6OUT | R_FORWARD) + (P6OUT | R_REVERSE)) == (R_FORWARD+R_REVERSE)){
    int oops=1;
  }
  if(((P6OUT | L_FORWARD)+(P6OUT | L_REVERSE)) == (L_FORWARD+L_REVERSE)){
    int oops=1;
  }

*/

//    // This is the "poor man's" timer when you need a timer in pinch
//    if(Time_Sequence != old_time_sequence){ // if timer moved forward
//      old_time_sequence = Time_Sequence;    // reset timer for new reference
//      time_changed = 1;
//      my_time++;
//      cycle_time++;
//    }



/*  Code for project 6 (hitting blck line and pivoting)
//    if(current_start_process != start_process){
//
//      switch(proj_6_state){
//      case 0:
//        if(ADC_Left_Sensor < 1020){
//          if(one_time){
//            start_counting = time_seconds + 1;
//            one_time = 0;
//          }
//          if(start_counting == time_seconds){
//            GREEN_LED_ON();
//            forward(MEDIUM);
//          }
//        }else if(ADC_Left_Sensor > 1020){
//          proj_6_state = 1;
//          one_time = 1;
//          GREEN_LED_OFF();
//        }
//        break;
//      case 1:
//        stop_car();
//
//        if(one_time){
//          start_counting = time_seconds + 1;
//          one_time = 0;
//          GREEN_LED_ON();
//        }
//        if(start_counting == time_seconds){
//          GREEN_LED_OFF();
//          proj_6_state = 2;
//          one_time = 1;
//        }
//        break;
//      case 2:
//        if(one_time){
//          start_counting = time_half_seconds + 1;
//          one_time = 0;
//          GREEN_LED_ON();
//
//          LEFT_FORWARD_SPEED = FAST;
//          RIGHT_REVERSE_SPEED = FAST;
//        }
//        if(start_counting == time_half_seconds){
//          stop_car();
//          proj_6_state = 0;
//          one_time = 1;
//          current_start_process = start_process;
//
//        }
//
//      break;
//    }
//  }
*/

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
                    => 52 = 52.08333333
      UCFx          = INT([(N/16) - INT(N/16)] ×16)
                    = ([833.333/16 - INT(833.333/16)]*16)
                    = (52.08333333 - 52)*16
                    = INT(0.083*16) = INT(1.328)
                    => 1

  4. UCSx is found by looking up the fractional part of N (= N INT(N)) in table Table 18-4
      Decimal of SMCLK / 8,000,000 / 9,600 = 833.3333333 => 0.333 yields 0x49 [Table]
  5. If OS16 = 0 was chosen, a detailed error calculation is recommended to be performed

                    TX error (%) RX error (%)

  BRCLK   Baudrate UCOS16 UCBRx UCFx UCSx  neg  pos  neg  pos
  8000000  9600       1     52    1  0x49 0.08 0.04 0.10 0.14

  */

/*
115,200 Baud Rate
  1. Calculate N = fBRCLK / Baudrate
  N = SMCLK / 115,200 => 8,000,000 / 115,200 = 69.444444
  if N > 16 continue with step 3, otherwise with step 2

  2. OS16 = 0, UCBRx = INT(N)

  continue with step 4

  3. OS16 = 1,
      UCx           = INT(N/16)
                    = INT(N/16)
                    = 69.444444/16
                    => 5
      UCFx          = INT([(N/16) - INT(N/16)] ×16)
                    = ([69.444444/16 - INT(69.444444/16)]*16)
                    = (4.340275 - 4)*16
                    = 0.340275*16 = 5.4444
                    => 5

  4. UCSx is found by looking up the fractional part of N (= N INT(N)) in table Table 18-4
      Decimal of SMCLK / 8,000,000 / 115,200 = 69.444444 => 0.444444 yields 0x55 [Table]
  5. If OS16 = 0 was chosen, a detailed error calculation is recommended to be performed

                    TX error (%) RX error (%)

  BRCLK   Baudrate UCOS16 UCBRx UCFx UCSx  neg  pos  neg  pos
  8000000  115,200   1     5     5  0x55  0.08 0.04 0.10 0.14
*/

/////////////////////////////////////////
/*  -- Wifi Module initial configuration log --
^AT
AT

OK
^AT+RESET=2
AT+RESET=2

APP Reset-APP SW Reset

---MEM-DUMP-START:


APP-ERROR-CODE:0 0 0 0

APP-ERROR-PARAM:
R[ 0]:00000000
R[ 1]:00000000
R[ 2]:00000000
R[ 3]:00000000
R[ 4]:00000000
R[ 5]:00000000
R[ 6]:00000000
R[ 7]:00000000
R[ 8]:00000000
R[ 9]:00000000
R[10]:00000000
R[11]:00000000
R[12]:00000000
R[13]:00000000
R[14]:00000000
R[15]:00000000


---MEM-DUMP-END:
Hard reset
Hard reset
Factory reset
Factory reset
^AT&F
AT&F

OK
step 39
step 39
save P0
save P0
^AT&W0
AT&W0

OK
save P1
save P1
^AT&W1
AT&W1

OK
boot to P1
boot to P1
^AT&Y1
AT&Y1

OK
establish clean conf
establish clean conf
^AT&W1
AT&W1

OK
soft reset
soft reset
^AT+RESET=1
AT+RESET=1

Serial2WiFi APP
42 now in stable state
42 now in stable state
next config net settings
next config net settings
AT+RESET=1
^AT
AT

OK
query net con manag
query net con manag
^AT+NCMAUTO=?
AT+NCMAUTO=?

NCM STARTED:7

OK
stop NCM
stop NCM
^AT+NCMAUTO=0,0,0
AT+NCMAUTO=0,0,0

OK
was already stopped
was already stopped
reboot P1
reboot P1
^AT&Y1
AT&Y1

OK
save to P1
save to P1
^AT&W1
AT&W1

OK
set Log lvl 2
set Log lvl 2
^AT+LOGLVL=2
AT+LOGLVL=2

OK
set radio always ON
set radio always ON
^AT+WRXACTIVE=1
AT+WRXACTIVE=1

OK
enable DHCP client
enable DHCP client
^AT+NDHCP=1
AT+NDHCP=1

OK
set wireless mode
set wireless mode
0 is WiFi station mode
0 is WiFi station mode
^AT+WM=0
AT+WM=0

OK
52 initiate auto con mode
52
^AT+WAUTO=0,"IGiveUp"
AT+WAUTO=0,"IGiveUp"

OK
dd was a finger slip
d was a finger slip
set pass
set pass
^AT+WAUTO="rubyloving"
AT+WAUTO="rubyloving"

ERROR: INVALID INPUT
^AT
AT

OK
^AT+WWPA="rubyloving"
AT+WWPA="rubyloving"

OK
save to P1
save to P1
^AT&W1
AT&W1

OK
start NCM
start NCM
^AT+NCMAUTO=0,1,0
AT+NCMAUTO=0,1,0

OK

    IP              SubNet         Gateway
 192.168.0.20:255.255.255.0:192.168.0.1
NWCONN-SUCCESS
save to P1
save to P1
^AT&W1
AT&W1

OK
reboot IoT
reboot IoT
^AT&Y1
AT&Y1

OK
query NCM state
query NCM state
^AT+NCMAUTO=??
AT+NCMAUTO=??

ACTIVE CLIENT PROFILE
+NDHCP=1 +NSET=192.168.1.99,255.255.255.0,192.168.1.1
+DNS1=0.0.0.0, +DNS2=0.0.0.0
+WM=0 +WAUTO=0,"IGiveUp",,
+NAUTO=1,1,192.168.1.99,8888
+WAUTH=0 +WWPA="rubyloving"+PSK-valid=1 +SSID=IGiveUp
+WWEP1=1234567890 +WWEP2=
+WWEP3= +WWEP4=

ACTIVE LIMITED-AP PROFILE
+NSET=192.168.240.1,255.255.255.0,192.168.240.1
+WAUTO=2,"GainSpanProv",1
+STATIONS=8  +REGDOMAIN=0  +BEACONINTRL=100
+DHCPSRVR=1  +DHCPCLIENTS=16  +DHCPLEASE=86400
+DNSSRVR=1  +DNSDOMAIN=config.gainspan
OK
check net status
check net status
^AT+NSTAT=?
AT+NSTAT=?
MAC=00:21:7e:2f:5c:79
WSTATE=CONNECTED     MODE=STA
BSSID=d8:0f:99:a8:ea:de   SSID="IGiveUp" CHANNEL=1   SECURITY=WPA2-PERSONAL
RSSI=-50
IP addr=192.168.0.20   SubNet=255.255.255.0  Gateway=192.168.0.1
DNS1=209.18.47.61       DNS2=209.18.47.62
Rx Count=246     Tx Count=5

OK
reset
reset
^AT+RESET=1
AT+RESET=1

Serial2WiFi APP

    IP              SubNet         Gateway
 192.168.0.20:255.255.255.0:192.168.0.1
NWCONN-SUCCESS
open port for comms
open port for comms
^AT+NSTCP=<Port>,1
AT+NSTCP=<Port>,1

ERROR: INVALID INPUT
^AT+NSTCP=<Port>,1030
AT+NSTCP=<Port>,1030

ERROR: INVALID INPUT
^AT+NSTCP=<Port>1030
AT+NSTCP=<Port>1030

ERROR: INVALID INPUT
^AT+NSTAT=?
AT+NSTAT=?
MAC=00:21:7e:2f:5c:79
WSTATE=CONNECTED     MODE=STA
BSSID=d8:0f:99:a8:ea:de   SSID="IGiveUp" CHANNEL=1   SECURITY=WPA2-PERSONAL
RSSI=-55
IP addr=192.168.0.20   SubNet=255.255.255.0  Gateway=192.168.0.1
DNS1=209.18.47.61       DNS2=209.18.47.62
Rx Count=780     Tx Count=6

OK
^AT+NSTCP=<1030>1
AT+NSTCP=<1030>1

ERROR: INVALID INPUT
^AT+NSTCP=<Port>,1
AT+NSTCP=<Port>,1

ERROR: INVALID INPUT
^AT+NSTCP=<Port>,1
^AT+NSTCP=<Port>,1,1030
^AT
AT+NSTCP=<Port>,1

ERROR: INVALID INPUT
[00]^AT
AT

ERROR: INVALID INPUT
^AT
AT

OK
^AT+NSTCP=<Port>,15,1030
AT+NSTCP=<Port>,15,1030

ERROR: INVALID INPUT
^AT+NSTCP=<Port>,1
AT+NSTCP=<Port>,1

ERROR: INVALID INPUT
^AT+NSTCP=1030,1
AT+NSTCP=1030,1

CONNECT 0

OK
the above WORKED!
the above WORKED!

CONNECT 0 1 192.168.0.27 54110
the above came from
the above came from
java TCP client
java TCP client
*/










//if(usb_rx_ring_rd != usb_rx_ring_wr){//new character ready from ring buffer
//
//    //grab the character, and null the ring buffer
//    USB_Process_Buff[P_BUFF_1][usb_process_buff_wr++] = USB_Rx_Ring_Buff[usb_rx_ring_rd];
//    USB_Rx_Ring_Buff[usb_rx_ring_rd++] = NULL_CHAR;
//
//  }else if (usb_rx_ring_rd == usb_rx_ring_wr){ //
//    USB_Process_Buff[P_BUFF_1][usb_process_buff_wr] = USB_Rx_Ring_Buff[usb_rx_ring_rd];
//    USB_Rx_Ring_Buff[usb_rx_ring_rd] = NULL_CHAR;
//    usb_process_buff_wr = RESET;
//
//    //COPY to BUFF 2 once at the end
//    for(int i = 0; i<P_BUFF_SIZE_32; i++){
//      USB_Process_Buff[P_BUFF_2][i] = USB_Process_Buff[P_BUFF_1][i];  //copying EVERYTHING to TWO buffers
//      //USB_Process_Buff[P_BUFF_1][i] = NULL_CHAR; //
//    }
//
//    ring_to_process_buffer_complete = TRUE;
//  }
//  //            check boundaries of ring and process rd ptrs
//  //       NOT checking if wr wrapped around already
//  if (usb_rx_ring_rd >= (sizeof(USB_Rx_Ring_Buff))){
//    usb_rx_ring_rd = RESET;
//  }
//  //by the end of this function, expect P_BUFF_1 to be cleared
//// decide if what I have is meant for IoT or PC
//  if(ring_to_process_buffer_complete){
//    ring_to_process_buffer_complete = FALSE;
//    if(USB_Process_Buff[P_BUFF_1][0] == '^'){ //incomming command for IoT
//      strip_carat_before_IoT();
//      load_TX_Out_buff_clr_P_Buff_1_and_kick_off(UCA0_TX); // send to IoT
//      clear_process_buffer(UCA1_TX, P_BUFF_1, P_BUFF_SIZE_32); //still clearing the USB process buffer here before msg reaches IoT
//    }else{ // passthrough PC
//
//      load_TX_Out_buff_clr_P_Buff_1_and_kick_off(UCA1_TX); // loop back around to PC
//      clear_process_buffer(UCA1_TX, P_BUFF_1, P_BUFF_SIZE_32);
//    }


