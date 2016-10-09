// NOTE: maybe this file should be named differently !

#include <math.h>
#include <string.h>

#include "GPSInterface.h"
#include "logInterface.h"
#include "serial/serialInterface.h"
#include "libs/RPiGPSDemo/src/rpi_gps_demo.h"
#include "libs/GPSDemo/src/gps_demo.h"

int GPS_init(GPS_Actions* gpsHandler){
	#ifdef GPS_RPI
		gpsHandler->getGPS = getGPSSample_RPI;
		open_port(); // inits the file descriptor for gps serial communication
		printf("uses RPi implementation.\n");
	#else
		gpsHandler->getGPS = getGPSSample_DEMO;
		printf("uses DEMO implementation\n");
	#endif

	return 0;
}

/*  FIXME: should work for every value (inc. < 1 etc). */
double to_deg(double x){
    double deg = 0.0f;
    double l = 0.0f;
    l = (int)x/100; // the whole part, floored 
    double rem = x/100 - l; // what's to the right of the dot after flooring
    double rem_deg = rem*100/60; // convert that to degrees.
    deg = l + rem_deg;
    printf("orig: %lf; floored: %lf; rem: %lf; rem_deg: %lf; res: %lf\n", x, l, rem, rem_deg, deg);
    return deg;
}

int wn_PnPoly(FullGPSData* location, Zone_general* zone, Edge* edges){
	GEO_Point p = { .longitude = location->longitude, .latitude = location->latitude };
	#ifdef HARDWARE_RPI // convert for use with real world coordinates. (deg)
		p.longitude = to_deg(location->longitude); 
		p.latitude = to_deg(location->latitude);
	#endif

	float w = 0; // the winding number
	
	for(size_t i = 0; i < zone->numVertices; i++){
		if((zone->vertices[i].latitude < p.latitude && zone->vertices[i+1].latitude >=p.latitude) ||
			(zone->vertices[i].latitude >= p.latitude && zone->vertices[i+1].latitude < p.latitude)){
				if(((det(zone->vertices[i], zone->vertices[i+1], location) > 0) && zone->vertices[i+1].latitude > zone->vertices[i].latitude) ||
					((det(zone->vertices[i], zone->vertices[i+1], location) < 0) && zone->vertices[i+1].latitude < zone->vertices[i].latitude)){
					if(zone->vertices[i+1].latitude > zone->vertices[i].latitude){
						w++;
					} else {
						w--;
					}
				}
			}
	}

	printf("wn: %f\n", w);
	return w;
}


float det(GEO_Point p1, GEO_Point p2, FullGPSData* location){
	return (p1.longitude - location->longitude)*(p2.latitude - location->latitude)
		- (p2.longitude - location->longitude)*(p1.latitude - location->latitude);
}

int create_edges(Zone_general* zone, Edge** edges){

	Edge tmp[zone->numVertices]; //dummy

	// create the edges in the dummy array
	for(size_t i = 0; i < zone->numVertices; i++){
		tmp[i].p1 = zone->vertices[i % zone->numVertices];
		tmp[i].p2 = zone->vertices[(i+1) % zone->numVertices];
	}												

	// move the dummy edges to the actual pointer.
	*edges = malloc(sizeof(tmp));
	memcpy(*edges, tmp, sizeof(tmp));

	/* An arbitrary, somewhat close estimation of a standard line that describes a vertex in the log. */
	const unsigned char ZONE_STR_LINE_LENGTH = 40;

	char zone_str[ZONE_STR_LINE_LENGTH];
	logEvent("Initialized the following zone vertices: ", LOG4C_PRIORITY_INFO, INFO, &logMaster);
	for(size_t i = 0; i < zone->numVertices; i++){
		sprintf(zone_str, "V%d: (%lf, %f)", (int)(i%zone->numVertices), 
					zone->vertices[i].longitude, zone->vertices[i].latitude);
		logEvent(zone_str, LOG4C_PRIORITY_INFO, INFO, &logMaster);
		//memset(zone_str, '\0', ZONE_STR_LINE_LENGTH); NOTE: should memset ???
	}	

	for(size_t i = 0; i < zone->numVertices; i++){
		printf("E%d: (%1.3lf, %1.3lf), (%1.3lf,%1.3lf)\n", (int)(i % zone->numVertices), (*edges)[i].p1.longitude,
						(*edges)[i].p1.latitude, (*edges)[i].p2.longitude, (*edges)[i].p2.latitude);
	}	

	return zone->numVertices;
}

/* TODO */
void find_mbr(Zone_general* polygon){
	polygon->mbr.p1.longitude =  polygon->vertices[0].longitude;
	polygon->mbr.p2.longitude = polygon->vertices[0].longitude;
	polygon->mbr.p1.latitude = polygon->vertices[0].latitude;
	polygon->mbr.p1.latitude = polygon->vertices[0].latitude;

	for(size_t i = 0; i < polygon->numVertices; i++){
		if (polygon->vertices[i].longitude < polygon->mbr.p1.longitude)
			polygon->mbr.p1.longitude = (polygon->vertices[i]).longitude;
		else if(polygon->vertices[i].longitude > polygon->mbr.p2.longitude)
			polygon->mbr.p2.longitude = (polygon->vertices[i]).longitude;

		if (polygon->vertices[i].latitude < polygon->mbr.p1.latitude)
			polygon->mbr.p1.latitude = (polygon->vertices[i]).latitude;
		else if(polygon->vertices[i].latitude > polygon->mbr.p2.latitude)
			polygon->mbr.p2.latitude = (polygon->vertices[i]).latitude;
	}

	//TODO: log.
	printf("xmin:%f xmax%f\nYmin:%f Ymax:%f\n\n", polygon->mbr.p1.longitude, polygon->mbr.p2.longitude,
												 polygon->mbr.p1.latitude, polygon->mbr.p2.latitude);
}
