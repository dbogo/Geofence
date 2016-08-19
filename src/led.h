
#ifndef LED_H
#define LED_H

#include "../wiringPi/wiringPi/wiringPi.h"

int init_wiringPi();
int set_pin(int p);

int light_led(int p);
int led_off(int p);


#endif /* LED_H */	
