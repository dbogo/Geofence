#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int parse_nmea(char* sentence, GPSSamp* samp){
    if((strstr(sentence, "$GPGGA") != NULL)){
        gga ggaSamp;
        parse_gga(&ggaSamp, sentence);
        samp->altitude = ggaSamp.altitude;
        samp->longitude = ggaSamp.longitude;
        samp->latitude = ggaSamp.latitude;
        samp->course = samp->speed = 0.0f; // temporary solution
        return REGISTERED_GGA;
    } else if((strstr(sentence, "$GPRMC") != NULL)){
        rmc rmcSamp;
        parse_rmc(&rmcSamp, sentence);
        samp->longitude = rmcSamp.longitude;
        samp->latitude = rmcSamp.latitude;
        samp->course = rmcSamp.course;
        samp->speed = rmcSamp.speed;
        samp->altitude = 0.0f; // temporary solution
        return REGISTERED_RMC;
    } else if((strstr(sentence, "$GPGSA") != NULL)){
    	gsa gsaSamp;
    	parse_gsa(&gsaSamp, sentence);
    	return REGISTERED_GSA;
    } else if((strstr(sentence, "$GPGLL") != NULL)){
    	gll gllSamp;
    	parse_gsa(&gllSamp, sentence);
    	return REGISTERED_GLL;
    } else if((strstr(sentence, "$GPVTG") != NULL)){
    	vtg vtgSamp;
    	parse_gsa(&vtgSamp, sentence);
    	return REGISTERED_VTG;
    
    return UNRECOGNIZED_NMEA;    
}

static void parse_gsa(gsa* gsa, char* nmea){
	char* p = nmea;
	p = strchr(p, ',')+1; 
	p = strchr(p, ',')+1;
	gsa->fixType = atoi(p);

	/* Now skip 12 fields of PRNs of satellites. */
	int i;
	for(i = 0; i < 12; i++){
		p = strchr(p, ',')+1;
	}

	p = strchr(p, ',')+1;
	gsa->pdop = atof(p);

	p = strchr(p, ',')+1;
	gsa->hdop = atof(p);

	p = strchr(p, ',')+1;
	gsa->vdop = atof(p);
}


static void parse_gll(gll* samp, char* nmea){
	char* p = nmea;
	p = strchr(p, ',')+1;

	samp->latitude = atof(p);

	p = strchr(p, ',')+1;
	samp->lat = p[0];

	p = strchr(p, ',')+1;
	samp->longitude = atof(p);

	p = strchr(p, ',')+1;
	samp->lon = p[0];

	p = strchr(p, ',')+1;
	samp->fixTime = atol(p);
}

static void parse_vtg(vtg* samp, char* nmea){
	char* p = nmea;
	p = strchr(p, ',')+1;
	p = strchr(p, ',')+1;
	p = strchr(p, ',')+1;
	p = strchr(p, ',')+1;

	p = strchr(p, ',')+1;
	samp->spdKnots = atof(p);

	p = strchr(p, ',')+1;

	p = strchr(p, ',')+1;
	samp->spdKph = atof(p);
}

//registers lat, lon, quality, satellites, altitude
static void parse_gga_new(gga* samp,cchar *nmea){
    char *p = nmea;
    p = strchr(p, ',')+1; //skip time
	
    p = strchr(p, ',')+1;
    samp->latitude = atof(p);

    p = strchr(p, ',')+1;
    samp->lat = p[0]; /* N, S or \0 */

    p = strchr(p, ',')+1;
    samp->longitude = atof(p);

    p = strchr(p, ',')+1;
    samp->lon = p[0]; /* W. E or \0 */

    p = strchr(p, ',')+1;
    samp->quality = (uint8_t)atoi(p);

    p = strchr(p, ',')+1;
    samp->satellites = (uint8_t)atoi(p);

    p = strchr(p, ',')+1;

    p = strchr(p, ',')+1;
    samp->altitude = atof(p);
}

//registers lat, lon, spd, course
static void parse_rmc_new(rmc* samp, char *nmea){
    char *p = nmea;

    p = strchr(p, ',')+1; //skip time
    p = strchr(p, ',')+1; //skip status

    p = strchr(p, ',')+1;
    samp->latitude = atof(p);
    
	p = strchr(p, ',')+1;
	samp->lat = p[0];

    p = strchr(p, ',')+1;
    samp->longitude = atof(p);

    p = strchr(p, ',')+1;
    samp->lon = p[0];

    p = strchr(p, ',')+1;
    samp->speed = atof(p);

    p = strchr(p, ',')+1;
    samp->course = atof(p);
}

//registers lat, lon, quality, satellites, altitude
static void parse_gga(gga* samp, char *nmea){
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
}

//registers lat, lon, spd, course
static void parse_rmc(rmc* samp, char *nmea){
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

bool is_nmea_txt(char* nmea){
	// true if this is a $GPTXT sentence.
	if(strstr(nmea, "$GPTXT") != NULL)
		return true;
	return false;
}
