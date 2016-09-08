// NOTE: maybe this file should be named differently !

#include "GPSInterface.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int create_edges(Zone_general* zone, Edge** edges){

	Edge tmp[zone->numVertices]; //dummy

	for(int i = 0; i < zone->numVertices; i++){
		tmp[i].p1 = zone->vertices[i % zone->numVertices];
		tmp[i].p2 = zone->vertices[(i+1) % zone->numVertices];
		//printf("E%d: (%lf, %lf), (%lf,%lf)\n", (int)(i % zone->numVertices), tmp[i].p1.longitude, tmp[i].p1.latitude,
		//													tmp[i].p2.longitude, tmp[i].p2.latitude);
	}												

	// move the dummy edges to where the pointer points.
	*edges = malloc(sizeof(tmp));
	memcpy(*edges, tmp, sizeof(tmp));

	for(int i = 0; i < zone->numVertices; i++){
		printf("E%d: (%lf, %lf), (%lf,%lf)\n", (int)(i % zone->numVertices), (*edges)[i].p1.longitude, (*edges)[i].p1.latitude,
														(*edges)[i].p2.longitude, (*edges)[i].p2.latitude);
	}	

	return zone->numVertices;
}

/*
1. an upward edge includes its starting endpoint, and excludes its final endpoint;
2. a downward edge excludes its starting endpoint, and includes its final endpoint;
3. horizontal edges are excluded
4. the edge-ray intersection point must be strictly right of the point P.
*/

inline int isLeft(GEO_Point p, Edge e){
	/* Cross Product. */
	return ((e.p2.longitude - e.p1.longitude)*(p.latitude - e.p1.latitude) - 
		(e.p2.latitude - e.p1.latitude)*(p.longitude - e.p1.longitude));
}

inline bool upwards_cross(Edge e, GEO_Point p){
	return (e.p1.latitude <= p.latitude) && (e.p2.latitude > p.latitude);
}

inline bool downwards_cross(Edge e, GEO_Point p){
	return (e.p2.latitude <= p.latitude);
}

int wn_PnPoly(FullGPSData* location, Zone_general* zone, Edge* edges){
	GEO_Point p = { .longitude = location->longitude, .latitude = location->latitude };
	int w = 0; // the winding number

	// NOTE: amount of edges = amount of vertices ! hence zone->numvertices
	for(int i = 0; i < zone->numVertices; i++){
		if(upwards_cross(edges[i], p)){
				if(isLeft(p, edges[i]) > 0)
					w++;
		} else if(downwards_cross(edges[i], p)){ 
			if(isLeft(p, edges[i]) < 0)
				w--;
		}
	}
	
	if(w > 0)
		return INSIDE;
	return OUTSIDE;
}

//int isDroneInRangeGeneral1(Zone_general* zone, FullGPSData* location){
	/*int counter = 0;
	int i;
	double xinters;
	GEO_Point p1,p2;
	// the test point
	GEO_Point p = {  .longitude = location->longitude, .latitude = location->latitude};
	
	p1 = zone->vertices[0];
	for (i = 1; i <= zone->numVertices; i++) {
		p2 = zone->vertices[i % zone->numVertices];
		if (p.latitude > min(p1.latitude, p2.latitude)){
			if(p.latitude <= max(p1.latitude, p2.latitude)) {
    			if (p.longitude <= min(p1.longitude,p2.longitude)) {
      				if (p1.latitude != p2.latitude) {
        				xinters = (p.latitude-p1.latitude) * (p2.longitude-p1.longitude) / (p2.latitude-p1.latitude) + p1.longitude;
        				if (p1.longitude == p2.longitude || p.longitude <= xinters)
          					counter++;
      				}
    			}
	  		}
		}
		p1 = p2;
	}
	
	if (counter % 2 == 0)
		return(OUTSIDE);
	return(INSIDE);
	*/
	//if(wn_PnPoly(location, zone) > 0)
	//	return INSIDE;
	//return OUTSIDE;
//}


#if 0
/* This function checks whether a point is inside an N-sides, general
	case polygon, using a version of the ray-casting algorithm.
	see: https://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html */
bool isDroneInRangeGeneral(GPSSamp* samp, Zone_general* zone_gen){
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
	GEO_Point p = { .latitude = estimatedNextSamp.latitude,
					.longitude = estimatedNextSamp.longitude };
	if(isDroneInRangeGeneral1(zone_gen, zone_gen->numVertices, &estimatedNextSamp)){	
		return false;
	}

	return true;
}

 #endif
