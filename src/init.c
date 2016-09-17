#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "init.h"
#include "utils.h"
#include "GPSInterface.h"
#include "led.h" //TODO: include this only if on RPi.

//TODO: error checking and return values...
void init(FullGPSData* gpsData, Zone_general* zone, Log_Master* logMaster, Edge** edges){

	initLogSystem(logMaster);

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

	gpsData = &tmp;

	create_edges(zone, edges);
	//find_mbr(zone);

	/* An arbitrary, somewhat close estimation of a standard line that describes a vertex in the log. */
	#define ZONE_STR_LINE_LENGTH 40

	char zone_str[ZONE_STR_LINE_LENGTH];
	logEvent("Initialized the following zone vertices: ", LOG4C_PRIORITY_INFO, INFO, logMaster);
	for(int i = 0; i < zone->numVertices; i++){
		sprintf(zone_str, "V%d: (%lf, %f)", (int)(i%zone->numVertices), 
					zone->vertices[i].longitude, zone->vertices[i].latitude);
		logEvent(zone_str, LOG4C_PRIORITY_INFO, INFO, logMaster);
		//memset(zone_str, '\0', ZONE_STR_LINE_LENGTH); NOTE: should memset ???
	}	

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

int parse_input_args(Zone_general* zone, int argc, char** args){
	/**
	 * TODO: Various error cheking and rv
	 */
	if(argc == 1){
		printf("Error: No files for were specified on input.\n");
		logEvent("Error: No files for were specified on input.\n", LOG4C_PRIORITY_ERROR, ERROR, &logMaster);
		return NO_ARGS;
	}

	FILE* fp = fopen(args[1], "r");
	if(fp == NULL){
		perror("fopen() has failed");
		return FOPEN_FAIL;
	}

	const int MAX_LINE_LEN = 25;
	char line[MAX_LINE_LEN];

	int lineNumber = 0;
	
	// get the first line with the number of verts.
	fgets(line, MAX_LINE_LEN, fp);
	char* p = line;
	p = strchr(p, '=')+1; // skip the '='
	zone->numVertices = atoi(p);
	zone->vertices = malloc(zone->numVertices * sizeof(GEO_Point));
	//memset(zone->vertices, 0.0f, zone->numVertices * sizeof(GEO_Point)); NOTE: should memset ?
	GEO_Point tmp[(const unsigned int)zone->numVertices];
	lineNumber++;

	while(fgets(line, MAX_LINE_LEN, fp) != NULL){
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

	memcpy(zone->vertices, tmp, sizeof tmp);
	fclose(fp);
	return ALL_ARGV_INIT_OK;
}

GEO_Point parse_line(char* str){
	GEO_Point p;
	p.longitude = atof(str);
	str = strchr(str, ',')+1;
	p.latitude = atof(str);
	return p;
}
