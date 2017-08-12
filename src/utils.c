#include <errno.h>
#include <string.h>

#include "utils.h"
#include "logInterface.h"


/* function wraps the use of nanosleep() function and does error handling 
	if toleratesInterrupt is true, then the nanosleep function will be checked
	for a premature stop, and will be resumed to finish the planned time (the remainder) 
	NOTE: should this function be in a separate file for such things ?
	
	//TODO: maybe instead of nanosleep implement a way using signals..
	// see: http://stackoverflow.com/questions/36953010/using-signals-in-c-how-to-stop-and-continue-a-program-when-timer-ends?
*/
int suspend_loop(time_t tv_sec, long nsec){
	/* TODO: consider the use of clock_nanosleep() */
	/* FIXME: maybe this function should call itself recursively
	 to make sure that the pause is fully ended, instead of checking only once.. */
	// NOTE: this struct declaration should maybe be somewhere else
#ifndef __arm__
	struct timespec ts = { .tv_sec = tv_sec , .tv_nsec = nsec };
	int errCode = 0;

	if(TOLERATES_INTERRUPT){
		struct timespec remainingTime;
		errCode = nanosleep(&ts, &remainingTime);
		// if EINTR then the pause has been interrupted
		if(errCode == EINTR) {
			log_err(&logMaster, "suspend_loop: nanosleep() was interrupted. continuing now.");
			nanosleep(&remainingTime, NULL);
		}
	}
	return nanosleep(&ts, NULL);
#else
	usleep(nsec/1000);
#endif
}

platform_id identify_platform(void){
	struct utsname name;
	uname(&name);
	if(strstr(name.machine, "x86"))
		return X86;
	else if(strstr(name.machine, "arm"))
		return ARM;

	return UNKNOWN_PLATFORM;
}