#ifndef UTILS_H
#define UTILS_H

#include <sys/utsname.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define range_valid(x, lo, hi) ( ((x >= lo) && (x <= hi)) ) ? 1 : 0

/* TODO: increase precision? */
#define PI 3.14159265
#define E 2.71828182 

#define TIME_TO_WAIT_SEC 1
#define TIME_TO_WAIT_NSEC 0

#define MAX_NMEA_MSG_SIZE 83

#define TOLERATES_INTERRUPT 1 //NOTE: may need to NOT be constant later..

#define EINTR -1

/* TODO: add more platforms (and a distinction between x86_32/64) ??? */
typedef enum {  
	UNKNOWN_PLATFORM,
	X86,
	ARM 
} platform_id; 

/**
 * @brief      checks what platform we are on.
 * @return     returns the id of the platform. one if the values of the platform_id enum.
 */
platform_id identify_platform(void);

/**
 * @brief      suspends the main loop for a specific time period, using nanosleep().
 * @param[in]  tv_sec  first value of 'struct timespec', storing the seconds.
 * @param[in]  nsec    second value of 'struct timespec' storing the nanoseconds.
 * @return     returns the rv of nanosleep()
 */
int suspend_loop(time_t tv_sec, long nsec);


#endif /* UTILS_H */
