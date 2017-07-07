// NOTE: maybe this file should be named differently !

#include <math.h>
#include <string.h>

#include "GPSInterface.h"
#include "logInterface.h"
#include "serial/serialInterface.h"
#include <libs/RPiGPSDemo/src/rpi_gps_demo.h>
#include <libs/GPSDemo/src/gps_demo.h>


int GPS_init(GPS_Actions* gpsHandler){
	#ifdef GPS_RPI
		gpsHandler->getGPS = getGPSSample_RPI;
		logEvent("Running code for the RPi implementation.", LOG4C_PRIORITY_INFO, INFO, &logMaster);
		open_port(); // inits the file descriptor for gps serial communication
	#else
		// gpsHandler->getGPS = getGPSSample_DEMO;
		logEvent("Running code for the Demo implementation.", LOG4C_PRIORITY_INFO, INFO, &logMaster);
	#endif

	return 0;
}

double to_deg(double x){
	int rounded_deg_x = x/100; // get the 2 most significant numbers of the value
	double right_of_point = x/100 - rounded_deg_x; //get the value to the right of the decimal point.
	double deg = rounded_deg_x + (right_of_point * 100 / 60);
	printf("deg: %f ", deg);
	return deg;	                                              	        
}

bool geofence_breached(FullGPSData* location, Zone_general* zone){
	GEO_Point p = { .longitude = location->longitude, .latitude = location->latitude };


	#ifdef GPS_RPI // convert for use with real world coordinates. (deg)
		printf("HARDWARE - RPi.\n");
		p.longitude = to_deg(location->longitude); 
		p.latitude = to_deg(location->latitude);
	#endif

	/**
	 * TODO: differentiate between WGS and MSL.
	 * TODO: check for minimum altitude ?
	 */
	if(geofecnce_alt_check(zone, location->altitude) == GEOFENCE_ALT_BREACH){
		return true;
	}

	if(geofence_polygon_check(zone, p) == GEOFENCE_POLYGON_BREACH){
		return true;
	}

	return false;
}

int geofecnce_alt_check(Zone_general* zone, double altitude){
	if(altitude > zone->altitude){
		return GEOFENCE_ALT_BREACH;
	}
	return GEOFENCE_ALT_OK;
}

int geofence_polygon_check(Zone_general* zone, GEO_Point p){
	float w = 0; // the winding number
	
	for(size_t i = 0; i < zone->numVertices; i++){
		if((zone->vertices[i].latitude < p.latitude && zone->vertices[i+1].latitude >=p.latitude) ||
			(zone->vertices[i].latitude >= p.latitude && zone->vertices[i+1].latitude < p.latitude)){
				if(((det(zone->vertices[i], zone->vertices[i+1], p) > 0) && zone->vertices[i+1].latitude > zone->vertices[i].latitude) ||
					((det(zone->vertices[i], zone->vertices[i+1], p) < 0) && zone->vertices[i+1].latitude < zone->vertices[i].latitude)){
					if(zone->vertices[i+1].latitude > zone->vertices[i].latitude){
						w++;
					} else {
						w--;
					}
				}
			}
	}

	if (w == 0){
		return GEOFENCE_POLYGON_BREACH;
	}
	return GEOFENCE_POLYGON_OK;
}

inline float det(GEO_Point p1, GEO_Point p2, GEO_Point location){
	return (p1.longitude - location.longitude)*(p2.latitude - location.latitude)
		- (p2.longitude - location.longitude)*(p1.latitude - location.latitude);
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
	// logEvent("Initialized the following zone vertices: ", LOG4C_PRIORITY_INFO, INFO, &logMaster);
	for(size_t i = 0; i < zone->numVertices; i++){
		sprintf(zone_str, "V%d: (%lf, %f)", (int)(i%zone->numVertices), 
					zone->vertices[i].longitude, zone->vertices[i].latitude);
		// logEvent(zone_str, LOG4C_PRIORITY_INFO, INFO, &logMaster);
		//memset(zone_str, '\0', ZONE_STR_LINE_LENGTH); NOTE: should memset ???
	}	

	for(size_t i = 0; i < zone->numVertices; i++){
		printf("E%d: (%1.6lf, %1.6lf), (%1.6lf,%1.6lf)\n", (int)(i % zone->numVertices), (*edges)[i].p1.longitude,
						(*edges)[i].p1.latitude, (*edges)[i].p2.longitude, (*edges)[i].p2.latitude);
	}	

	return zone->numVertices;
}

/* NOTE: still relevant ? */
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
