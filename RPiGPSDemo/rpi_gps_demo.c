#include "rpi_gps_demo.h"
#include "nmea_examples.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


GPSSamp* RPiGetGPSSample(void){
	char* nmea1 = NMEA_GGA;
	//char* nmea2 = NMEA_RMC;
	
	GPSSamp* sample = malloc(sizeof(GPSSamp));
	
	if((strstr(nmea1, "$GPGGA") != NULL)){
		printf("got GGA !\n");
		gga ggaSamp;
		parse_gga(&ggaSamp, nmea1);
		sample->altitude = ggaSamp.altitude;
		sample->longitude = ggaSamp.longitude;
		sample->latitude = ggaSamp.latitude;
	}
	else if((strstr(nmea1, "$GPRMC") != NULL)){
		printf("got RMC !\n");
		rmc rmcSamp;
		parse_rmc(&rmcSamp, nmea1);
		sample->longitude = rmcSamp.longitude;
		sample->latitude = rmcSamp.latitude;
		sample->course = rmcSamp.course;
		sample->speed = rmcSamp.speed;
	} 
	else{
		//NOTE: think of a better way to warn about failure (memset)
		//char s[5];
		//memset(s, '\0', sizeof(s));
		printf("unrecognized NMEA format %.5s!\n", nmea1);
		return NULL;
		// FIXME: what to return here, if at all ?!?!?!
	}
	
	return sample;
}

/*
void RPiGetGPSSample(GPSSamp* sample, char* nmea, bool hasgga, bool hasrmc){
	// if the signal is GGA and we didn't register it yet
	if((strstr(nmea, "$GPGGA") != NULL) && (hasgga == false)){
		printf("gga !\n");
		gga ggaSamp;
		parse_gga(&ggaSamp, nmea);
		sample->altitude = ggaSamp.altitude;
		sample->longitude = ggaSamp.longitude;
		sample->latitude = ggaSamp.latitude;
		hasgga = true;
	} else if((strstr(nmea, "$GPRMC") != NULL) && (hasrmc == false)){
		printf("rmc !\n");
		rmc rmcSamp;
		parse_rmc(&rmcSamp, nmea);
		sample->longitude = rmcSamp.longitude;
		sample->latitude = rmcSamp.latitude;
		sample->course = rmcSamp.course;
		sample->speed = rmcSamp.speed;
		hasrmc = true;
	} else {
		//NOTE: think of a better way to warn about failure (memset)
		char s[5];
		memset(s, '\0', sizeof(s));
		printf("unrecognized NMEA format %s!\n", strncpy(s, nmea, 6));
		;
		// FIXME: what to return here, if at all ?!?!?!
	}
}*/

void parse_gga(gga* samp, char *nmea){
    char *p = nmea;
    p = strchr(p, ',')+1; //skip time
	
    p = strchr(p, ',')+1;
    samp->latitude = atof(p);

    p = strchr(p, ',')+1;
    switch (p[0]) {
        case 'N':
            samp->lat = 'N';
            break;
        case 'S':
            samp->lat = 'S';
            break;
        case ',':
            samp->lat = '\0';
            break;
    }

    p = strchr(p, ',')+1;
    samp->longitude = atof(p);

    p = strchr(p, ',')+1;
    switch (p[0]) {
        case 'W':
            samp->lon = 'W';
            break;
        case 'E':
            samp->lon = 'E';
            break;
        case ',':
            samp->lon = '\0';
            break;
    }

    p = strchr(p, ',')+1;
    samp->quality = (uint8_t)atoi(p);

    p = strchr(p, ',')+1;
    samp->satellites = (uint8_t)atoi(p);

    p = strchr(p, ',')+1;

    p = strchr(p, ',')+1;
    samp->altitude = atof(p);
	//printf("sampALT: %f \n",samp->altitude);
}

/*
void parse_rmc(rmc* samp, char* nmea){
	char *p = nmea;
	p = strchr(p, ',')+1; //skip time
	p = strchr(p, ',')+1; //skip status
	
	// this is precious!
	char tes[50];
    memset(tes, '\0', sizeof(tes));
    //strncpy(tes, p, m-p);
	printf("%f asdadawddw234wadsd\n", atof(strncpy(tes, p, m-p)));
}*/


void parse_rmc(rmc* samp, char *nmea){
    char *p = nmea;

    p = strchr(p, ',')+1; //skip time
    p = strchr(p, ',')+1; //skip status

    p = strchr(p, ',')+1;
    samp->latitude = atof(p);
    
	p = strchr(p, ',')+1;
    switch (p[0]) {
        case 'N':
            samp->lat = 'N';
            break;
        case 'S':
            samp->lat = 'S';
            break;
        case ',':
            samp->lat = '\0';
            break;
    }

    p = strchr(p, ',')+1;
    samp->longitude = atof(p);

    p = strchr(p, ',')+1;
    switch (p[0]) {
        case 'W':
            samp->lon = 'W';
            break;
        case 'E':
            samp->lon = 'E';
            break;
        case ',':
            samp->lon = '\0';
            break;
    }

    p = strchr(p, ',')+1;
    samp->speed = atof(p);

    p = strchr(p, ',')+1;
    samp->course = atof(p);
}