#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#include <sys/time.h>
#include <signal.h>

#include "main.h"
#include "utils.h"
#include "logInterface.h"
#include "GPSInterface.h" // TODO: reconsider this hierarcy !!
#include "../GPSDemo/src/gps_demo.h"
//#include "../RPiGPSDemo/src/rpi_gps_demo.h"

#if 0
void timer_handler(int signum){
	static int count = 0;
	printf("SIG: %d timer expired %d times\n", signum, ++count);
}
#endif

int main(int argc, char** argv) {
	init();
	finish();
	// hardcode some zone data for a quick test !
	Zone_general zone;
	zone.numVertices = 6;
	zone.altitude = 285.0f;
	GEO_Point p0 = { .latitude = 0.0f, .longitude = 0.0f};
	GEO_Point p1 = { .latitude = 10.0f, .longitude = 0.0f};
	GEO_Point p2 = { .latitude = 17.0f, .longitude = 7.5f};
	GEO_Point p3 = { .latitude = 17.0f, .longitude = 17.0f};
	GEO_Point p4 = { .latitude = 11.0f, .longitude = 21.0f};
	GEO_Point p5 = { .latitude = 0.0f, .longitude = 22.0f};
	
	zone.vertices[0] = p0;
	zone.vertices[1] = p1;
	zone.vertices[2] = p2;
	zone.vertices[3] = p3;
	zone.vertices[3] = p3;
	zone.vertices[4] = p4;
	zone.vertices[5] = p5;
	
	Log_Master logMaster = {
		.operationLogger = {
			.logFile = fopen(OPERATION_LOG_FILE, "w"),
			.logObj = NULL,
			.logInstanceName = "operationlog"
		} , 
		.errorLogger = {
			.logFile = fopen(ERROR_LOG_FILE, "w"),
			.logObj = NULL,
			.logInstanceName = "errorlog"
		}
	};

	initLogSystem(&logMaster);

	GPSSamp sample;
	char logStr[80];
	//int sampleStatus = 0; // invalid(9), gga(2), rmc(3), or full(1).
	
	//time_t startTimeSec = time(NULL);
	time_t currentTimeSec = 0;
	
	//TODO: maybe instead of nanosleep implement a way using signals..
	// see: http://stackoverflow.com/questions/36953010/using-signals-in-c-how-to-stop-and-continue-a-program-when-timer-ends?

	while (!suspend_loop(false)) {
		
		currentTimeSec = time(NULL);
		getGPSSample(&sample, true);

		printf("lon: %f, lat %f\n", sample.latitude, sample.longitude);

		//printf("lat: %f, lon: %f, alt: %f, crs: %f, spd: %f\n", sample.latitude, 
		//	sample.longitude, sample.altitude, sample.course, sample.speed);

		sprintf(logStr, "@(%ld) lat: %f, lon: %f, alt: %f, crs: %f, spd: %f |", 
			(long)currentTimeSec, sample.latitude, sample.longitude, 
			sample.altitude, sample.course, sample.speed);

		
		// whether currently in border
		if(isSampleInRangeGeneral(&sample, &zone)){
			printf("Current pos - within border\n");
		} else {
			printf("Current pos - outside the border\n");
		}

#if 0
		// whether estimated to go out in the next iteration of this loop.
		if(isDroneGoingOffBorder(&sample, &zone)){
			printf("Next estimated pos - outside of border\n");
		} else {
			printf("Next estimated pos - within border\n");
		}
#endif		
		
		//log the operation. (timestamp and the data of a GPSSamp)
		logEvent(logMaster.operationLogger.logObj, LOG4C_PRIORITY_INFO, logStr);

		//log the NMEA sentence from GPS. FIXME: TEMPORARELY DOESN'T LOG THE SENTENCES.
		//sprintf(logStr, "@(%ld), <temporary: nmea sentence place holder>", (long)currentTimeSec);
		//logEvent(nmeaLogger, LOG4C_PRIORITY_INFO, logStr);
		printf("******************************\n");
	}

	fclose(logMaster.operationLogger.logFile);
	fclose(logMaster.errorLogger.logFile);
	finiLogSystem();
	
	return (0);
}

/* function wraps the use of nanosleep() function and does error handling 
	if toleratesInterrupt is true, then the nanosleep function will be checked
	for a premature stop, and will be resumed to finish the planned time (the remainder) 
	NOTE: should this function be in a separate file for such things ?
	*/
int suspend_loop(bool toleratesInterrupt){
	/* TODO: consider the use of clock_nanosleep() */
	/* FIXME: maybe this function should call itself recursively
	 to make sure that the pause is fully ended, instead of checking only once.. */
	// NOTE: this struct declaration should maybe be somewhere else
	struct timespec ts = { .tv_sec = 1, .tv_nsec = 100 };
	int errCode = 0;

	if(toleratesInterrupt){
		struct timespec remainingTime;
		errCode = nanosleep(&ts, &remainingTime);
		// if EINTR then the pause has been interrupted
		if(errCode == EINTR) {
			//TODO: log this !
			nanosleep(&remainingTime, NULL);
		}
	}
	return nanosleep(&ts, NULL);
}