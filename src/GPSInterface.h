
/*
					Latitude = Y (0-90)
					Longitude = X (0-180)

 						AT THE EQUATOR
		One degree of latitude =  110.57 km or  68.71 mi
		One minute of latitude =    1.84 km or   1.15 mi
		One second of latitude =   30.72 m  or 100.77 ft
		One degree of longitude = 111.32 km or  69.17 mi
		One minute of longitude =   1.86 km or   1.15 mi
		One second of longitude =  30.92 m  or 101.45 ft
 * 
						AT THE POLES
		One degree of latitude =  111.69 km or  69.40 mi
		One minute of latitude =    1.86 km or  1.16 mi
		One second of latitude =   31.03 m  or  101.79 ft
 */

#ifndef GPSINTERFACE_H
#define GPSINTERFACE_H

#include "types.h"
#include "utils.h"

#define FULL_SAMPLE	1
#define REGISTERED_GGA 2
#define REGISTERED_RMC 3
#define UNRECOGNIZED_NMEA_FORMAT 9


/* 
'generic' function of a GPS sample, implemented by different libraries
	TODO: review the purpose of passToLog.. either find a way to use it 
	or remove it for now..
*/
extern int getGPSSample(GPSSamp* samp, bool passToLog);

bool isSampleInRangeGeneral(GPSSamp* samp, Zone_general* zone_gen);
bool isSampleInRange(GPSSamp* samp, Zone* zone);
int pnpoly(int nvert, double *vertx, double *verty, double testx, double testy);
//int areIntersecting(Segment side1, Segment side2);
bool isDroneGoingOffBorder(GPSSamp* samp, Zone* zone);

#endif /* GPSINTERFACE_H */

