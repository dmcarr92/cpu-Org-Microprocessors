#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H


#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>


#define CONCATENATE(type, letter)	type##letter
#define PIN(letter)			CONCATENATE(PIN, letter)
#define PORT(letter)			CONCATENATE(PORT, letter)
#define DDR(letter)			CONCATENATE(DDR, letter)

#define RS_PORT			D
#define RW_PORT			B
#define E_PORT			D
#define DB7_PORT		D
#define DB6_PORT		B
#define DB5_PORT		B
#define DB4_PORT		B

#define RS              	(1 << 2)
#define RW              	(1 << 0)
#define E               	(1 << 4)
#define DB7             	(1 << 7)
#define DB6             	(1 << 5)
#define DB5             	(1 << 4)
#define DB4             	(1 << 1)


void write_command(uint8_t command);
void write_data(uint8_t data);
void config_lcd_display();
void turn_on_lcd_display();
void turn_off_lcd_display();
void lcd_goto_xy(uint8_t row, uint8_t column);
void lcd_erase();
void putc(char c);
void puts(char *s);


#endif
