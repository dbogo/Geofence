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
 * Latitude = X
 * Longitude = Y
 */

Zone zone = {.altitude = 285.0f,
	.p1.latitude = 20.5f, .p1.longitude = 80.0f,
	.p2.latitude = 43.5f, .p2.longitude = 120.9f};

int main(int argc, char** argv) {

	//TODO: timer


	//GPSSamp sample = getGPSSample();

	//GPSSamp sample;
	//RPiGetGPSSample(&sample, nmea, false, false);

	GPSSamp* sample = NULL;

	//very basic loop with 1 sec interval
	long a = time(NULL);
	int b = 0;
	while (1) {
		b = time(NULL);
		if ((b - a) > 0) {
			sample = RPiGetGPSSample();
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

