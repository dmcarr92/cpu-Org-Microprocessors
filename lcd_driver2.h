#ifndef LCD_DRIVER2_H
#define LCD_DRIVER2_H


#include <avr/io.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// int8_t distance_from_bottom;
// int8_t distance_from_top;
// bool row_select;

typedef struct {

	char str[8];
} line_p;


typedef struct {

        line_p buff[32];
	char row0_current[8];
	char row1_current[8];
	uint8_t size;
	uint8_t distance_from_top;
	uint8_t distance_from_bottom;
	bool row_select;
} string_buffer;


string_buffer *string_buffer_create(char first_str[8]);
void buffer_window_print(string_buffer *buffer);
void lcd_shift_up(string_buffer *buffer);
void lcd_shift_down(string_buffer *buffer);
void lcd_shift_up_show(string_buffer *buffer);
void lcd_shift_down_show(string_buffer *buffer);
void append_string(string_buffer *buffer, char new_str[8]);
void delete_buffer_tail(string_buffer *buffer);
void clear_buffer(string_buffer *buffer);
bool password_check( );


#endif
