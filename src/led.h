
#ifndef LED_H
#define LED_H

#include "../wiringPi/wiringPi/wiringPi.h"

int init_wiringPi();
int set_led_output(int p);
int led(int p, int mode);

#endif /* LED_H */	
