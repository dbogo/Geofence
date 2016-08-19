
#include "led.h"

int init_wiringPi(){
	return wiringPiSetup();
}

int set_pin(int p){
	pinMode(p, OUTPUT);
	return 0;
}

int light_led(int p){
	digitalWrite(p, HIGH);
	return 0;
}

int led_off(int p){
	digitalWrite(p, LOW);
	return 0;
}