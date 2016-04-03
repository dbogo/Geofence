#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <inttypes.h>

#include "main.h"
#include "../RPiGPSDemo/src/rpi_gps_demo.h"
#include "../log4c/include/log4c.h"

//#include "GPSInterface.h"


/*
					Latitude = Y (0-90)
					Longitude = X (0-180)

 						AT THE EQUATOR
		One degree of latitude =  110.57 km or  68.71 mi
		One minute of latitude =    1.84 km or   1.15 mi
		One second of latitude =   30.72 m  or 100.77 ft
		One degree of longitude = 111.32 km or  69.17 mi
		One minute of longitude =   1.86 km or   1.15 mi
		One second of longitude =  30.92 m  or 101.45 ft
 * 
						AT THE POLES
		One degree of latitude =  111.69 km or  69.40 mi
		One minute of latitude =    1.86 km or  1.16 mi
		One second of latitude =   31.03 m  or  101.79 ft
 */

Zone zone = {.altitude = 285.0f,
	.p1.latitude = 20.5f, .p1.longitude = 20.8f,
	.p2.latitude = 43.5f, .p2.longitude = 44.0f};

int main(int argc, char** argv) {
	
	FILE* fp;
	fp = fopen("log.txt", "w");
	
	log4c_category_t* logCat = NULL;
	
	if(log4c_init()){
		printf("lo4c_init() failed.\n");
	} else {
		logCat = log4c_category_get("geofence.src.logtest");
		log4c_category_set_appender(logCat, log4c_appender_get("log.txt"));
		log4c_category_log(logCat, LOG4C_PRIORITY_ERROR, "Hello World");
	}
	
	//cleanup
	if(log4c_fini()){
		printf("log4c_fini() failed.\n");
	}
	
	fclose(fp);
	
	//TODO: timer

	GPSSamp sample;
	int sampleStatus = 0; // invalid(9), gga(2), rmc(3), or full(1).
	
	//TODO: reconsider this busy loop design !!
	//very basic loop with 1 sec interval
	time_t startTimeSec = time(NULL);
	time_t currentTimeSec = 0;
	while (1) {
		
		
		
		
		//handleEvents();
		currentTimeSec = time(NULL);
		if ((currentTimeSec - startTimeSec) >= TIME_TO_WAIT_SEC) {
			sampleStatus = getGPSSample(&sample);
			
			printf("lat: %f, lon: %f, alt: %f, crs: %f, spd: %f\n", sample.latitude,
				sample.longitude, sample.altitude, sample.course, sample.speed);
			
			printf("%d ", sampleStatus);
			if (isSampleInRange(&sample, &zone)) {
				printf("\t--IN RANGE !--\n");
			} else {
				printf("\t--OUT OF RANGE !--\n");
			}
			
			startTimeSec = currentTimeSec;
			printf("\n");
		}
	}
	
	return (0);
}

void handleEvents(){
	if(getchar() == 'c'){
		printf("message\n");
	}
}

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

