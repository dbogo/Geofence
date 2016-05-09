#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#include <sys/time.h>
#include <signal.h>

#include "main.h"
#include "utils.h"
#include "logInterface.h"
#include "GPSInterface.h" // TODO: reconsider this hierarcy !!
#include "../GPSDemo/src/gps_demo.h"
//#include "../RPiGPSDemo/src/rpi_gps_demo.h"

#if 0
void timer_handler(int signum){
	static int count = 0;
	printf("SIG: %d timer expired %d times\n", signum, ++count);
}
#endif

int main(int argc, char** argv) {

	// hardcode some zone data for a quick test !
	Zone_general zone;
	zone.numVertices = 6;
	zone.altitude = 285.0f;
	GEO_Point p0 = { .latitude = 0.0f, .longitude = 0.0f};
	GEO_Point p1 = { .latitude = 10.0f, .longitude = 0.0f};
	GEO_Point p2 = { .latitude = 17.0f, .longitude = 7.5f};
	GEO_Point p3 = { .latitude = 17.0f, .longitude = 17.0f};
	GEO_Point p4 = { .latitude = 11.0f, .longitude = 21.0f};
	GEO_Point p5 = { .latitude = 0.0f, .longitude = 22.0f};
	
	zone.vertices[0] = p0;
	zone.vertices[1] = p1;
	zone.vertices[2] = p2;
	zone.vertices[3] = p3;
	zone.vertices[3] = p3;
	zone.vertices[4] = p4;
	zone.vertices[5] = p5;
	
	FILE* fp;
	//FILE* fp2;
	fp = fopen(OPERATION_LOG_FILE, "w");
	//fp2 = fopen(NMEA_LOG_FILE, "w");

	log4c_category_t* operationLogger = NULL;
	//log4c_category_t* nmeaLogger = NULL;
	
	char* operationLogInstanceName = "operationlog";
	//char* nmeaLogInstanceName = "nmealog";

	initLogSystem(&operationLogger, operationLogInstanceName, OPERATION_LOG_FILE);
	//initLogSystem(&nmeaLogger, nmeaLogInstanceName, NMEA_LOG_FILE);

	GPSSamp sample;
	char logStr[80];
	//int sampleStatus = 0; // invalid(9), gga(2), rmc(3), or full(1).
	
	//TODO: TIMER (??) !!!!! reconsider this busy loop design !!
	
	//time_t startTimeSec = time(NULL);
	time_t currentTimeSec = 0;
	
	
	/*
	// --- TEMPORARY ABANDONED THE USE OF SIGNALS...
	struct sigaction sa;
	struct itimerval timer;

	sa.sa_handler = &timer_handler;

	sigaction(SIGALRM, &sa, NULL);
	printf("%d\n\n\n", SIGALRM);

	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 500000;

	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 500000;

	//setitimer(ITIMER_REAL, &timer, NULL);
	*/

	/* TODO: wrapper around the nanosleep function !
		- should handle errors thrown by it,
		handle the second papameter (an event of pre-mature exit)
		hide those ugly struct declarations somewhere..
	*/
	struct timespec ts;
	ts.tv_sec = 1;
	ts.tv_nsec = 100;

	while (!nanosleep(&ts, NULL)) {
		
		currentTimeSec = time(NULL);
		//if ((currentTimeSec - startTimeSec) >= TIME_TO_WAIT_SEC) {
			getGPSSample(&sample, true);

			printf("lon: %f, lat %f\n", sample.latitude, sample.longitude);

			//printf("lat: %f, lon: %f, alt: %f, crs: %f, spd: %f\n", sample.latitude, 
			//	sample.longitude, sample.altitude, sample.course, sample.speed);

			sprintf(logStr, "@(%ld) lat: %f, lon: %f, alt: %f, crs: %f, spd: %f |", 
				(long)currentTimeSec, sample.latitude, sample.longitude, 
				sample.altitude, sample.course, sample.speed);

			
			// whether currently in border
			if(isSampleInRangeGeneral(&sample, &zone)){
				printf("Current pos - within border\n");
			} else {
				printf("Current pos - outside the border\n");
			}

#if 0
			// whether estimated to go out in the next iteration of this loop.
			if(isDroneGoingOffBorder(&sample, &zone)){
				printf("Next estimated pos - outside of border\n");
			} else {
				printf("Next estimated pos - within border\n");
			}
#endif		
			
			//log the operation. (timestamp and the data of a GPSSamp)
			logEvent(operationLogger, LOG4C_PRIORITY_INFO, logStr);
			//startTimeSec = currentTimeSec;

			//log the NMEA sentence from GPS. FIXME: TEMPORARELY DOESN'T LOG THE SENTENCES.
			//sprintf(logStr, "@(%ld), <temporary: nmea sentence place holder>", (long)currentTimeSec);
			//logEvent(nmeaLogger, LOG4C_PRIORITY_INFO, logStr);
			printf("******************************\n");
		//}
	}

	fclose(fp);
	//fclose(fp2);
	finiLogSystem();
	
	return (0);
}
