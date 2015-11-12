#include "motor_driver.h"
#include "button_driver.h"
#include "lcd_driver.h"
#include "lcd_driver2.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>


int main() {


	config_push_button_array();
	config_lcd_display();	
	turn_on_lcd_display();

	puts("B to       begin");
	lcd_goto_xy(0, 7);
	do { } while( !get_button_B_state() );
	lcd_erase( );
	_delay_ms(250);
	puts("3PI DemoDillon C");
	_delay_ms(1000);
	lcd_erase( );
	
	bool pw_correct;
	pw_correct = password_check();
	if(!pw_correct) while(1) { };
	
	config_left_motor( );
	config_right_motor( );
	_delay_ms(250);
	
	bool done = false;
	uint8_t ascii_value = 48;
	string_buffer *buf = string_buffer_create("BUFFER");

	char *line2 = "1Count";
	char *line3 = "2Motors";
	char *line4 = "3Exit";
	append_string(buf, line2);
	append_string(buf, line3);
	append_string(buf, line4);

	lcd_erase( );
	_delay_ms(250);
	puts("A = up  C = down");
	_delay_ms(750);
	buffer_window_print(buf);

	uint8_t left_motor_speed;
	uint8_t right_motor_speed;

	uint8_t speed1 = 192;
	uint8_t speed2 = 128;
	uint8_t speed3 = 64;	

	uint8_t selection;
	while(1) {

		if( get_button_A_state() ) {

			lcd_shift_up_show(buf);
			_delay_ms(250);
		}
		if( get_button_C_state() ) {
		
			lcd_shift_down_show(buf);
			_delay_ms(250);
		}
		if( get_button_B_state() ) {

			if(!buf->row_select) selection = buf->distance_from_top;
			else selection = buf->distance_from_top + 1;
			lcd_erase( );
			puts("Any       button");
			_delay_ms(250);
			break;
		}
	}

	while(1) {

		if( (get_button_A_state()) || (get_button_B_state()) || (get_button_C_state()) ) {

			lcd_erase();
			write_command_lcd(0x0c);
			_delay_ms(200);
			break;
		}
	}

	while(selection == 1) {

		if( (get_button_A_state()) && (get_button_B_state()) && (get_button_C_state()) ) {

			shift_left_motor_brake();
			shift_right_motor_brake();
			_delay_ms(2500);
			lcd_erase();
			break;
		}

		if( get_button_B_state() ) {
			
			lcd_erase();
			putc((char)ascii_value);
			ascii_value++;
			if(ascii_value == 58) ascii_value = 48;
			_delay_ms(200);
		}
	}

	while(selection == 2) {

		if( done ) break;

		if( (get_button_A_state()) && (get_button_B_state()) && (get_button_C_state()) ) {

			shift_left_motor_brake();
			shift_right_motor_brake();
			_delay_ms(200);
			lcd_erase();
			break;
		} 
		
		if( get_button_B_state() ) {
			set_left_motor_speed(speed1);
			set_right_motor_speed(speed1);
			shift_left_motor_forward( );
			shift_right_motor_forward( );
		while(1) {
			left_motor_speed = get_left_motor_speed( );
			right_motor_speed = get_right_motor_speed( );
			if( get_button_A_state() ) {
				if(left_motor_speed == speed1) {
					set_left_motor_speed(speed2);
					shift_left_motor_forward( );
				} else if(left_motor_speed == speed2) {
					set_left_motor_speed(speed3);
					shift_left_motor_backward( );
				} else if(left_motor_speed == speed3) {
					shift_left_motor_coast( );
					OCR0A = 0xff;
					OCR0B = 0xff;
				}
				_delay_ms(200);
			}
			if( get_button_C_state() ) {
				if(right_motor_speed == speed1) {
					set_right_motor_speed(speed2);
					shift_right_motor_forward( );
				} else if(right_motor_speed == speed2) {
					set_right_motor_speed(speed3);
					shift_right_motor_backward( );
				} else if(right_motor_speed == speed3) {
					shift_right_motor_coast( );
					OCR2A = 0xff;
					OCR2B = 0xff;
				}
				_delay_ms(200);
			}
			if((left_motor_speed == 0x00) && (right_motor_speed == 0x00)) {
				done = true;
				break;
			}
		}
		}			 
	} 
	lcd_erase( );
	clear_buffer(buf);
	buffer_window_print(buf);
	_delay_ms(500);
	append_string(buf, "This");
	append_string(buf, "is");
	append_string(buf, "my");
	append_string(buf, "string");
	append_string(buf, "buffer");
	append_string(buf, "demo.");
	lcd_erase( );
	buffer_window_print(buf);
	set_left_motor_speed(speed1);
	set_right_motor_speed(speed1);
	shift_left_motor_forward( );
	shift_right_motor_forward( );
	while(1) {

		if( get_button_A_state() ) {

			lcd_shift_up_show(buf);
			_delay_ms(250);
		}
		if( get_button_C_state() ) {

			lcd_shift_down_show(buf);
			_delay_ms(250);
		}
		if( get_button_B_state() ) break;
	}
	shift_left_motor_backward( );
	shift_right_motor_backward( );
	_delay_ms(2500);
	lcd_erase( );
	puts("  Exit");
	return 0;
} 
