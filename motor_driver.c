#include "motor_driver.h"
#include <util/delay.h>



static void left_motor_pwm_A_on( ) {

	TCCR0A |= PWM_COM_A;
}


static void left_motor_pwm_A_off( ) {

	TCCR0A &= ~PWM_COM_A;
}


static void left_motor_pwm_B_on( ) {

	TCCR0A |= PWM_COM_B;
}


static void left_motor_pwm_B_off( ) {

	TCCR0A &= ~PWM_COM_B;
}


static void right_motor_pwm_A_on( ) {

	TCCR2A |= PWM_COM_A;
}


static void right_motor_pwm_A_off( ) {

	TCCR2A &= ~PWM_COM_A;
}


static void right_motor_pwm_B_on( ) {

	TCCR2A |= PWM_COM_B;
}


static void right_motor_pwm_B_off( ) {

	TCCR2A &= ~PWM_COM_B;
}


void config_left_motor() {

	DDR(LEFT_MOTOR_A_PORT) |= LEFT_MOTOR_A;
	DDR(LEFT_MOTOR_B_PORT) |= LEFT_MOTOR_B;
	shift_left_motor_brake();
	TCCR0A = 0x03;
	TCCR0B = 0x01;
	OCR0A = 0x00;
	OCR0B = 0x00;
} 


void config_right_motor() {

	DDR(RIGHT_MOTOR_A_PORT) |= RIGHT_MOTOR_A;
	DDR(RIGHT_MOTOR_B_PORT) |= RIGHT_MOTOR_B;
	shift_right_motor_brake();
	TCCR2A = 0x03; //WGM(2:0) = "011" : Fast-PWM, TOP = 0xFF, BOTTOM = 0x00
	TCCR2B = 0x01; //CS(2:0)  = "001" : select I/Oclk
	OCR2A = 0x00;
	OCR2B = 0x00;
} 


void set_left_motor_speed(uint8_t speed) {

	if(speed > 240) {
		shift_left_motor_coast( );
	} else {

		OCR0A = speed;
		OCR0B = speed;
	}
}

		
void set_right_motor_speed(uint8_t speed) {

	if(speed > 240) {
		shift_right_motor_coast( );
	} else {

		OCR2A = speed;
		OCR2B = speed;
	}
}


void shift_left_motor_forward() {
	
	if((TCCR0A & PWM_COM_A) != 0x00) {
		shift_left_motor_brake( );
		_delay_ms(50);
	}
	left_motor_pwm_A_off( );
	left_motor_pwm_B_on( );
	PORT(LEFT_MOTOR_B_PORT) |= LEFT_MOTOR_B;
} 

void shift_right_motor_forward() {
	
	if((TCCR2A & PWM_COM_A) != 0x00) {
		shift_right_motor_brake( );
		_delay_ms(50);
	}
	right_motor_pwm_A_off( );
	right_motor_pwm_B_on( );
	PORT(RIGHT_MOTOR_B_PORT) |= RIGHT_MOTOR_B;
} 

void shift_left_motor_backward() {
	
	if((TCCR0A & PWM_COM_B) != 0x00) {
		shift_left_motor_brake( );
		_delay_ms(50);
	}
	left_motor_pwm_A_on( );
	left_motor_pwm_B_off( );
	PORT(LEFT_MOTOR_A_PORT) |= LEFT_MOTOR_A;
} 

void shift_right_motor_backward() {

	if((TCCR2A & PWM_COM_B) != 0x00) {
		shift_right_motor_brake( );
		_delay_ms(50);
	}
	right_motor_pwm_A_on( );
	right_motor_pwm_B_off( );
	PORT(LEFT_MOTOR_A_PORT) |= LEFT_MOTOR_A;
} 

void shift_left_motor_coast() {

	left_motor_pwm_A_off( );
	left_motor_pwm_B_off( );
	PORT(LEFT_MOTOR_A_PORT) &= ~(LEFT_MOTOR_A);
	PORT(LEFT_MOTOR_B_PORT) &= ~(LEFT_MOTOR_B);
} 

void shift_right_motor_coast() {

	right_motor_pwm_A_off( );
	right_motor_pwm_B_off( );
	PORT(RIGHT_MOTOR_A_PORT) &= ~(RIGHT_MOTOR_A);
	PORT(RIGHT_MOTOR_B_PORT) &= ~(RIGHT_MOTOR_B);
} 

void shift_left_motor_brake() {

	left_motor_pwm_A_off( );
	left_motor_pwm_B_off( );
	PORT(LEFT_MOTOR_A_PORT) |= LEFT_MOTOR_A;
	PORT(LEFT_MOTOR_B_PORT) |= LEFT_MOTOR_B;
} 

void shift_right_motor_brake() {

	right_motor_pwm_A_off( );
	right_motor_pwm_B_off( );
	PORT(RIGHT_MOTOR_A_PORT) |= RIGHT_MOTOR_A;
	PORT(RIGHT_MOTOR_B_PORT) |= RIGHT_MOTOR_B;
} 


uint8_t get_left_motor_speed( ) {

	if((TCCR0A & COM_MASK) != 0x00) return OCR0A;
	else				return 0;
}


uint8_t get_right_motor_speed( ) {

	if((TCCR2A & COM_MASK) != 0x00) return OCR2A;
	else				return 0;
}
