
#ifndef LED_H
#define LED_H

#include "../libs/wiringPi/wiringPi/wiringPi.h"

#define STATUSLED 0
#define GEOFENCE_OK_LED 6

/**
 * @brief      call wiringPi's function for init of what's neccessary.
 * @return     returns the called function's rv.
 */
int init_wiringPi(void);

/**
 * @brief      "inits" the current led @p, to be available for wiringPi's actions 
 *             such as on or off.	
 * @param[in]  p     this led's wiringPi GPIO number
 * @return     0 on success.s
 */
int set_led_output(unsigned short p);

/**
 * @brief      swithes the led's state - either HIGH or LOW (on/off)
 * @param[in]  p     this led's wiringPi GPIO number
 * @param[in]  mode  on or off	
 * @return     0 on success
 */
int led(unsigned char p, int mode);

#endif /* LED_H */	
