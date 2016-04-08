#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <inttypes.h>

#include "main.h"
#include "../RPiGPSDemo/src/rpi_gps_demo.h"
#include "logInterface.h"

//TODO: should not be here; fix hardcoding
Zone zone = {.altitude = 285.0f,
	.p1.latitude = 20.5f, .p1.longitude = 20.8f,
	.p2.latitude = 43.5f, .p2.longitude = 44.0f};


int main(int argc, char** argv) {
	
	FILE* fp;
	FILE* fp2;
	fp = fopen(OPERATION_LOG_FILE, "w");
	fp2 = fopen(NMEA_LOG_FILE, "w");

	log4c_category_t* operationLogger = NULL;
	log4c_category_t* nmeaLogger = NULL;
	
	char* operationLogInstanceName = "operationlog";
	char* nmeaLogInstanceName = "nmealog";

	initLogSystem(&operationLogger, operationLogInstanceName, OPERATION_LOG_FILE);
	initLogSystem(&nmeaLogger, nmeaLogInstanceName, NMEA_LOG_FILE);
	
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

			sprintf(logStr, "@(%ld) lat: %f, lon: %f, alt: %f, crs: %f, spd: %f |", 
				(long)currentTimeSec, sample.latitude, sample.longitude, 
				sample.altitude, sample.course, sample.speed);
			
			if (isSampleInRange(&sample, &zone)) {
				strcat(logStr, " IN RANGE\n");
			} else {
				strcat(logStr, " OUT OF RANGE\n");
			}
			
			//log the operation. (the data of a GPSSamp)
			logEvent(operationLogger, LOG4C_PRIORITY_INFO, logStr);
			startTimeSec = currentTimeSec;

			//log the NMEA sentence from GPS. FIXME: TEMPORARELY DOESN'T LOG THE SENTENCES.
			sprintf(logStr, "@(%ld), <temporary: nmea sentence place holder>", (long)currentTimeSec);
			logEvent(nmeaLogger, LOG4C_PRIORITY_INFO, logStr);
		}
	}

	fclose(fp);
	fclose(fp2);
	finiLogSystem();
	
	return (0);
}

#if 0
void handleEvents(void){
	if(getchar() == 'c'){
		printf("message\n");
	}
}
#endif

bool isSampleInRange(GPSSamp* samp, Zone* zone) {
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

