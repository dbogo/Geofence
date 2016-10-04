// NOTE: maybe this file should be named differently !

#include <math.h>
#include <string.h>

#include "GPSInterface.h"
#include "libs/RPiGPSDemo/src/rpi_gps_demo.h"
#include "libs/GPSDemo/src/gps_demo.h"

int GPS_init(GPS_Actions* gpsHandler){
	#ifdef GPS_RPI
		gpsHandler->getGPS = getGPSSample_RPI;
		printf("uses RPi implementation.\n");
	#else
		gpsHandler->getGPS = getGPSSample_DEMO;
		printf("uses DEMO implementation\n");
	#endif

	return 0;
}

int wn_PnPoly(FullGPSData* location, Zone_general* zone, Edge* edges){
	
	GEO_Point p = { .longitude = location->longitude, .latitude = location->latitude };
	float w = 0; // the winding number
	
	for(int i = 0; i < zone->numVertices; i++){
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

#if 0
float det(Edge* e, FullGPSData* location){
	return (e->p1.longitude - location->longitude)*(e->p2.latitude - location->latitude)
		- (e->p2.longitude - location->longitude)*(e->p1.latitude - location->latitude);
}
#endif

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

	for(size_t i = 0; i < zone->numVertices; i++){
		printf("E%d: (%1.3lf, %1.3lf), (%1.3lf,%1.3lf)\n", (int)(i % zone->numVertices), (*edges)[i].p1.longitude,
						(*edges)[i].p1.latitude, (*edges)[i].p2.longitude, (*edges)[i].p2.latitude);
	}	

	return zone->numVertices;
}

/* TODO */
void find_mbr(Zone_general* polygon){
	polygon->mbr.p1.longitude = polygon->mbr.p2.longitude = polygon->vertices[0].longitude;
	polygon->mbr.p1.latitude = (polygon->mbr.p1.latitude = polygon->vertices[0].latitude);

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
