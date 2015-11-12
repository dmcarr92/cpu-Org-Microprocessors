#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H


#include <avr/io.h>
#include <stdbool.h>


#define CONCATENATE(type, letter)	type##letter
#define PIN(letter)			CONCATENATE(PIN, letter)
#define PORT(letter)			CONCATENATE(PORT, letter)
#define DDR(letter)			CONCATENATE(DDR, letter)

#define LEFT_MOTOR_A_PORT	D
#define LEFT_MOTOR_B_PORT	D
#define RIGHT_MOTOR_A_PORT	D
#define RIGHT_MOTOR_B_PORT	B

#define LEFT_MOTOR_A		(1 << 5)
#define LEFT_MOTOR_B		(1 << 6)
#define RIGHT_MOTOR_A		(1 << 3)
#define RIGHT_MOTOR_B		(1 << 3)
#define LEFT_MOTOR_A_STATE	(PORT(LEFT_MOTOR_A_PORT) & LEFT_MOTOR_A)
#define	LEFT_MOTOR_B_STATE	(PORT(LEFT_MOTOR_B_PORT) & LEFT_MOTOR_B)
#define RIGHT_MOTOR_A_STATE	(PORT(RIGHT_MOTOR_A_PORT) & RIGHT_MOTOR_A)
#define RIGHT_MOTOR_B_STATE	(PORT(RIGHT_MOTOR_B_PORT) & RIGHT_MOTOR_B)

#define COM_MASK   0xf0
#define PWM_COM_A (3 << 6) //set on compare match
#define PWM_COM_B (3 << 4)

#define FORWARD		1
#define BACKWARD	0

#define MOTOR_IS_COASTING	0b00000000
#define MOTOR_IS_BRAKING	0b00000011
#define MOTOR_IS_BACKWARD	0b00000010
#define MOTOR_IS_FORWARD	0b00000001


void config_left_motor();
void config_right_motor();

void set_left_motor_speed(uint8_t speed);
void set_right_motor_speed(uint8_t speed);

void shift_left_motor_forward();
void shift_right_motor_forward();

void shift_left_motor_backward();
void shift_right_motor_backward();

void shift_left_motor_coast();
void shift_right_motor_coast();

void shift_left_motor_brake();
void shift_right_motor_brake();

uint8_t get_left_motor_speed( );
uint8_t get_right_motor_speed( );

#endif
