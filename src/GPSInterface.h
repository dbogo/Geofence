
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

/**
 * @brief      This struct contains function pointers to functions that
 *             GPSInterface provides for handling gps stuff.
 */
struct GPS_Actions{
	int (*getGPS)(FullGPSData*, bool);
};

typedef struct GPS_Actions GPS_Actions;

/**
 * @brief      assignes the correct library for the getGPS function pointer.
 *             either an RPi or a demo implementation of the getGPSSample() will be used.
 * @return     0 on success.
 */
int GPS_init(GPS_Actions* gpsHandler);

/**
 * @brief      Determines if the test point is to the left of the vector,
 *             accounting for the direction of the vector (up or down).
 * @param[in]  p     The test point
 * @param[in]  e     the vector, represented by a segment of it as an Edge. (two points)
 * @return     return a positive integer if the point is on the left, and a negative otherwise.
 *             A special case: 0, if the point is on the line.
 */
int isLeft(GEO_Point p, Edge e);

/**
 * @brief      checks if a polygon's segment crosses a ray in the upwards direction.
 * @param      e     A polygons edge
 * @param[in]  p     the test point
 * @return     true if a upwards cross, false otherwise
 */
bool upwards_cross(Edge e, GEO_Point p);

/**
 * @brief      see upwards_cross(). This is the opposite.
 */
bool downwards_cross(Edge e, GEO_Point p);

/**
 * @brief      Winding Number algorithm for test of Point-In-Polygon inclusion.
 *             see http://geomalgorithms.com/a03-_inclusion.html
 * @param      location  the location of the test points (coordinates)
 * @param      zone      the Polygon - represented by its vertices
 * @param      edges     the Edges of the polygon
 * @return     returns true if the point is inside the polygon.
 *             if the point is inside the polygon, the winding number is non-zero.
 *             a detailed description of the algorithm is in the link.
 */
int wn_PnPoly(FullGPSData* location, Zone_general* zone, Edge* edges);

/**
 * @brief      Creates the edges of the polygon from its vertices.
 *             Called once.
 * @param      zone   The polygon - represented by its vertices
 * @param      edges  A Edge pointer. points to an array of edges that is populated in this function
 * @return     returns the number of edges. (Actually returnes the number of vertices).
 */
int create_edges(Zone_general* zone, Edge** edges);

/* TODO */
#if 0
/**
 * @brief      computes the minimun bounding rectangle of the polygon. (aka MBR)
 * @param      polygon  a struct with the polygon's points.
 */
void find_mbr(Zone_general* polygon);
#endif 

bool isDroneGoingOffBorder(FullGPSData* location, Zone_general* zone_gen);

#endif /* GPSINTERFACE_H */