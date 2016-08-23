
#include "led.h"

int init_wiringPi(){
	return wiringPiSetup();
}

int set_led_output(int p){
	pinMode(p, OUTPUT);
	return 0;
}

int led(int p, int mode){
	//mode can be HIGH/LOW defined in wiringPi.h as 1, 0
	digitalWrite(p, mode);
	return 0;
}