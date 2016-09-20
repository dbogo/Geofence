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
#include "logInterface.h"
#include "GPSInterface.h" // TODO: reconsider this hierarchy !!

#include "../libs/GPSDemo/src/gps_demo.h"
#include "../libs/RPiGPSDemo/src/rpi_gps_demo.h"
#include "serial/serialInterface.h"

#ifdef HARDWARE_RPI
	#include "pifaceCAD/cad_utils.h"
	#include "led.h"
	//printf("Including RPi-specific modules.\n");
#endif


int main(int argc, char** argv) {
	FullGPSData gpsData; /* stores every kind of data we may need that's possible to extract from NMEA */
	Zone_general zone;
	Edge* edges = NULL;

	if(parse_input_args(&zone, argc, argv) != ALL_ARGV_INIT_OK){
		return -1;
	}
	init(&gpsData, &zone, &logMaster, &edges);
	
	//TODO: maybe instead of nanosleep implement a way using signals..
	// see: http://stackoverflow.com/questions/36953010/using-signals-in-c-how-to-stop-and-continue-a-program-when-timer-ends?
	
	int fd = open_port();

	while (!suspend_loop(TIME_TO_WAIT_SEC, TIME_TO_WAIT_NSEC)) {

		getGPSSample(fd, &gpsData, true);
		
		printf("lon: %f, lat %f\n", gpsData.latitude, gpsData.longitude);
		
		// whether currently in border
		if(wn_PnPoly(&gpsData, &zone, edges) > 0){
			printf("Current pos - within border\n");
		} else {
			printf("Current pos - outside the border\n");
		}	

		printf("-------------------------------\n");
	}

	finiLogSystem();
	
	return (0);
}
