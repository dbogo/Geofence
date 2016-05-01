// NOTE: maybe this fyle should be named differently !

#include "GPSInterface.h"
#include <math.h>

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
