/* 
 * File:   main.c
 * Author: bogo
 */

#include <stdio.h>
#include "main.h"
#include <stdlib.h>
#include "GPSDemo/gps_demo.h"
#include "RPiGPSDemo/rpi_gps_demo.h"

#include <time.h>
/*
 * Latitude = Y (0-90)
 * Longitude = X (0-180)
 */

/*
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

	//TODO: timer

	GPSSamp* sample = NULL;

	//very basic loop with 1 sec interval
	long a = time(NULL);
	int b = 0;
	while (1) {
		b = time(NULL);
		if ((b - a) > 0) {
			//sample = RPiGetGPSSample();
			sample = getGPSSample();
			printf("lat: %f, lon: %f, alt: %f, crs: %f, spd: %f\n", sample->latitude,
				sample->longitude, sample->altitude, sample->course, sample->speed);

			if (sampInRange(sample, &zone)) {
				printf("\n\t--IN RANGE !--\n");
			} else {
				printf("\n\t--OUT OF RANGE !--\n");
			}
			a = b;
		}
	}

	/*
	if(sample == NULL){
		printf("\nUnrecognized NMEA format !\n");
	} else {
		printf("\nsample:\nlat:%f\nlong:%f\nspd:%f\nalt:%f\ncrs:%f\n", sample->latitude,
		 sample->longitude, sample->speed, sample->altitude, sample->course);
	}*/

	if (sampInRange(sample, &zone)) {
		printf("\n\t--IN RANGE !--\n");
	} else {
		printf("\n\t--OUT OF RANGE !--\n");
	}
	return (0);
}

bool sampInRange(GPSSamp* samp, Zone* zone) {
	// figure out if samp is within borders (including altitude)
	if (((samp->latitude <= max(zone->p1.latitude, zone->p2.latitude)) && (samp->latitude >= min(zone->p1.latitude, zone->p2.latitude)))
			&& ((samp->longitude <= max(zone->p1.longitude, zone->p2.longitude)) && (samp->longitude >= min(zone->p1.longitude, zone->p2.longitude)))
			&& (samp->altitude <= zone->altitude))
		return true;
	return false;
}

