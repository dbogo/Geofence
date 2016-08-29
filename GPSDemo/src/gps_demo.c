#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gps_demo.h"
#include "../../src/utils.h"

  
/* creates random GPSSamp data for simulation. 
 directly sets samp value rather than generate a cusom NMEA sentence
 ** for that see RPiGPSDemo
*/
int getGPSSample(int fd, FullGPSData* samp, bool passToLog){
	//TODO: introduce some randomizing factor
	samp->spdKph = 2.5f;
	samp->course = 50.0f;

	samp->latitude = samp->latitude +  samp->spdKph * sin(samp->course * PI/180);
	samp->longitude = samp->latitude +  samp->spdKph * cos(samp->course * PI/180);
	samp->altitude = 280.0f;

	return FULL_SAMPLE;
}
