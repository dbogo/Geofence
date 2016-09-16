#ifndef UTILS_H
#define UTILS_H

#include <sys/utsname.h>
#include <time.h>
#include <stdio.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

/* TODO: increase precision? */
#define PI 3.14159265
#define E 2.71828182 

#define TIME_TO_WAIT_SEC 1
#define TIME_TO_WAIT_NSEC 0

#define MAX_NMEA_MSG_SIZE 83

#define TOLERATES_INTERRUPT 1 //NOTE: may need to NOT be constant later..

#define EINTR -1

/* TODO: add more platforms (and a distinction between x86_32/64) ??? */
typedef enum {  UNKNOWN_PLATFORM,
				X86,
				ARM } platform_id; 


/**
 * @brief      checks what platform the current machine.
 * @return     returns the id of the platform.
 */
platform_id identify_platform(void);

/**
 * @brief      manages the pause of loop for a certain time. for now
 *             using the nanosleep() function. 		
 * @return     value returned by nanosleep() upon successful of failed sleep/
 */
int suspend_loop(time_t tv_sec, long nsec);


#endif /* UTILS_H */
