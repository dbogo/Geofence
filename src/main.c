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
#include "../GPSDemo/src/gps_demo.h"
//#include "../RPiGPSDemo/src/rpi_gps_demo.h"

#if 0
void timer_handler(int signum){
	static int count = 0;
	printf("SIG: %d timer expired %d times\n", signum, ++count);
}
#endif


int main(int argc, char** argv) {
	running = true;
	Zone zone = {.altitude = 285.0f,
	.p1.latitude = 0.0f, .p1.longitude = 0.0f,
	.p2.latitude = 10.0f, .p2.longitude = 10.0f};
	
	FILE* fp;
	//FILE* fp2;
	fp = fopen(OPERATION_LOG_FILE, "w");
	//fp2 = fopen(NMEA_LOG_FILE, "w");

	log4c_category_t* operationLogger = NULL;
	//log4c_category_t* nmeaLogger = NULL;
	
	char* operationLogInstanceName = "operationlog";
	//char* nmeaLogInstanceName = "nmealog";

	initLogSystem(&operationLogger, operationLogInstanceName, OPERATION_LOG_FILE);
	//initLogSystem(&nmeaLogger, nmeaLogInstanceName, NMEA_LOG_FILE);

	GPSSamp sample;
	char logStr[80];
	//int sampleStatus = 0; // invalid(9), gga(2), rmc(3), or full(1).
	
	//TODO: TIMER (??) !!!!! reconsider this busy loop design !!
	
	/*
	time_t startTimeSec = time(NULL);
	time_t currentTimeSec = 0;
	*/


	/*
	// --- TEMPORARY ABANDONED THE USE OF SIGNALS...
	struct sigaction sa;
	struct itimerval timer;

	sa.sa_handler = &timer_handler;

	sigaction(SIGALRM, &sa, NULL);
	printf("%d\n\n\n", SIGALRM);

	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 500000;

	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 500000;

	//setitimer(ITIMER_REAL, &timer, NULL);
	*/

	/* TODO: wrapper around the nanosleep function !
		- should handle errors thrown by it,
		handle the second papameter (an event of pre-mature exit)
		hide those ugly struct declarations somewhere..
	*/
	struct timespec ts;
	ts.tv_sec = 3;
	ts.tv_nsec = 0;

	while (!nanosleep(&ts, NULL)) {
		
		//handleEvents();
		currentTimeSec = time(NULL);
		//if ((currentTimeSec - startTimeSec) >= TIME_TO_WAIT_SEC) {
			getGPSSample(&sample, true);

			/*
			printf("lat: %f, lon: %f, alt: %f, crs: %f, spd: %f\n", sample.latitude, 
				sample.longitude, sample.altitude, sample.course, sample.speed);

			printf("zone: lat1: %f lat2: %f lon1: %f lon2: %f\n", zone.p1.latitude, 
				zone.p2.latitude, zone.p1.longitude, pzone.p2.longitude);
			*/

			sprintf(logStr, "@(%ld) lat: %f, lon: %f, alt: %f, crs: %f, spd: %f |", 
				(long)currentTimeSec, sample.latitude, sample.longitude, 
				sample.altitude, sample.course, sample.speed);

			
			// whether currently in border
			if(isSampleInRange(&sample, &zone)){
				printf("Current pos - within border\n");
			} else {
				printf("Current pos - outside the border\n");
			}

			// whether estimated to go out in the next iteration of this loop.
			if(isDroneGoingOffBorder(&sample, &zone)){
				printf("Next estimated pos - outside of border\n");
			} else {
				printf("Next estimated pos - within border\n");
			}
			
			
			//log the operation. (timestamp and the data of a GPSSamp)
			logEvent(operationLogger, LOG4C_PRIORITY_INFO, logStr);
			startTimeSec = currentTimeSec;

			//log the NMEA sentence from GPS. FIXME: TEMPORARELY DOESN'T LOG THE SENTENCES.
			//sprintf(logStr, "@(%ld), <temporary: nmea sentence place holder>", (long)currentTimeSec);
			//logEvent(nmeaLogger, LOG4C_PRIORITY_INFO, logStr);
			printf("******************************\n");
		//}
	}

	fclose(fp);
	//fclose(fp2);
	finiLogSystem();
	
	return (0);
}

bool isSampleInRange(GPSSamp* samp, Zone* zone) {
	//The simplest check of a body within a rectangle
	if(samp->altitude > zone->altitude) 
		return false;

	if(samp->latitude >= max(zone->p1.latitude, zone->p2.latitude) || 
		samp->latitude <= min(zone->p1.latitude, zone->p2.latitude))
		return false;

	if(samp->longitude >= max(zone->p1.longitude, zone->p2.longitude) || 
		samp->longitude <= min(zone->p1.longitude, zone->p2.longitude))
		return false;

	return true;
}

bool isDroneGoingOffBorder(GPSSamp* samp, Zone* zone){

	//TODO: maybe estimate for more than 1 step ahead.
	//		introduce some margin for error
	//		* maybe alert levels, etc..

	GPSSamp estimatedNextSamp = {
		.altitude = samp->altitude,
		.latitude = samp->latitude + (samp->speed * sin(samp->course * PI/180)),
		.longitude = samp->longitude + (samp->speed * cos(samp->course * PI/180)),
		.course = samp->course,
		.speed = samp->speed };


	// check if the drone's next estimated position is within the borders 
	if(isSampleInRange(&estimatedNextSamp, zone)){	
		return false;
	}

	return true;
}


#if 0
//ear clipping
int findAnEar(Zone_general zGen, int p){
	if(isPointAnEar(p)){
		return p;
	}

	Zone_general GSP;
	yieldGSP(&GSP);
	
	findAnEar(GSP, floor(GSP.numVertices/2));
}


bool isInArbitraryZone(GPSSamp* samp, Zone_general* zone){
	Segment borderLines[zone->numVertices];
	for(int i = 0; i < (zone->numVertices - 1); i++){
		borderLines[i].p1.latitude = zone->vertices[i].latitude;
		borderLines[i].p1.longitude = zone->vertices[i].longitude;
		borderLines[i].p2.latitude = zone->vertices[i+1].latitude;
		borderLines[i].p2.longitude = zone->vertices[i+1].longitude;
	}

}
#endif


































