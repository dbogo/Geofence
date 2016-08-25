
#ifndef LED_H
#define LED_H

#include "../wiringPi/wiringPi/wiringPi.h"

#define STATUSLED 0
#define GEOFENCE_OK_LED 6

int init_wiringPi(void);
int set_led_output(int p);
int led(int p, int mode);

#endif /* LED_H */	
