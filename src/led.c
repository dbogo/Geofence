
#include "led.h"

int init_wiringPi(void){
	//TODO: figure out a neat way to do something like
	// a try/catch on the wiringPiSetup(). it screws the program.
	if(wiringPiSetup() != 0)
		return -1;
	return 0;
}

int set_led_output(unsigned short p){
	pinMode(p, OUTPUT);
	return 0;
}

int led(unsigned char p, int mode){
	//mode can be HIGH/LOW defined in wiringPi.h as 1, 0
	digitalWrite(p, mode);
	return 0;
}