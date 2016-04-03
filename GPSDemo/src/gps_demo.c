#include <stdio.h>
#include <stdlib.h>
//#include "gps_demo.h"
#include "../../src/GPSInterface.h"

  
// gets info from GPS
int getGPSSample(GPSSamp* samp){
	static double a = 0.0f;
	
	samp->altitude = 285.0f + a;
	samp->latitude = 20.55f + a;
	samp->longitude = 43.52f + a;
	samp->speed = 10.0f;
	samp->course = 60.5f;
	
	a += 0.05;
	return FULL_SAMPLE;
}