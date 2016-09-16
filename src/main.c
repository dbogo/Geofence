#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#include <sys/time.h>
#include <signal.h>

#include "main.h"
#include "GPSInterface.h" // TODO: reconsider this hierarchy !!
#include "../GPSDemo/src/gps_demo.h"
#include "../RPiGPSDemo/src/rpi_gps_demo.h"
#include "serial/serialInterface.h"
#include "pifaceCAD/cad_utils.h"
#include "led.h"

#if 0
void timer_handler(int signum){
	static int count = 0;
	printf("SIG: %d timer expired %d times\n", signum, ++count);
}
#endif

//TODO: error checking and return values...
void init(FullGPSData* gpsData, Zone_general* zone, Log_Master* logMaster, Edge** edges){

	FullGPSData tmp = {
		.latitude = 0.0f,
		.longitude = 0.0f,
		.lat = '\0',
		.lon = '\0',
		.altitude = 0.0f,
		.course = 0.0f,
		.spdKph = 0.0f,
		.quality = 0,
		.satellites = 0,
		.fixType = 0,
		.fixTime = 0,
		.pdop = 0.0f,
		.hdop = 0.0f,
		.vdop = 0.0f,
		.spdKnots = 0.0f,
		.status = false
	};

	gpsData = &tmp;

	// hardcode some zone data for a quick test !
	zone->numVertices = 4;
	zone->altitude = 285.0f;
	GEO_Point p0 = { .longitude = 0.1f, .latitude = 0.1f};
	GEO_Point p1 = { .longitude = 10.0f, .latitude = 0.0f};
	GEO_Point p2 = { .longitude = 10.0f, .latitude = 10.0f};
	GEO_Point p3 = { .longitude = 0.0f, .latitude = 10.0f};
	
	//TODO: look for a neater/faster way to do this...
	GEO_Point verts[4] = { p0, p1, p2, p3 };
	zone->vertices = malloc(sizeof(verts));
	memcpy(zone->vertices, verts, sizeof(verts));

	create_edges(zone, edges);
	find_mbr(zone);

/* An arbitrary, somewhat close estimation of a standard line that describes the vertices in the log. */
#define ZONE_STR_LINE_LENGTH 65

	char zone_str[ZONE_STR_LINE_LENGTH * zone->numVertices];
	int length = 0;

	for(int i = 0; i < zone->numVertices; i++){
		length += sprintf((zone_str + length), "\nV%d: (%lf,%lf), (%lf,%lf)", (int)(i % zone->numVertices), 
			(*edges)[i].p1.longitude, (*edges)[i].p1.latitude, (*edges)[i].p2.longitude, (*edges)[i].p2.latitude);
	}	
	
	initLogSystem(logMaster);
	
	logEvent("Initialized the following zone vertices: ", LOG4C_PRIORITY_INFO, INFO, logMaster);
	logEvent(zone_str, LOG4C_PRIORITY_INFO, INFO, logMaster);
	

	if(identify_platform() == ARM){
		logEvent("Program detected to be run on ARM.", LOG4C_PRIORITY_INFO, INFO, logMaster);
		init_wiringPi();
		set_led_output(STATUSLED);
		set_led_output(GEOFENCE_OK_LED);

		//init the Piface Control & Display 
		//if(init_cad() == -1)
		//	printf("init cad: pifacecad_open() hasn't yielded a file descriptor for SPI transactions.\n");
	}
}

int main(int argc, char** argv) {

	parse_input_args(argc, argv);


	FullGPSData gpsData; /* stores every kind of data we may need that's possible to extract from NMEA */
	Zone_general zone;
	Edge* edges = NULL;

	init(&gpsData, &zone, &logMaster, &edges);

	char logStr[80];
	time_t currentTimeSec = 0;
	int sampleStatus = 0; // see GPSInterface.h
	
	//TODO: maybe instead of nanosleep implement a way using signals..
	// see: http://stackoverflow.com/questions/36953010/using-signals-in-c-how-to-stop-and-continue-a-program-when-timer-ends?
	
	int fd = open_port();

	while (!suspend_loop(TIME_TO_WAIT_SEC, TIME_TO_WAIT_NSEC)) {

		currentTimeSec = time(NULL);
		sampleStatus = getGPSSample(fd, &gpsData, true);
		
		printf("lon: %f, lat %f\n", gpsData.latitude, gpsData.longitude);
		//sprintf(logStr, "@(%ld) lat: %f, lon: %f, alt: %f, crs: %f, spd(kph): %f |",(long)currentTimeSec, gpsData.latitude, gpsData.longitude, 
		//	gpsData.altitude, gpsData.course, gpsData.spdKph);

		
		// whether currently in border
		if(wn_PnPoly(&gpsData, &zone, edges) > 0){
			printf("Current pos - within border\n");
		} else {
			printf("Current pos - outside the border\n");
		}
		

#if 0
		// whether estimated to go out in the next iteration of this loop.
		if(isDroneGoingOffBorder(&gpsData, &zone)){	
			printf("Next estimated pos - outside of border\n");
		} else {
			printf("Next estimated pos - within border\n");
		}
#endif		
		
		//log the operation. (timestamp and the data of a GPSSamp)
		logEvent(logStr, LOG4C_PRIORITY_INFO, INFO, &logMaster);

		printf("-------------------------------\n");
	}

	finiLogSystem();
	
	return (0);
}

int parse_input_args(int argc, char** args){
	if(argc == 1){
		printf("Error: No arguments were provided for geofence creation and no input files are listed.\n");
		logEvent("Error: No arguments were provided for geofence creation and no input files are listed.", 
					LOG4C_PRIORITY_ERROR, ERROR, &logMaster);
		return NO_ARGS;
	}

	if()

	int ch = 0;
	for(int i = 1; i < argc; i++){
		while(args[i][ch] != '\0'){
			
		}
	}
}
