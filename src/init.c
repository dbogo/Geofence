#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "init.h"
#include "utils.h"
#ifdef WIRINGPI
	#include "led.h" //NOTE: include this only if on RPi.
#endif

void init(GPS_Actions* GPSHandler, FullGPSData* gpsData, Zone_general* zone, Log_Master* logMaster, Edge** edges){
	/**
	 * TODO: Error cheking and rv
	 */
	/* execute the script that makes NMEA output cleaner - without empty lines 
		after the sentence. same effect for NMEA log files. */
	system("./misc/serial_config.sh && cd ..");

	initLogSystem(logMaster);
	GPS_init(GPSHandler);
	init_gps_data(&gpsData);
	find_mbr(zone);
	create_edges(zone, edges);

	#ifdef WIRINGPI
	init_platform_specific_modules();
	#endif
}

int parse_input_args(Zone_general* zone, int argc, char** args){
	/**
	 * TODO: error cheking
	 */
	if(argc == 1){
		char errStr[60];
		strcpy(errStr,"Error: No files were specified on input.");
		printf("%s\n", errStr);
		logEvent(errStr, LOG4C_PRIORITY_ERROR, ERROR, &logMaster);
		return NO_ARGS;
	}else if(argc == 2)
		return init_geofence_from_file(zone, args);
	else // > 2
		return init_geofence_from_argv(zone, argc, args);
	

	return ALL_ARGV_INIT_OK;
}

#ifdef WIRINGPI
void init_platform_specific_modules(void){
	logEvent("Program detected to be run on ARM.", LOG4C_PRIORITY_INFO, INFO, &logMaster);
	init_wiringPi();
	set_led_output(STATUSLED);
	spet_led_output(GEOFENCE_OK_LED);
	//init the Piface Control & Display 
	//if(init_cad() == -1)
	//	printf("init cad: pifacecad_open() hasn't yielded a file descriptor for SPI transactions.\n");
}
#endif

void init_gps_data(FullGPSData** gpsData){
	FullGPSData tmp = {
		.latitude = 0.0f, .longitude = 0.0f,
		.lat = '\0', .lon = '\0',
		.altitude = 0.0f,
		.course = 0.0f, .spdKph = 0.0f,
		.quality = 0, .satellites = 0,
		.fixType = 0, .fixTime = 0,
		.pdop = 0.0f, .hdop = 0.0f, .vdop = 0.0f,
		.spdKnots = 0.0f,
		.status = false
	};

	*gpsData = &tmp;
}

int init_geofence_from_argv(Zone_general* zone, int argc, char** args){ 	
	/**
	 * TODO: error checking
	 * altitude is entered first. then the points.
	 */
	

	//FILE* argvStorageFile = fopen("data/geofence-argv.txt", 'w'); // file stores the parameters entered by the user.

	zone->altitude = atof(args[1]);
	zone->numVertices = argc - 2;
	zone->vertices = malloc((zone->numVertices + 1) * sizeof(GEO_Point));
	for(int i = 2; i < argc; ++i){
		char* pa = strchr(args[i], ',') + 1; // go to second number of the pair (lat,lon)
		zone->vertices[i-2].longitude = atof(args[i]);
		zone->vertices[i-2].latitude = atof(pa);
	}
	zone->vertices[zone->numVertices] = zone->vertices[0];

	return ALL_ARGV_INIT_OK;
}

int init_geofence_from_file(Zone_general* zone, char** args){

	/**
	 * TODO: proper error checking
	 * TODO: throw an error if geofence in file consists of less than 3 polygons.
	 * 		Add support for a circle geofence (essentialy a cylinder cuz height).
	 */

	FILE* argvInputFile = fopen(args[1], "r"); // declared extern in init.h
	
	if(argvInputFile == NULL){
			char errStr[60];
			perror("fopen() has failed");
			sprintf(errStr, "Error: fopen() has failed. Couldn't find '%s'.", args[1]);
			logEvent(errStr, LOG4C_PRIORITY_ERROR, ERROR, &logMaster);
			return FOPEN_FAIL;
	}

	const int MAX_LINE_LEN = 25;
	char line[MAX_LINE_LEN];

	int lineNumber = 0;
	
	// get the first line with the number of verts.
	fgets(line, MAX_LINE_LEN, argvInputFile);
	char* p = line;
	p = strchr(p, '=')+1; // skip the '='
	zone->numVertices = atoi(p);
	zone->vertices = malloc((zone->numVertices + 1) * sizeof(GEO_Point));
	//memset(zone->vertices, 0.0f, zone->numVertices * sizeof(GEO_Point)); NOTE: should memset ?
	GEO_Point tmp[(const unsigned int)zone->numVertices + 1];
	lineNumber++;

	while(fgets(line, MAX_LINE_LEN, argvInputFile) != NULL){
		lineNumber++;
		switch(lineNumber){
			case 2: {
				char* p = line;
				p = strchr(p, '=')+1;
				zone->altitude = atof(p);
				break;
			}default: {
				tmp[lineNumber-3] = parse_line(line); // TODO: its TEMPORARY, that the points' data starts on line 3.
				break;
			}
		}
	}
	tmp[zone->numVertices] = tmp[0]; // close the circle.

	memcpy(zone->vertices, tmp, sizeof tmp);
	fclose(argvInputFile);
	return ALL_ARGV_INIT_OK;
}

GEO_Point parse_line(char* str){
	GEO_Point p = { .longitude = atof(str), .latitude = atof(strchr(str, ',') + 1) };
	return p;
}
