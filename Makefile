cc		= avr-gcc
mcu		= atmega328p
f_cpu		= 20000000
ccopts		= -g -Wall -Os -mmcu=$(mcu) -DF_CPU=$(f_cpu)

default: main

all: program

motor_driver.o: motor_driver.c motor_driver.h
	$(cc) $(ccopts) -c motor_driver.c

button_driver.o: button_driver.c button_driver.h
	$(cc) $(ccopts) -c button_driver.c

lcd_driver.o: lcd_driver.c lcd_driver.h
	$(cc) $(ccopts) -c lcd_driver.c

lcd_driver2.o: lcd_driver2.c lcd_driver2.h
	$(cc) $(ccopts) -c lcd_driver2.c

main: main.c motor_driver.o button_driver.o lcd_driver.o lcd_driver2.o
	avr-gcc $(ccopts) -mmcu=$(mcu) -DF_CPU=$(f_cpu) -o main -g main.c motor_driver.o button_driver.o lcd_driver.o lcd_driver2.o

main.hex: main
	avr-strip main
	avr-objcopy -R .eeprom -O ihex main main.hex

program: main.hex
	avrdude -p m328p -c avrisp2 -P /dev/ttyACM0 -U flash:w:main.hex

clean:
	-rm main main.hex motor_driver.o button_driver.o lcd_driver.o lcd_driver2.o
