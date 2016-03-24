/* 
 * File:   main.c
 * Author: bogo
 */

#include <stdio.h>
#include "main.h"

#include "GPSDemo/gps_demo.h"
#include "RPiGPSDemo/rpi_gps_demo.h"

int main(int argc, char** argv) {
	
	//TODO: timer
	
	//GPSSamp sample = getGPSSample();
	
	//GPSSamp sample;
	//RPiGetGPSSample(&sample, nmea, false, false);
	
	GPSSamp* sample = RPiGetGPSSample();
	
	printf("sample is at : %d (%X)\n", sample);
	
	if(sample == NULL){
		printf("\nUnrecognized NMEA format !\n");
	} else {
		printf("\nsample:\nlat:%f\nlong:%f\nspd:%f\nalt:%f\ncrs:%f\n", sample->latitude,
		 sample->longitude, sample->speed, sample->altitude, sample->course);
	}
	
	Zone zone;
	zone.altitude = 35.5f;
	zone.p1.latitude = 50.0f;
	zone.p1.longitude = 70.0f;
	zone.p2.latitude = 65.0f;
	zone.p2.longitude = 90.0f;
	
	if(sampInRange(sample, &zone)){
		printf("\n\t--IN RANGE !--\n");
	} else {
		printf("\n\t--OUT OF RANGE !--\n");
	}
	return (0);
}


bool sampInRange(GPSSamp* samp, Zone* zone){
	// figure out if samp is within borders (including altitude)
	if(((samp->latitude <= max(zone->p1.latitude, zone->p2.latitude)) && (samp->latitude >= min(zone->p1.latitude, zone->p2.latitude)))
		&& ((samp->longitude <= max(zone->p1.longitude, zone->p2.longitude)) && (samp->longitude >= min(zone->p1.longitude, zone->p2.longitude)))
		&& (samp->altitude <= zone->altitude))
			return true;
	return false;
}

