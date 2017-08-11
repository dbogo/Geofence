#include <time.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/time.h>

#include "init.h"
#include "types.h"
#include "utils.h"
#include "logInterface.h"
#include "GPSInterface.h" // TODO: reconsider this hierarchy !!
#include "autopilot_controller.h"
#include "serial/serialInterface.h"
#include "mavlink_interface/inc/interface.h"

#ifdef RPI_GPS
#include "RPiGPSDemo/rpi_gps_demo.h"
#else
#include "GPSDemo/gps_demo.h"
#endif

#ifdef RPI_ADDONS
	// #include "pifaceCAD/cad_utils.h"
	// #include "led.h"
#endif

#define UNKNOWN 0
#define IN_BORDER 1
#define OUT_OF_BORDER 2
#define RETURNING 3


// int test(){

// 	uint64_t last_gps = 0;

// 	while(true){

// 		if(get_time_usec() - last_gps > 1000){
// 			GPSHandler.getGPS(&gpsData, true, NULL);
// 			last_gps = get_time_usec();
// 		}

// 		write_gps_to_autopilot(&gpsData);
		
// 		if(geofence_breached(&gpsData, &zone)){
// 			if(!autopilot_control_status()){
// 				controller_take_control();
// 			}
// 			controller_stop_drone();
// 		} else {
// 			if(autopilot_control_status()){
// 				controller_release_control();
// 			}
// 		}

// 		if ()
		
// 		sleep(10000);
// 	}

// }

int main(int argc, char** argv) {
	int state = UNKNOWN;
	signal(SIGINT, quit_handler);
	FullGPSData gpsData; /* stores every kind of data we may need, that's possible to extract from NMEA */
	Zone_general zone; 
	Edge* edges = NULL;
	GPS_Actions GPSHandler;
	GEO_Point home;

	// deal with argv
	if(parse_input_args(&zone, argc, argv) != ARGV_OK){
		printf("error parsing arguments.\n");
		return -1; 
	}

	init(&GPSHandler, &gpsData, &zone, &logMaster, &edges);
	

	// while(!gpsData.latitude && !gpsData.longitude){
	// 	printf("At first GPS loop...\n");
	// 	GPSHandler.getGPS(&gpsData, true, NULL);
	// 	usleep(500000); //500ms
	// }

	home.latitude = gpsData.latitude;
	home.longitude = gpsData.longitude;

	// if(!geofence_breached(&gpsData, &zone)){
	// 	state = IN_BORDER;
	// } else {
	// 	state = OUT_OF_BORDER;
	// }

	//autopilot stuff
	autopilot_initialize();
	serial_start("/dev/ttyUSB0"); // TODO: receive from argv ?

	read_messages();
	autopilot_start();


	//gpsData.latitude = 13.0f;
	// takeover_control();
	
	// clock_t start, end;
	// double dt;
	// time_t commanderTimestamp;
	// int counter = 0;


	uint64_t last_gps = 0;

	while(true){

		if(get_time_usec() - last_gps > 1000){
			GPSHandler.getGPS(&gpsData, true, NULL);
			last_gps = get_time_usec();
		}

		write_gps_to_autopilot(&gpsData);
		
		if(geofence_breached(&gpsData, &zone)){

			if(!autopilot_control_status()){
				controller_take_control();
			}

			controller_stop_drone();
		
		} else {

			if(autopilot_control_status()){
				controller_release_control();

			}
		}

		
		sleep(10000);
	}





	// while (true) {

	// 	switch(state){
	// 		case IN_BORDER:{
	// 			// autopilot_write();
	// 			break;
	// 		}

	// 		case OUT_OF_BORDER:{
	// 			controller_take_control();
	// 			return_to_zone(home);
	// 			state = RETURNING;
	// 			break;
	// 		}

	// 		case RETURNING: {
	// 			if(geofence_breached(&gpsData, &zone)){
	// 				return_to_zone(home);
	// 			} else {
	// 				release_control();
	// 				state = IN_BORDER;
	// 			}
	// 			// if(in_radius(gpsData, dest, radius)){
	// 			// 	release_control();
	// 			// 	state = IN_BORDER;
	// 			// }
	// 			break;
	// 		}
	// 	}
	// 	start = clock();
		
	// 	autopilot_write();

	// 	write_gps_to_autopilot(&gpsData);
		
	// 	read_local_pos_ned();

	// 	//sample GPS apprx every 1 sec 
	// 	if(counter == 100){
	// 		GPSHandler.getGPS(&gpsData, true, NULL);
	// 		counter = 0;
	// 	}
		

	// 	if(counter == 10){
	// 	// 	takeover_control(&commanderTimestamp);
	// 	// 	release_control();
	// 	// 	counter = 0;
	// 	}
		
	// 	// printf("lon: %f, lat %f\n", gpsData.longitude, gpsData.latitude);
	// 	// write_gps_to_autopilot(&gpsData);
	// 	// print_global_pos_int();

	// 	// whether currently in border
	// 	if(!geofence_breached(&gpsData, &zone)){
	// 		printf("Current pos - within border\n");
	// 		#ifdef WIRINGPI
	// 			led(GEOFENCE_OK_LED, HIGH);
	// 		#endif
	// 	} else {
	// 		printf("Current pos - outside the border\n");
	// 		// commanderTimestamp = time(NULL);
	// 		// takeover_control(&commanderTimestamp);
	// 		#ifdef WIRINGPI
	// 			led(GEOFENCE_OK_LED, LOW);
	// 		#endif
	// 	}	

	// 	end = clock();
	// 	dt = (double)(end - start)/CLOCKS_PER_SEC;
	// 	// printf("--------------%f----------------\n", dt);

	// 	counter++;
	// 	usleep(10000); // 10ms
	// }

	finiLogSystem();
	
	return (0);
}

#ifdef DEBUG
void quit_handler( int sig ){
	printf("\n");
	printf("TERMINATING AT USER REQUEST\n");
	printf("\n");

	// autopilot interface
	handle_quit_autopilot();

	handle_quit_serial();
	exit(0);
}
#endif
