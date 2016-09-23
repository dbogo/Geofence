/**
 * TODO: im implementing function pointers that point to the getGPSSample functions either of the
 * 		demo version or of the RPi library (depending on what is defined to gcc at compile time.)
 * 		then this pointer is init by #ifdefing, to the appropriate function.
 * 		
 * 		the problem is that the getGPSSample() function that was in GPSInterface.h, is now whithout
 * 		the file descriptor parameter. how can I get the fd in getGPSSample_RPI ?
 */

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
#include "serial/serialInterface.h"

#include "../libs/GPSDemo/src/gps_demo.h"
#include "../libs/RPiGPSDemo/src/rpi_gps_demo.h"

#ifdef HARDWARE_RPI
	#include "pifaceCAD/cad_utils.h"
	#include "led.h"
#endif


int main(int argc, char** argv) {
	FullGPSData gpsData; /* stores every kind of data we may need, that's possible to extract from NMEA */
	Zone_general zone; 
	Edge* edges = NULL;
	int (*getGPS)(FullGPSData*, bool) = NULL;

	if(parse_input_args(&zone, argc, argv) != ALL_ARGV_INIT_OK){
		return -1;
	}
	
	GPS_init(&getGPS);
	init(&gpsData, &zone, &logMaster, &edges);
	open_port();

	while (!suspend_loop(TIME_TO_WAIT_SEC, TIME_TO_WAIT_NSEC)) {

		getGPS(&gpsData, true);
		
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
