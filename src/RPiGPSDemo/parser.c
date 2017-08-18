#include "parser.h"
#include <string.h>
#include <stdio.h>


int parse_nmea(char* sentence, full_gps_data_t* samp){
    if((strstr(sentence, "$GPGGA") != NULL)){
        gga ggaSamp;
        parse_gga(&ggaSamp, sentence);
        samp->alt = ggaSamp.altitude;
        samp->longitude = ggaSamp.longitude;
        samp->latitude = ggaSamp.latitude;
        samp->satellites = ggaSamp.quality;
        samp->satellites = ggaSamp.satellites;
        return REGISTERED_GGA;
    } else if((strstr(sentence, "$GPRMC") != NULL)){
        rmc rmcSamp;
        parse_rmc(&rmcSamp, sentence);
        samp->longitude = rmcSamp.longitude;
        samp->latitude = rmcSamp.latitude;
        samp->course = rmcSamp.course;
        samp->spdKnots = rmcSamp.speed; //rmc gives Knots
        return REGISTERED_RMC;
    } else if((strstr(sentence, "$GPGSA") != NULL)){
    	gsa gsaSamp;
    	parse_gsa(&gsaSamp, sentence);
    	samp->fixType = gsaSamp.fixType;
    	samp->pdop = gsaSamp.pdop;
    	samp->hdop = gsaSamp.hdop;
    	samp->vdop = gsaSamp.vdop;
    	return REGISTERED_GSA;
    } else if((strstr(sentence, "$GPGLL") != NULL)){
    	gll gllSamp;
    	parse_gll(&gllSamp, sentence);
    	samp->latitude = gllSamp.latitude;
    	samp->lat = gllSamp.lat;
    	samp->longitude = gllSamp.longitude;
    	samp->lon = gllSamp.lon;
    	samp->fixTime = gllSamp.fixTime;
    	samp->status = gllSamp.status;
    	return REGISTERED_GLL;
    } else if((strstr(sentence, "$GPVTG") != NULL)){
    	vtg vtgSamp;
    	parse_vtg(&vtgSamp, sentence);
    	samp->spdKnots = vtgSamp.spdKnots;
    	samp->spdKph = vtgSamp.spdKph;
    	return REGISTERED_VTG;
    }

    return UNRECOGNIZED_NMEA;    
}

int validate_checksum(char* nmea){
    char* p = nmea;
    p++; // skip $
    unsigned short checksum = 0x0;

    while(*p != '*'){
        checksum ^= *p;
        p++;
    }

    p++; // skip the '*'. now at checksum of nmea
    char checksum_str[2]; // only need 2 digits. no <CR><LF>
    snprintf(checksum_str, 3, "%s", p); // string's length will be (n-1).


    if(!strncmp(checksum_str, nmea+(p-nmea), 2))
        return CHECKSUM_OK;
    
    return CHECKSUM_ERR;
}


void parse_gsa(gsa* gsa, char* nmea){
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


void parse_gll(gll* samp, char* nmea){
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

void parse_vtg(vtg* samp, char* nmea){
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
void parse_gga_new(gga* samp, char *nmea){
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
void parse_rmc_new(rmc* samp, char *nmea){
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
}

//registers lat, lon, spd, course
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

bool is_nmea_txt(char* nmea){
	// true if this is a $GPTXT sentence.
	if(strstr(nmea, "$GPTXT") != NULL)
		return true;
	return false;
}
