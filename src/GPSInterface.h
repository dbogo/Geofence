
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
#define REGISTERED_GSA 4
#define REGISTERED_VTG 5
#define REGISTERED_GLL 6
#define IGNORED_TXT 9
#define UNRECOGNIZED_NMEA -1

typedef struct {
	double Xmin;
	double Xmax;
	double Ymin;
	double Ymax;
	double e;
} Zone_Limits;

Segment ray;
Zone_general zone;
Zone_Limits zone_limits;
GPSSamp sample;
Segment* sides;



/**
 * @brief      basically gets GPS data
 *
 * @param[in]  fd         file descriptor of the port that's outputting NMEA
 * @param      samp       struct with neccessary data
 * @param[in]  passToLog  true if we want to log the stuff
 *
 * @return     the ID of the sentence we got on this call.
 */
extern int getGPSSample(int fd, FullGPSData* samp, bool passToLog);

void create_segments_of_zone(Zone_general* zone);

void update_ray_location(GPSSamp* samp);

int isDroneInRangeGeneral1(Zone_general* zone, FullGPSData* location);
bool isDroneInRangeGeneral(FullGPSData* location, Zone_general* zone_gen);
bool isDroneInRange(FullGPSData* location, Zone* zone);
int pnpoly(int nvert, double *vertx, double *verty, double testx, double testy);
//int areIntersecting(Segment side1, Segment side2);
bool isDroneGoingOffBorder(FullGPSData* location, Zone_general* zone_gen);

#endif /* GPSINTERFACE_H */

