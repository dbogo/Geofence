#include <stdio.h>
#include "gps_demo.h"
  
// gets info from GPS
GPSSamp getGPSSample(){
	GPSSamp info = { .latitude = 50.0f, .longitude = 72.0f};
	return info;
}