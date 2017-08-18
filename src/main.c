#include <math.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

#include "init.h"
#include "autopilot_controller.h"
#include <mavlink_interface/interface.h>

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


int main(int argc, char** argv) {
	signal(SIGINT, quit_handler);
	full_gps_data_t gpsData; /* stores every kind of data we may need, that's possible to extract from NMEA */
	zone_t zone; 
	edge_t* edges = NULL;
	GPS_actions_t GPSHandler;

	if(parse_input_args(&zone, argc, argv) != ARGV_OK){
		printf("error parsing arguments.\n");
		return -1; 
	}

	init(&GPSHandler, &gpsData, &zone, &logMaster, &edges);
	log_info("Init Done.");

	// while(!gpsData.latitude && !gpsData.longitude){
	// 	printf("At first GPS loop...\n");
	// 	GPSHandler.getGPS(&gpsData, true, NULL);
	// 	usleep(500000); //500ms
	// }


	// if(!geofence_breached(&gpsData, &zone)){
	// 	state = IN_BORDER;
	// } else {
	// 	state = OUT_OF_BORDER;
	// }

	//autopilot stuff
	autopilot_initialize();
	#ifdef __arm__
	open_telem_port("/dev/ttyAMA0"); // TELEM2 is connected to RPi's GPIO
	#else
	open_telem_port("/dev/ttyUSB0"); // TELEM2 is connected via FTDI cable.
	#endif

	read_messages();
	autopilot_start();

	uint64_t last_gps = 0;

	while(true){

		if(get_time_usec() - last_gps > 1000000){
			GPSHandler.getGPS(&gpsData, true, NULL);
			last_gps = get_time_usec();
		}

		autopilot_write_gps(&gpsData);
		
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

		usleep(10000);
	}

	finiLogSystem();
	
	return 0;
}

#ifdef DEBUG
void quit_handler(){
	printf("\nTERMINATING AT USER REQUEST\n\n");
	handle_quit_autopilot();
	handle_quit_serial();
	exit(0);
}
#endif
