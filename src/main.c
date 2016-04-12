#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#include "main.h"
#include "../GPSDemo/src/gps_demo.h"
//#include "../RPiGPSDemo/src/rpi_gps_demo.h"
#include "logInterface.h"


int main(int argc, char** argv) {

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
	
	//TODO: timer

	GPSSamp sample;
	//int sampleStatus = 0; // invalid(9), gga(2), rmc(3), or full(1).
	
	//TODO: reconsider this busy loop design !!
	//very basic loop with 1 sec interval
	time_t startTimeSec = time(NULL);
	time_t currentTimeSec = 0;
	char logStr[80];

	while (1) {
		
		//handleEvents();
		currentTimeSec = time(NULL);
		if ((currentTimeSec - startTimeSec) >= TIME_TO_WAIT_SEC) {
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
		}
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

	//TODO: estimate for more than 1 step ahead.
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

