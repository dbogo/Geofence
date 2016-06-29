// NOTE: maybe this file should be named differently !

#include "GPSInterface.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>

int isSampleInRangeGeneral1(Zone_general* zone, int numVertices, GEO_Point p){
	//TODO: understand this stuff, delete the comment and commit
	int counter = 0;
	int i;
	double xinters;
	GEO_Point p1,p2;

	p1 = zone->vertices[0];
	for (i = 1; i <= numVertices; i++) {
		p2 = zone->vertices[i % numVertices];
		if (p.latitude > min(p1.latitude, p2.latitude) && p.latitude <= max(p1.latitude, p2.latitude)) {
    		if (p.longitude <= min(p1.longitude,p2.longitude)) {
      			if (p1.latitude != p2.latitude) {
        			xinters = (p.latitude-p1.latitude) * (p2.longitude-p1.longitude) / (p2.latitude-p1.latitude) + p1.longitude;
        			if (p1.longitude == p2.longitude || p.longitude <= xinters)
          				counter++;
      			}
    		}
	  	}
		
		p1 = p2;
	}

	if (counter % 2 == 0)
		return(OUTSIDE);
	return(INSIDE);
}


#if 0
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
#endif

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
	GEO_Point p = { .latitude = estimatedNextSamp.latitude,
					.longitude = estimatedNextSamp.longitude };
	if(isSampleInRangeGeneral1(zone_gen, zone_gen->numVertices, p)){	
		return false;
	}

	return true;
}
