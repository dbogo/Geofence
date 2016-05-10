// NOTE: maybe this fyle should be named differently !

#include "GPSInterface.h"
#include <math.h>
#include <stdio.h>

/* This function checks whether a point is inside an N-sides, general
	case polygon, using a version of the ray-casting algorithm.
	see: https://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html */
bool isSampleInRangeGeneral(GPSSamp* samp, Zone_general* zone_gen){
	double vertx[zone_gen->numVertices];
	double verty[zone_gen->numVertices];
	
	int i;
	for(i = 0; i < zone_gen->numVertices; i++){
		vertx[i] = zone_gen->vertices[i].longitude;
		verty[i] = zone_gen->vertices[i].latitude;
	}

	if(pnpoly(zone_gen->numVertices, vertx, verty, samp->latitude, samp->longitude))
		return true; // if returned value is 1 (odd, then inside)
	return false;
}

int pnpoly(int nvert, double *vertx, double *verty, double testx, double testy){
	int i, j, c = 0;
  	for (i = 0, j = nvert-1; i < nvert; j = i++) {
    	if ( ((verty[i]>testy) != (verty[j]>testy)) &&
	 	(testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
       		c = !c; // c alternates between 1 and 0 - even and odd.
  	}
  	return c;
}


bool isDroneGoingOffBorder(GPSSamp* samp, Zone_general* zone_gen){

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
	if(isSampleInRangeGeneral(&estimatedNextSamp, zone_gen)){	
		return false;
	}

	return true;
}
