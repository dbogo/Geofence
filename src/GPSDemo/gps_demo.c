#include <stdio.h>
#include <math.h>

#include "gps_demo.h"
#include <src/utils.h>
#include <src/logInterface.h>
  
int getGPSSample_DEMO(FullGPSData* samp, bool passToLog){
	samp->spdKph = 1.34f;
	samp->course = 50.0f;

	samp->longitude = 6.7f;
	samp->latitude += 0.5f;
	//samp->latitude = samp->latitude +  samp->spdKph * sin(samp->course * PI/180);
	//samp->longitude = samp->latitude +  samp->spdKph * cos(samp->course * PI/180);
	samp->altitude = 280.0f;

	if(passToLog){
		char logStr[120];
		sprintf(logStr, "DEMO: lat: %f, lon: %f, alt: %f, crs: %f, spd(kph): %f",
				samp->latitude, samp->longitude, samp->altitude, samp->course, samp->spdKph);
		log_info(&logMaster, logStr);
	}
	return FULL_SAMPLE;
}
