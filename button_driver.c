#include "button_driver.h"


void config_push_button_array() {

	DDR(BUTTON_A_PORT) &= ~(BUTTON_A);
	DDR(BUTTON_B_PORT) &= ~(BUTTON_B);
	DDR(BUTTON_C_PORT) &= ~(BUTTON_C);

	PORT(BUTTON_A_PORT) |= BUTTON_A;
	PORT(BUTTON_B_PORT) |= BUTTON_B;
	PORT(BUTTON_C_PORT) |= BUTTON_C;
} 

bool get_button_A_state() {

	if((BUTTON_A_STATE) == 0b00000000) {

		return BUTTON_PUSHED;
	} else {

		return BUTTON_NOT_PUSHED;
	} 
} 

bool get_button_B_state() {

	if((BUTTON_B_STATE) == 0b00000000) {

		return BUTTON_PUSHED;
	} else {

		return BUTTON_NOT_PUSHED;
	} 
} 

bool get_button_C_state() {

	if((BUTTON_C_STATE) == 0b00000000) {

		return BUTTON_PUSHED;
	} else {

		return BUTTON_NOT_PUSHED;
	} 
}
