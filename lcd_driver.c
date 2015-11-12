#include "lcd_driver.h"


#define DATA		1
#define INSTRUCTION	0
#define BUSY_FLAG	(1 << 7)


/*   Single bit manipulation   */
/////////////////////////////////

static void rs_output() {

	DDR(RS_PORT) |= RS;
}

static void rw_output() {

	DDR(RW_PORT) |= RW;
}

static void e_output() {

	DDR(E_PORT) |= E;
}

static void db7_output() {

	DDR(DB7_PORT) |= DB7;
}

static void db6_output() {

	DDR(DB6_PORT) |= DB6;
}

static void db5_output() {

	DDR(DB5_PORT) |= DB5;
}

static void db4_output() {

	DDR(DB4_PORT) |= DB4;
}

static void db7_input() {

	DDR(DB7_PORT) &= ~DB7;
}

static void db6_input() {

	DDR(DB6_PORT) &= ~DB6;
}

static void db5_input() {

	DDR(DB5_PORT) &= ~DB5;
}

static void db4_input() {

	DDR(DB4_PORT) &= ~DB4;
}

static void set_rs() {

	PORT(RS_PORT) |= RS;
}

static void set_rw() {

	PORT(RW_PORT) |= RW;
}

static void set_e() {

	PORT(E_PORT) |= E;
}

static void set_db7() {

	PORT(DB7_PORT) |= DB7;
}

static void set_db6() {

	PORT(DB6_PORT) |= DB6;
}

static void set_db5() {
	
	PORT(DB5_PORT) |= DB5;
}

static void set_db4() {
	
	PORT(DB4_PORT) |= DB4;
}

static void clear_rs() {

	PORT(RS_PORT) &= ~RS;
}

static void clear_rw() {

	PORT(RW_PORT) &= ~RW;
}

static void clear_e() {

	PORT(E_PORT) &= ~E;
}

static void clear_db7() {

	PORT(DB7_PORT) &= ~DB7;
}

static void clear_db6() {

	PORT(DB6_PORT) &= ~DB6;
}

static void clear_db5() {

	PORT(DB5_PORT) &= ~DB5;
}

static void clear_db4() {

	PORT(DB4_PORT) &= ~DB4;
}

static bool get_db7() {

	return (PIN(DB7_PORT) & DB7);
}

static bool get_db6() {

	return (PIN(DB6_PORT) & DB6);
}

static bool get_db5() {

	return (PIN(DB5_PORT) & DB5);
}

static bool get_db4() {

	return (PIN(DB4_PORT) & DB4);
}


/*   Mid-level subroutines   */
///////////////////////////////

static void config_bus_output() {

	db7_output();
	db6_output();
	db5_output();
	db4_output();
}

static void config_bus_input() {

	db7_input();
	db6_input();
	db5_input();
	db4_input();
}

static uint8_t input_nibble() {

	uint8_t data = 0;

	bool db7 = get_db7();
	bool db6 = get_db6();
	bool db5 = get_db5();
	bool db4 = get_db4();
	
	if(db7) data |= (1 << 7);
	if(db6) data |= (1 << 6);
	if(db5) data |= (1 << 5);
	if(db4) data |= (1 << 4);

	return data;
}

static uint8_t pulse_e() {

	uint8_t data = 0;
	
	set_e();
	_delay_ms(1);
	data = input_nibble();
	clear_e();
	
	return data;
}

static void output_nibble(uint8_t data) {

	bool db7 = data & (1 << 7);
	bool db6 = data & (1 << 6);
	bool db5 = data & (1 << 5);
	bool db4 = data & (1 << 4);

	if(db7) set_db7();
	else	clear_db7();

	if(db6) set_db6();
	else	clear_db6();

	if(db5) set_db5();
	else	clear_db5();

	if(db4) set_db4();
	else	clear_db4();

	pulse_e();
}

uint8_t read_register(bool register_sel) {

	uint8_t data = 0;
	uint8_t upper4 = 0;
	uint8_t lower4 = 0;

	if(register_sel) set_rs();
	else		 clear_rs();

	set_rw();
	config_bus_input();
	upper4 = pulse_e();
	lower4 = pulse_e();
	data |= upper4;
	data |= (lower4 >> 4);
	
	return data;
}

static void busy_wait() {

	bool busy;
	uint8_t register_val = 0;
	
	do {

		register_val = read_register(INSTRUCTION);
		busy = (register_val & BUSY_FLAG);
	}while(busy);
}

static void write_register(uint8_t data, bool register_sel) {

	busy_wait();
	
	if(register_sel) set_rs();
	else		 clear_rs();

	clear_rw();
	config_bus_output();
	output_nibble(data);
	output_nibble(data << 4);
	config_bus_input();
}


/*   3PI API   */
/////////////////

void write_command_lcd(uint8_t command) {

	write_register(command, INSTRUCTION);
}

void write_data_lcd(uint8_t data) {

	write_register(data, DATA);
}

void config_lcd_display() {

	clear_rs();
	clear_rw();
	clear_e();

	rs_output();
	rw_output();
	e_output();
	config_bus_output();
	
	_delay_ms(15);
	output_nibble(0x30);
	_delay_ms(6);
	output_nibble(0x30);
	_delay_ms(2);
	output_nibble(0x30);
	_delay_ms(2);
	output_nibble(0x20);

	busy_wait();
	
	write_command_lcd(0x28);
	write_command_lcd(0x08);
	write_command_lcd(0x01);
	write_command_lcd(0x06);
}	

void turn_on_lcd_display() {

	write_command_lcd(0x0c);
}

void turn_off_lcd_display() {

	write_command_lcd(0x08);
}

void lcd_goto_xy(uint8_t row, uint8_t column) {

	uint8_t cursor_address = 0;
	uint8_t upper4 = 0;

	if(row == 0) upper4 = 0x80;
	else	     upper4 = 0xc0;
	
	cursor_address |= upper4;
	cursor_address |= column;

	write_command_lcd(cursor_address);
}

void lcd_erase() {

	write_command_lcd(0x01);
}

void putc(char c) {

	uint8_t cursor_location = read_register(INSTRUCTION);
	cursor_location &= ~BUSY_FLAG;
	
	if(cursor_location == 0x07) {
	
		write_data_lcd(c);
		write_command_lcd(0xc0);
	} else if(cursor_location == 0x47) {

		write_data_lcd(c);
		write_command_lcd(0x80);
	} else  write_data_lcd(c);
}

void puts(char *s) {

	uint8_t i = 0;
	uint8_t s_len = strlen(s);
	for(i; i < s_len; i++) putc(s[i]);
}



