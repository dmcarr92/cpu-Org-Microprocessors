#ifndef BUTTON_DRIVER_H
#define BUTTON_DRIVER_H


#include <avr/io.h>
#include <stdbool.h>


#define CONCATENATE(type, letter)	type##letter
#define PIN(letter)			CONCATENATE(PIN, letter)
#define PORT(letter)			CONCATENATE(PORT, letter)
#define DDR(letter)			CONCATENATE(DDR, letter)


#define BUTTON_A_PORT		B
#define BUTTON_B_PORT		B
#define BUTTON_C_PORT		B

#define BUTTON_A		(1 << 1)
#define BUTTON_B		(1 << 4)
#define BUTTON_C		(1 << 5)

#define BUTTON_A_STATE		(PIN(BUTTON_A_PORT) & BUTTON_A)
#define BUTTON_B_STATE		(PIN(BUTTON_B_PORT) & BUTTON_B)
#define BUTTON_C_STATE		(PIN(BUTTON_C_PORT) & BUTTON_C)

#define BUTTON_PUSHED		1
#define BUTTON_NOT_PUSHED	0


void config_push_button_array();

bool get_button_A_state();
bool get_button_B_state();
bool get_button_C_state();


#endif
