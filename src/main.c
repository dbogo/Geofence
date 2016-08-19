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
//#include "../GPSDemo/src/gps_demo.h"
#include "../RPiGPSDemo/src/rpi_gps_demo.h"
#include "serial/serialInterface.h"
#include "pifaceCAD/cad_utils.h"

#if 0
void timer_handler(int signum){
	static int count = 0;
	printf("SIG: %d timer expired %d times\n", signum, ++count);
}
#endif

//TODO: error checking and return values...
void init(Zone_general* zone, Log_Master* logMaster){

	// hardcode some zone data for a quick test !
	zone->numVertices = 4;
	zone->altitude = 285.0f;
	GEO_Point p0 = { .longitude = 0.0f, .latitude = 0.0f};
	GEO_Point p1 = { .longitude = 10.0f, .latitude = 0.0f};
	GEO_Point p2 = { .longitude = 10.0f, .latitude = 10.0f};
	GEO_Point p3 = { .longitude = 0.0f, .latitude = 10.0f};
	
	//TODO: look for a neater/faster way to do this...
	GEO_Point verts[4] = { p0, p1, p2, p3 };
	zone->vertices = malloc(sizeof(verts));
	memcpy(zone->vertices, verts, 4 * sizeof(GEO_Point));

	//create_segments_of_zone(zone);

	initLogSystem(logMaster);

	//init the Piface Control & Display 
	//if(init_cad() == -1)
	//	printf("init cad: pifacecad_open() hasn't yielded a file descriptor for SPI transactions.\n");
}

int main(int argc, char** argv) {

	init(&zone, &logMaster);

	char logStr[80];
	time_t currentTimeSec = 0;

	int sampleStatus = 0; // invalid(9), gga(2), rmc(3), or full(1).

	//int sampleStatus = 0; // invalid(9), gga(2), rmc(3), or full(1).
	
	//TODO: maybe instead of nanosleep implement a way using signals..
	// see: http://stackoverflow.com/questions/36953010/using-signals-in-c-how-to-stop-and-continue-a-program-when-timer-ends?
	
	int fd = open_port();
	char buffer[100];
	memset(buffer, '\0', 100);

	while (!suspend_loop(false)) {

		currentTimeSec = time(NULL);
		sampleStatus = getGPSSample(fd, &sample, true);
		
		//fetch_sentence_from_gps(fd, buffer);
		
		//printf("%s", buffer);
		
		/*
		if(sampleStatus == REGISTERED_GGA){
			sprintf(buffer, "GGA, %f,\n%f", sample.latitude, sample.longitude);
			clear_cad();		
		} else if(sampleStatus == REGISTERED_RMC){
			sprintf(buffer, "RMC, %f,\n%f", sample.latitude, sample.longitude);
			clear_cad();		
		}

		print_to_cad(buffer);
		*/
		printf("lon: %f, lat %f\n", sample.latitude, sample.longitude);

		//printf("lat: %f, lon: %f, alt: %f, crs: %f, spd: %f\n", sample.latitude, 
		//	sample.longitude, sample.altitude, sample.course, sample.speed);

		sprintf(logStr, "@(%ld) lat: %f, lon: %f, alt: %f, crs: %f, spd: %f |", 
			(long)currentTimeSec, sample.latitude, sample.longitude, 
			sample.altitude, sample.course, sample.speed);



		// whether currently in border
		if(isSampleInRangeGeneral1(&zone, &sample)){
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
		logEvent(logStr, LOG4C_PRIORITY_INFO, INFO, &logMaster);

		memset(buffer, '\0', 100);
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
	struct timespec ts = { .tv_sec = 1 , .tv_nsec = 0 };
	int errCode = 0;
	
	//TODO: this should stay commented out until I figure out how to globalize logMaster
	// or something..
	//logEvent("suspend loop", LOG4C_PRIORITY_INFO, ERROR, &logMaster);

	if(toleratesInterrupt){
		struct timespec remainingTime;
		errCode = nanosleep(&ts, &remainingTime);
		// if EINTR then the pause has been interrupted
		if(errCode == EINTR) {
			logEvent("suspend_loop: nanosleep() has been interrupted and will now continue.",
						LOG4C_PRIORITY_ERROR, ERROR, &logMaster);
			nanosleep(&remainingTime, NULL);
		}
	}
	return nanosleep(&ts, NULL);
}
