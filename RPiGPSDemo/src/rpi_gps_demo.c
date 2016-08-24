#include "rpi_gps_demo.h"
#include "parser.h"
#include "../../src/serial/serialInterface.h"
#include "../../src/logInterface.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if 0
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
#endif

/* defined as extern in GPSInterface.h */
int getGPSSample(int fd, GPSSamp* samp, bool passToLog){

    char nmea[100];
	memset(nmea, '\0', 100);
	fetch_sentence_from_gps(fd, nmea);
    if(passToLog)
        logEvent(nmea, LOG4C_PRIORITY_INFO, INFO, &logMaster);    

	return parse_nmea(nmea, samp);	
}
