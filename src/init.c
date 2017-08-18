#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "init.h"
#include "utils.h"
#ifdef WIRINGPI
	// #include "led.h" //NOTE: include this only if on RPi.
#endif

void init(GPS_actions_t *GPSHandler, FullGPSData* gpsData, zone_t* zone, log_master_t *logMaster, edge_t** edges){
	/**
	 * TODO: Error checking and rv
	 */

	initLogSystem(logMaster);
	GPS_init(GPSHandler);
	init_gps_data(&gpsData);
	find_mbr(zone);
	create_edges(zone, edges);

	#ifdef WIRINGPI
	init_platform_specific_modules();
	#endif
}

int parse_input_args(zone_t* zone, int argc, char** args){
	/**
	 * TODO: error checking
	 */
	if(argc < 2 || strstr(args[1], "-h") != NULL){
		display_help_message();
		return ARGV_ERROR;
	} else if (argc == 2){
		printf("Not enough arguments were given. %s\n", COMMON_ERR_STR);
		return ARGV_ERROR; // not enough args
	} else {
		if (strstr(args[1], "-i") != NULL){
			return init_geofence_from_argv(zone, argc, args);
		} else if (strstr(args[1], "-f") != NULL){
			return init_geofence_from_file(zone, args);
		} else {
			printf("Unknown argument \"%s\". %s\n", args[1], COMMON_ERR_STR);
			return ARGV_ERROR; // any other option is invalid.
		}
	}

	return ARGV_OK;
}

void display_help_message(void){
	printf("\n");
	printf("Usage: geofence [options] -i | -f [arguments] | [filename]\n\n");
	printf(" Note that the arguments should follow the specified order.\n\n");
	printf(" options:\n");
	printf("\t-h              Display this help message. This flag can't be used with other options\n\n");
	printf(" Valid Arguments:\n");
	printf("\t-i              Input of geofence coordinates will be inline,\n");
	printf("\t                i.e the coordinates will be in the [arguments] list,\n");
	printf("\t                and must follow the following format: \n");
	printf("\t                \t1st value - a floating point value for the height\n");
	printf("\t                \tof the geofence.\n");
	printf("\t                \tFollowing values - space separated lat,lon paires,\n");
	printf("\t                \t(with a ',' between the values). Up to a precision of 6 digits.\n");

	printf("\t-f              Input of geofence coordinates will be from a user-made file,\n");
	printf("\t                the file path (with name) should be passed in [filename].\n");
	printf("\t                See the geofence_input_instructions file instructions on\n");
	printf("\t                writing the file.\n");
	printf("\n");
}

#ifdef WIRINGPI
void init_platform_specific_modules(void){
	log_info(&logMaster, "Program detected to be run on ARM.");
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
		.latitude = 0.0f,
		.longitude = 0.0f,
		.lat = '\0',
		.lon = '\0',
		.alt = 0.0f,
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

	*gpsData = &tmp;
}

int init_geofence_from_argv(zone_t* zone, int argc, char** args){ 	

	if(!range_valid(atof(args[2]), MIN_ALT, MAX_ALT)){
		printf("Illegal geofence altitude value. %s\n", COMMON_ERR_STR);
		return ARGV_ERROR;
	} else if( argc <= 5){
		printf("Not enough data for geofence initialization. %s\n", COMMON_ERR_STR);
		return ARGV_ERROR;
	} else {
		for(int i = 3; i < argc; i++){
			if(strchr(args[i], ',') == NULL){
				printf("Unexpected argument \"%s\". %s\n", args[i], COMMON_ERR_STR);
				return ARGV_ERROR;
			}
		}
	}

	zone->alt = atof(args[2]);
	zone->numVertices = argc - 3;
	zone->vertices = malloc((zone->numVertices + 1) * sizeof(geo_point_t));
	for(int i = 3; i < argc; ++i){
		char* pa = strchr(args[i], ',') + 1; // go to second number of the pair (lat,lon)
		zone->vertices[i-3].lon = atof(args[i]);
		zone->vertices[i-3].lat = atof(pa);
	}
	zone->vertices[zone->numVertices] = zone->vertices[0];

	return ARGV_OK;
}


int init_geofence_from_file(zone_t* zone, char** args){

	/**
	 * TODO: proper error checking
	 * TODO: throw an error if geofence in file consists of less than 3 polygons.
	 * 		Add support for a circle geofence (essentialy a cylinder cuz height).
	 */

	FILE* argvInputFile = fopen(args[2], "r"); // declared extern in init.h
	
	if(argvInputFile == NULL){
		char errStr[60];
		sprintf(errStr, "Error: fopen() has failed. Couldn't find '%s'%s", args[2], strerror(errno));
		log_info(&logMaster, errStr);
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
	zone->vertices = malloc((zone->numVertices + 1) * sizeof(geo_point_t));
	//memset(zone->vertices, 0.0f, zone->numVertices * sizeof(geo_point_t)); NOTE: should memset ?
	geo_point_t tmp[(const unsigned int)zone->numVertices + 1];
	lineNumber++;

	while(fgets(line, MAX_LINE_LEN, argvInputFile) != NULL){
		lineNumber++;
		switch(lineNumber){
			case 2: {
				char* p = line;
				p = strchr(p, '=')+1;
				zone->alt = atof(p);
				break;
			}
			default: {
				tmp[lineNumber-3] = parse_line(line); // TODO: its TEMPORARY, that the points' data starts on line 3.
				break;
			}
		}
	}
	tmp[zone->numVertices] = tmp[0]; // close the circle.

	memcpy(zone->vertices, tmp, sizeof tmp);
	fclose(argvInputFile);
	return ARGV_OK;
}

geo_point_t parse_line(char* str){
	geo_point_t p = { .lon = atof(str), .lat = atof(strchr(str, ',') + 1) };
	return p;
}
