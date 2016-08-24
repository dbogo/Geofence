#include "parser.h"
#include <string.h>

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
    }
    return UNRECOGNIZED_NMEA_FORMAT;    
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