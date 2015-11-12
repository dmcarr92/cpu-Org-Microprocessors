#include "button_driver.h"
#include "lcd_driver.h"
#include "lcd_driver2.h"


string_buffer *string_buffer_create(char first_str[8]) {

	
	string_buffer *buffer = malloc(sizeof(string_buffer));
	uint8_t i = 0;
	for(i; i < 32; i++) strcpy(buffer->buff[i].str, "        ");
	strcpy(buffer->row0_current, "        ");	
	strcpy(buffer->row1_current, "        ");

	strcpy(buffer->buff[0].str, first_str);
	strcpy(buffer->row0_current, first_str);
	
	buffer->size = 1;
	buffer->distance_from_top = 0;
	buffer->distance_from_bottom = -1;	
	buffer->row_select = 0;

	return buffer;
}


static void set_row_select(string_buffer *buffer) {

	buffer->row_select = 1;
}

 
static void clear_row_select(string_buffer *buffer) {
 
	buffer->row_select = 0;
}


void buffer_window_print(string_buffer *buffer) {

	if(buffer->size == 0) { }
	else if(buffer->size == 1) {

		lcd_erase( );
		puts(buffer->row0_current);
		write_command_lcd(0x0e);
		write_command_lcd(0x87);
		if(!buffer->row_select) lcd_goto_xy(0, 7);
		else lcd_goto_xy(1, 7);
	} else {

		lcd_erase( );
		puts(buffer->row0_current);
		write_command_lcd(0xc0);
		puts(buffer->row1_current);
		write_command_lcd(0x0e);
		write_command_lcd(0x87);
		if(!buffer->row_select) lcd_goto_xy(0, 7);
		else lcd_goto_xy(1, 7);
	}
}


void lcd_shift_up(string_buffer *buffer) {

	if(buffer->distance_from_top == 0) {

		clear_row_select(buffer);
	} else {
	
		uint8_t new_row0 = buffer->distance_from_top - 1;
		char temp0[8];
		char temp1[8];

		strcpy(temp1, buffer->row0_current);
		strcpy(temp0, buffer->buff[new_row0].str);
		strcpy(buffer->row0_current, temp0);
		strcpy(buffer->row1_current, temp1);

		buffer->distance_from_top--;
		buffer->distance_from_bottom++;
	}
}


void lcd_shift_down(string_buffer *buffer) {

	if(buffer->distance_from_bottom < 1) {

		if(buffer->size > 1) {

			set_row_select(buffer);
		} else { }
	} else {

		uint8_t buff_tail = buffer->size - 1;
		uint8_t new_row1 = buff_tail - (buffer->distance_from_bottom - 1);
		char temp0[8];
		char temp1[8];
	
		strcpy(temp0, buffer->row1_current);
		strcpy(temp1, buffer->buff[new_row1].str);
		strcpy(buffer->row0_current, temp0);
		strcpy(buffer->row1_current, temp1);
	
		buffer->distance_from_top++;
		buffer->distance_from_bottom--;
	}
}


void lcd_shift_up_show(string_buffer *buffer) {

	lcd_shift_up(buffer);
	buffer_window_print(buffer);
}


void lcd_shift_down_show(string_buffer *buffer) {

	lcd_shift_down(buffer);
	buffer_window_print(buffer);
}


void append_string(string_buffer *buffer, char new_str[8]) {

	uint8_t buff_size = buffer->size;
	
	if(buff_size == 32) return;
	if(buff_size == 0) strcpy(buffer->row0_current, new_str);
	if(buff_size == 1) strcpy(buffer->row1_current, new_str);

	strcpy(buffer->buff[buff_size].str, new_str);
	buffer->distance_from_bottom++;
	buffer->size++;
}


void delete_buffer_tail(string_buffer *buffer) {

	uint8_t buff_tail = buffer->size - 1;
	strcpy(buffer->buff[buff_tail].str, "        ");
	buffer->size--;
	
	uint8_t new_size = buffer->size;
	char temp0[8];
	char temp1[8];
	
	strcpy(temp0, buffer->buff[0].str);
	strcpy(temp1, buffer->buff[1].str);
	strcpy(buffer->row0_current, temp0);
	strcpy(buffer->row1_current, temp1);
	
	buffer->distance_from_top = 0;
	buffer->distance_from_bottom = new_size - 2;
}


void clear_buffer(string_buffer *buffer) {
	
	while(buffer->size != 0) delete_buffer_tail(buffer);
	buffer->distance_from_bottom = -2;
	buffer->size = 0;
	write_command_lcd(0x0c);
}


bool password_check( ) {

	bool pw1;
        bool pw2;
        bool pw3;
        bool pw4;

        ///////////////////
        /* PASSWORD LOCK */
        //      BBAC     //
        ///////////////////

        lcd_erase( );
        puts("Enter   password");
	lcd_goto_xy(0, 7);
        while(1) {

                if( get_button_A_state() ) {

                        pw1 = false;
                        break;
                }
                if( get_button_B_state() ) {

                        pw1 = true;
                        break;
                }
                if( get_button_C_state() ) {

                        pw1 = false;
                        break;
                }
        }
        lcd_erase( );
        lcd_goto_xy(0, 2);
        putc('*');
        _delay_ms(250);
        while(1) {

                if( get_button_A_state() ) {

                        pw2 = false;
                        break;
                }
                if( get_button_B_state() ) {

                        pw2 = true;
                        break;
                }
		if( get_button_C_state() ) {

                        pw2 = false;
                        break;
                }
        }
        putc('*');
        _delay_ms(250);
        while(1) {

                if( get_button_A_state() ) {

                        pw3 = true;
                        break;
                }
                if( get_button_B_state() ) {

                        pw3 = false;
                        break;
                }
                if( get_button_C_state() ) {

                        pw3 = false;
                        break;
                }
        }
        putc('*');
        _delay_ms(250);
        while(1) {

                if( get_button_A_state() ) {

                        pw4 = false;
                        break;
		 }
                if( get_button_B_state() ) {

                        pw4 = false;
                        break;
                }
                if( get_button_C_state() ) {

                        pw4 = true;
                        break;
                }
        }
        putc('*');
        _delay_ms(250);
        if(pw1 && pw2 && pw3 && pw4) {

                _delay_ms(250);
                lcd_erase( );
                puts("SUCCESS!");
                lcd_goto_xy(1, 7);
		
		return true;
        } else {

                _delay_ms(250);
                lcd_erase( );
                puts("FAILURE    RESET");
                lcd_goto_xy(0, 7);
                
		return false;
        }
}





















