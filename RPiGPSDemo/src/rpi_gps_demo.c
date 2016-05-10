#include "rpi_gps_demo.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


char* generate_nmea_sentence(void){
	srand(time(NULL));
	
	char* nmea = malloc(100 * sizeof(char)); // allocate a string of 100
	memset(nmea, '\0', sizeof(nmea));
	
	int r = rand() % 2;
	if(r == 0) strcat(nmea, "$GPGGA,");
	else strcat(nmea, "$GPRMC,");
	
	char str[100];
	sprintf(str, "%d,", rand());
	
	if(strncmp(nmea, "$GPGGA", 6) == 0){
		// 3. latitude can only be between 0 and 90, longitude between 0 and 180
		sprintf(str + strlen(str), "%f,N,", ((double)(rand() / ((double)((unsigned)RAND_MAX + 1))*0.9)) * 100);
		sprintf(str + strlen(str), "%f,E,", ((double)(rand() / ((double)((unsigned)RAND_MAX + 1))*0.9)) * 200);
		// 5. fix quality, num of tracked satellites
		sprintf(str + strlen(str), "%d,", (uint8_t)((rand()%8)+1)); // fix can be from 0-8 but 0 is invalid signal. dont bother right now
		sprintf(str + strlen(str), "%d,", (uint8_t)((rand()%9)+2));
		// 7. HDOP (for example between 0 and 30, (the more the worse!))
		sprintf(str + strlen(str), "%f,", ((double)(rand() / ((double)((unsigned)RAND_MAX + 1))*0.3)) * 100); // arbitrary number but no too big (24 sats in space at all)
		// 8. altitude above sea level
		sprintf(str + strlen(str), "%f,M,", ((double)(rand() / ((double)((unsigned)RAND_MAX + 1)))) * 1000); // arbitrary number but no too big (24 sats in space at all)
		// 9. height above geoid(-106 to 85), time since last DGPS update (sec), DGPS satation id, checksum
		sprintf(str + strlen(str), "%f,M,0,0,E*68", ((((double)(rand() / ((double)((unsigned)RAND_MAX + 1))*0.191)) * 1000) - 106)); // arbitrary number but no too big (24 sats in space at all)
		strcat(nmea, str);
	} else {
		sprintf(str + strlen(str), "A, %f,N,", ((double)(rand() / ((double)((unsigned)RAND_MAX + 1))*0.9)) * 100);
		sprintf(str + strlen(str), "%f,E,", ((double)(rand() / ((double)((unsigned)RAND_MAX + 1))*0.9)) * 200);
		sprintf(str + strlen(str), "%f,", ((double)(rand() / ((double)((unsigned)RAND_MAX + 1))*0.55)) * 100);
		sprintf(str + strlen(str), "%f,230394,3.1,W*68", ((double)(rand() / ((double)((unsigned)RAND_MAX + 1))*0.36)) * 1000);
		strcat(nmea, str);
	}
	
	printf("%s\n", nmea);
	//free(nmea); NOTE: should be freed or not ?
	return nmea;
}

//NOTE: passToLog
int getGPSSample(GPSSamp* samp, bool passToLog){
	//TODO: somehow make this nmea sentence be passed outside,
    // where I need it to be looged in a file.
    char* nmea = generate_nmea_sentence();
    
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
	free(nmea);
	return UNRECOGNIZED_NMEA_FORMAT;	
}

#if 0
char* saveNMEAforLog(void){
    return nmea;
}
#endif

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