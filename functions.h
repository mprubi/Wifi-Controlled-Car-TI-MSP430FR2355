//******************************************************************************
//
//  Description: This file contains the Function prototypes
//
//  Jim Carlson
//  Aug 2013
//
//  Edited by Mark Rubianes 2021
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (5.40.1)
//******************************************************************************
// Functions

// Initialization
void Init_Display(void);

// Interrupts
void Init_Interrupts(void);
__interrupt void Timer0_B0_ISR(void);
__interrupt void switch_interrupt(void);

// Analog to Digital Converter

// Clocks
void Init_Clocks(void);

// LED Configurations
void Init_LEDs(void);
void IR_LED_control(char selection);
void Backlite_control(char selection);
void LED_blink_at_rate(void);

  // LCD
void Display_Process(void);
void Display_Update(char p_L1,char p_L2,char p_L3,char p_L4);
void enable_display_update(void);
void update_string(char *string_data, int string);
void Init_LCD(void);
void lcd_clear(void);
void lcd_putc(char c);
void lcd_puts(char *s);

void lcd_power_on(void);
void lcd_write_line1(void);
void lcd_write_line2(void);
//void lcd_draw_time_page(void);
//void lcd_power_off(void);
void lcd_enter_sleep(void);
void lcd_exit_sleep(void);
//void lcd_write(unsigned char c);
//void out_lcd(unsigned char c);

void Write_LCD_Ins(char instruction);
void Write_LCD_Data(char data);
void ClrDisplay(void);
void ClrDisplay_Buffer_0(void);
void ClrDisplay_Buffer_1(void);
void ClrDisplay_Buffer_2(void);
void ClrDisplay_Buffer_3(void);

void SetPostion(char pos);
void DisplayOnOff(char data);
void lcd_BIG_mid(void);
void lcd_4line(void);
void lcd_out(char *s, char line, char position);
void lcd_rotate(char view);

//void lcd_write(char data, char command);
void lcd_write(unsigned char c);
void lcd_write_line1(void);
void lcd_write_line2(void);
void lcd_write_line3(void);

void lcd_command( char data);
void LCD_test(void);
void LCD_iot_meassage_print(int nema_index);

// Menu
void Menu_Process(void);

//port initializations
void Init_Ports(void);
void init_port_1();
void init_port_2();
void init_port_3();
void init_port_4();
void init_port_5();
void init_port_6();

// SPI
void Init_SPI_B1(void);
void SPI_B1_write(char byte);
void spi_rs_data(void);
void spi_rs_command(void);
void spi_LCD_idle(void);
void spi_LCD_active(void);
void SPI_test(void);
void WriteIns(char instruction);
void WriteData(char data);

// Switches
void Init_Switches(void);
void switch_control(void);
void enable_switch_SW1(void);
void enable_switch_SW2(void);
void disable_switch_SW1(void);
void disable_switch_SW2(void);
void Switches_Process(void);
void Init_Switch(void);
//void Switch_Process(void);
//void Switch1_Process(void);
//void Switch2_Process(void);
void menu_act(void);
void menu_select(void);

// Timers
void Init_Timers(void);
void Init_Timer_B0(void);
void Init_Timer_B1(void);
void Init_Timer_B2(void);
void Init_Timer_B3(void);

void usleep(unsigned int usec);
void usleep10(unsigned int usec);
void five_msec_sleep(unsigned int msec);
void measure_delay(void);
void out_control_words(void);

// Motor control and movement
//void forward_ON(void);
//void forward_OFF(void);
//void reverse_ON(void);
//void reverse_OFF(void);
//void all_motors_OFF(void);
//void shape_state_select(void);
//void motion_process(void);
//void motion_state_select(void);
//void initiate_motion(void);
//void my_sleep(int duration, char set_motion_state);
//void start_case(void);
//void end_case(void);
//void blink_LED(void);

//void fet_test(void);
//void fet_test_state_select(void);
//void fet_test_activate(void);

void Init_Timer_B0(void);
void Init_Timer_B1(void);
void Init_Timer_B2(void);
void Init_Timer_B3(void);

void carlsons_code(void);

char assign_PWM_values_with_error_check(void);
void forward(int speed, char direction, char time_seconds);
void reverse(int speed);
void stop_car(void);
void change_velocity(int magnitude, char acc_dec);

void Init_ADC (void);
void ADC_perform_moving_average(unsigned int volatile ADC_result, char specific_sensor);
void HEXtoBCD(int hex_value_left, int hex_value_right);
void adc_line(char display_line_position, char display_character_position);

void Init_Serial_UCA1(int baud_select);
void Init_Serial_UCA0(int baud_select);

void display_menu_state_process(void);
void my_display_process(const char* line1, const char* line2, const char* line3, const char* line4);

//PID
void follow_black_line(void);
int control_signal_calculation(void);
void process_actuator(int control_value);
char determine_current_state(void);

void TX_message_USB_UCA1(volatile char message[]);
void TX_message_IoT_UCA0(const char *message);

void RX_USB_Process_Buff(void);
void RX_IoT_Process_Buff(void);

void hop_to_new_buff(char from_buff[], char to_buff[]);
void load_TX_Out_buff_clr_P_Buff_1_and_kick_off(char outgoing_ISR);

void clear_display(void);
void strip_carat_before_IoT(void);
void parse_commands_from_IoT(void);

void clear_process_buffer(char process_buffer, int row_number, int buffer_size);

void pivot(char direction, int magnitude);


