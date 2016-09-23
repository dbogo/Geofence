#include <stdio.h>
#include <math.h>

#include "gps_demo.h"
#include "../../../src/utils.h"
#include "../../../src/logInterface.h"
  
/* creates random GPSSamp data for simulation. 
 directly sets samp value rather than generate a cusom NMEA sentence
 ** for that see RPiGPSDemo
*/
int getGPSSample_DEMO(FullGPSData* samp, bool passToLog){
	//TODO: introduce some randomizing factor
	samp->spdKph = 1.34f;
	samp->course = 50.0f;

	samp->latitude = samp->latitude +  samp->spdKph * sin(samp->course * PI/180);
	samp->longitude = samp->latitude +  samp->spdKph * cos(samp->course * PI/180);
	samp->altitude = 280.0f;

	char logStr[120];
	sprintf(logStr, "DEMO: lat: %f, lon: %f, alt: %f, crs: %f, spd(kph): %f |", samp->latitude, samp->longitude, 
			samp->altitude, samp->course, samp->spdKph);

	logEvent(logStr, LOG4C_PRIORITY_INFO, INFO, &logMaster);
	return FULL_SAMPLE;
}
