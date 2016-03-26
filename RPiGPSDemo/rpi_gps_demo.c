#include "rpi_gps_demo.h"
//#include "nmea_examples.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


char* generate_nmea_sentence(void){
	srand(time(NULL));
	char* nmea = malloc(100 * sizeof(char)); // allocate string of 100
	memset(nmea, '\0', sizeof(nmea));
	int r = rand() % 2;
	if(r == 0) strcat(nmea, "$GPGGA,");
	else strcat(nmea, "$GPRMC,");
	
	// 2. append time (first convert to a string) NOTE: is there a nicer way to do this ?
	char str[30];
	sprintf(str, "%d,", rand());
	strcat(nmea, str);
	
	//continue to generate the correct type of nmea sentence
	if(strncmp(nmea, "$GPGGA", 6) == 0){
		// 3. latitude can only be between 0 and 90
		memset(str, '\0', sizeof(str));
		sprintf(str, "%f,N,", ((double)(rand() / ((double)((unsigned)RAND_MAX + 1))*0.9)) * 100);
		strcat(nmea, str);
		// 4. longitude can only be between 0 and 180
		memset(str, '\0', sizeof(str));
		sprintf(str, "%f,E,", ((double)(rand() / ((double)((unsigned)RAND_MAX + 1))*0.9)) * 200);
		strcat(nmea, str);
		// 5. fix quality
		memset(str, '\0', sizeof(str));
		sprintf(str, "%d,", (uint8_t)((rand()%8)+1)); // fix can be from 0-8 but 0 is invalid signal. dont bother right now
		strcat(nmea, str);
		// 6. num of tracked satellites
		memset(str, '\0', sizeof(str));
		sprintf(str, "%d,", (uint8_t)((rand()%9)+2)); // arbitrary number but no too big (24 sats in space at all)
		strcat(nmea, str);
		// 7. HDOP (for example between 0 and 30, (the more the worse!))
		memset(str, '\0', sizeof(str));
		sprintf(str, "%f,", ((double)(rand() / ((double)((unsigned)RAND_MAX + 1))*0.3)) * 100); // arbitrary number but no too big (24 sats in space at all)
		strcat(nmea, str);
		// 8. altitude above sea level
		memset(str, '\0', sizeof(str));
		sprintf(str, "%f,M,", ((double)(rand() / ((double)((unsigned)RAND_MAX + 1)))) * 1000); // arbitrary number but no too big (24 sats in space at all)
		strcat(nmea, str);
		// 9. height above gedid, time since last DGPS update (sec), DGPS satation id, checksum
		memset(str, '\0', sizeof(str));
		sprintf(str, "%f,M,0,0,E*68", ((((double)(rand() / ((double)((unsigned)RAND_MAX + 1))*0.191)) * 1000) - 106)); // arbitrary number but no too big (24 sats in space at all)
		strcat(nmea, str);
	} else {
		strcat(nmea, "A,"); //status
		// 4. latitude can only be between 0 and 90
		memset(str, '\0', sizeof(str));
		sprintf(str, "%f,N,", ((double)(rand() / ((double)((unsigned)RAND_MAX + 1))*0.9)) * 100);
		strcat(nmea, str);
		// 5. longitude can only be between 0 and 180
		memset(str, '\0', sizeof(str));
		sprintf(str, "%f,E,", ((double)(rand() / ((double)((unsigned)RAND_MAX + 1))*0.9)) * 200);
		strcat(nmea, str);
		//speed over the ground IN KNOTS
		memset(str, '\0', sizeof(str));
		sprintf(str, "%f,", ((double)(rand() / ((double)((unsigned)RAND_MAX + 1))*0.55)) * 100);
		strcat(nmea, str);
		// track angle in degrees True (course, thus 0-360), date, magnetic variation, and checksum, all not relevant for us
		memset(str, '\0', sizeof(str));
		sprintf(str, "%f,230394,3.1,W*68", ((double)(rand() / ((double)((unsigned)RAND_MAX + 1))*0.36)) * 1000);
		strcat(nmea, str);
	}
	return nmea;
}

int getGPSSample(GPSSamp* samp){
	char* nmea = generate_nmea_sentence(); // get randomly generated NMEA sentence
	
	if((strstr(nmea, "$GPGGA") != NULL)){
		gga ggaSamp;
		parse_gga(&ggaSamp, nmea);
		samp->altitude = ggaSamp.altitude;
		samp->longitude = ggaSamp.longitude;
		samp->latitude = ggaSamp.latitude;
		samp->course = samp->speed = 0.0f; // temporary solution
		return REGISTERED_GGA;
	} else if((strstr(nmea, "$GPRMC") != NULL)){
		rmc rmcSamp;
		parse_rmc(&rmcSamp, nmea);
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