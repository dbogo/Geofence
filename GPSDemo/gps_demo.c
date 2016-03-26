#include <stdio.h>
#include <stdlib.h>
#include "gps_demo.h"


  
// gets info from GPS
GPSSamp* getGPSSample(){
	static double a = 0.0f;
	printf("%f\n", a);
	
	GPSSamp* samp = malloc(sizeof(GPSSamp));
	
	samp->altitude = 285.0f + a;
	samp->latitude = 20.55f + a;
	samp->longitude = 43.52f + a;
	samp->speed = 10.0f;
	samp->course = 60.5f;
	
	
	
	a += 0.05;
	return samp;
}