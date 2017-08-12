// NOTE: maybe this file should be named differently !

#include <math.h>
#include <string.h>

#include "GPSInterface.h"
#include "logInterface.h"
#include "serial/serialInterface.h"

#ifdef RPI_GPS
#include "RPiGPSDemo/rpi_gps_demo.h"
#else
#include "GPSDemo/gps_demo.h"
#endif


int GPS_init(GPS_actions_t* gpsHandler){
	#ifdef RPI_GPS
		gpsHandler->getGPS = getGPSSample_RPI;
		log_info(&logMaster, "Running Code for the RPi implementation.");
		open_gps_port("/dev/ttyUSB0"); // inits the file descriptor for gps serial communication
	#else
		gpsHandler->getGPS = getGPSSample_DEMO;
		log_int(&logMaster, "Running code for the Demo implementation.");
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

bool geofence_breached(FullGPSData* location, zone_t* zone){
	geo_point_t p = { .lon= location->lon, .lat = location->lat };


	#ifdef RPI_GPS // convert for use with real world coordinates. (deg)
		// printf("HARDWARE - RPi.\n");
		p.lon= to_deg(location->lon); 
		p.lat = to_deg(location->lat);
		printf("\n");
	#endif

	/**
	 * TODO: differentiate between WGS and MSL.
	 * TODO: check for minimum altitude ?
	 */
	if(geofence_alt_check(zone, location->alt) == GEOFENCE_ALT_BREACH){
		return true;
	}

	if(geofence_polygon_check(zone, p) == GEOFENCE_POLYGON_BREACH){
		return true;
	}

	return false;
}

int geofence_alt_check(zone_t* zone, double altitude){
	if(altitude > zone->alt){
		return GEOFENCE_ALT_BREACH;
	}
	return GEOFENCE_ALT_OK;
}

int geofence_polygon_check(zone_t* zone, geo_point_t p){
	float w = 0; // the winding number
	
	for(size_t i = 0; i < zone->numVertices; i++){
		if((zone->vertices[i].lat < p.lat && zone->vertices[i+1].lat >=p.lat) ||
			(zone->vertices[i].lat >= p.lat && zone->vertices[i+1].lat < p.lat)){
				if(((det(zone->vertices[i], zone->vertices[i+1], p) > 0) && zone->vertices[i+1].lat > zone->vertices[i].lat) ||
					((det(zone->vertices[i], zone->vertices[i+1], p) < 0) && zone->vertices[i+1].lat < zone->vertices[i].lat)){
					if(zone->vertices[i+1].lat > zone->vertices[i].lat){
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

inline float det(geo_point_t p1, geo_point_t p2, geo_point_t location){
	return (p1.lon- location.lon)*(p2.lat - location.lat)
		- (p2.lon- location.lon)*(p1.lat - location.lat);
}

int create_edges(zone_t* zone, edge_t** edges){

	edge_t tmp[zone->numVertices]; //dummy

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
					zone->vertices[i].lon, zone->vertices[i].lat);
		// logEvent(zone_str, LOG4C_PRIORITY_INFO, INFO, &logMaster);
		//memset(zone_str, '\0', ZONE_STR_LINE_LENGTH); NOTE: should memset ???
	}	

	for(size_t i = 0; i < zone->numVertices; i++){
		printf("E%d: (%1.6lf, %1.6lf), (%1.6lf,%1.6lf)\n", (int)(i % zone->numVertices), (*edges)[i].p1.lon,
						(*edges)[i].p1.lat, (*edges)[i].p2.lon, (*edges)[i].p2.lat);
	}	

	return zone->numVertices;
}

/* NOTE: still relevant ? */
void find_mbr(zone_t* polygon){
	polygon->mbr.p1.lon=  polygon->vertices[0].lon;
	polygon->mbr.p2.lon= polygon->vertices[0].lon;
	polygon->mbr.p1.lat = polygon->vertices[0].lat;
	polygon->mbr.p1.lat = polygon->vertices[0].lat;

	for(size_t i = 0; i < polygon->numVertices; i++){
		if (polygon->vertices[i].lon< polygon->mbr.p1.lon)
			polygon->mbr.p1.lon= (polygon->vertices[i]).lon;
		else if(polygon->vertices[i].lon> polygon->mbr.p2.lon)
			polygon->mbr.p2.lon= (polygon->vertices[i]).lon;

		if (polygon->vertices[i].lat < polygon->mbr.p1.lat)
			polygon->mbr.p1.lat = (polygon->vertices[i]).lat;
		else if(polygon->vertices[i].lat > polygon->mbr.p2.lat)
			polygon->mbr.p2.lat = (polygon->vertices[i]).lat;
	}

	//TODO: log.
	printf("xmin:%f xmax%f\nYmin:%f Ymax:%f\n\n", polygon->mbr.p1.lon, polygon->mbr.p2.lon,
												 polygon->mbr.p1.lat, polygon->mbr.p2.lat);
}
