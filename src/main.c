#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <sys/time.h>
#include <signal.h>

#include "init.h"
#include "types.h"
#include "utils.h"
#include "autopilot_controller.h"
#include "logInterface.h"
#include "GPSInterface.h" // TODO: reconsider this hierarchy !!
#include "serial/serialInterface.h"

#include <mavlink_interface/inc/interface.h>
#include <GPSDemo/src/gps_demo.h>
// #include <RPiGPSDemo/src/rpi_gps_demo.h>

#ifdef RPI_ADDONS
	// #include "pifaceCAD/cad_utils.h"
	// #include "led.h"
#endif


int main(int argc, char** argv) {
	FullGPSData gpsData; /* stores every kind of data we may need, that's possible to extract from NMEA */
	Zone_general zone; 
	Edge* edges = NULL;
	GPS_Actions GPSHandler;

	// deal with argv
	if(parse_input_args(&zone, argc, argv) != ARGV_OK){
		printf("error parsing argumnts.\n");
		return -1; 
	}
	
	init(&GPSHandler, &gpsData, &zone, &logMaster, &edges);
	
	//autopilot stuff
	// autopilot_initialize();
	// serial_start("/dev/ttyUSB0"); // TODO: receive from argv ?
	// read_messages();
	// autopilot_start();

	//gpsData.latitude = 13.0f;

	clock_t start, end;
	double dt;

	while (!suspend_loop(TIME_TO_WAIT_SEC, TIME_TO_WAIT_NSEC)) {
		start = clock();
 
		GPSHandler.getGPS(&gpsData, true, NULL);
		
		printf("lon: %f, lat %f\n", gpsData.longitude, gpsData.latitude);
		
		// whether currently in border
		if(!geofence_breached(&gpsData, &zone)){
			printf("Current pos - within border\n");
			#ifdef WIRINGPI
				led(GEOFENCE_OK_LED, HIGH);
			#endif
		} else {
			printf("Current pos - outside the border\n");
			takeover_control();
			#ifdef WIRINGPI
				led(GEOFENCE_OK_LED, LOW);
			#endif
		}	

		end = clock();
		dt = (double)(end - start)/CLOCKS_PER_SEC;
		printf("--------------%f----------------\n", dt);

	}

	finiLogSystem();
	
	return (0);
}
