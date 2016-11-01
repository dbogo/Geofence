
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
 * the GEOFENCE_ macros are used for denoting the status of different geofence checks
 */
#define GEOFENCE_ALT_OK 10
#define GEOFENCE_ALT_BREACH 100
#define GEOFENCE_POLYGON_OK 20
#define GEOFENCE_POLYGON_BREACH 200

/**
 * @brief      This struct contains function pointers to functions that
 *             GPSInterface provides for handling gps stuff.
 */
struct GPS_Actions{
	int (*getGPS)(FullGPSData*, bool, void*);
};
typedef struct GPS_Actions GPS_Actions;


/**
 * TODO: inline ?
 * @brief      the signed area of the triangle loc,P1,P2
 *
 * @param[in]  p1        Polygon's first point
 * @param[in]  p2        Polygon's second point
 * @param      location  The query point
 *
 * @return     the determinant
 */
float det(GEO_Point p1, GEO_Point p2, GEO_Point location);

/**
 * @brief      assignes the correct library for the getGPS function pointer.
 *             either an RPi or a demo implementation of the getGPSSample() will be used.
 * @return     0 on success.
 */
int GPS_init(GPS_Actions* gpsHandler);

#if 0 /* DEPRECATED ?? */
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
#endif 

/**
 * @brief      runs tests for different kinds of possible geofence violations,
 *             if one of the tests is failed (i.e some kind of violation has occured), 
 *             there's a breach.
 * Winding Number algorithm for test of Point-In-Polygon inclusion.
 *             see http://geomalgorithms.com/a03-_inclusion.html
 * @param      location  the location of the test points (coordinates)
 * @param      zone      the Polygon - represented by its vertices
 * @param      edges     the Edges of the polygon
 * @return     returns true if the point is inside the polygon.
 *             if the point is inside the polygon, the winding number is non-zero.
 *             a detailed description of the algorithm is in the link.
 */
bool geofence_breached(FullGPSData* location, Zone_general* zone, Edge* edges);

/**
 * TODO: min and max alt.
 * @brief      checks if the altitude in question is within the geofence's height.
 *
 * @param      zone      The Polygon. the Zone_general struct contains the limit altitude	
 * @param[in]  altitude  The altitude of the point in question.
 *
 * @return     one of the constants describing the status, according to the outcome of the height test.
 */
int geofecnce_alt_check(Zone_general* zone, double altitude);

/**
 * @brief      checks if the point in question is inside or outside the geofence polygon.
 *             uses the Winding-Number algorithm for test of Point-In-Polygon inclusion.
 *             see http://geomalgorithms.com/a03-_inclusion.html
 * @param      zone  The geofence polygon
 * @param[in]  p     the POI
 * @return     one of the constants describing the status, according to the outcome of the PiP test.
 */
int geofence_polygon_check(Zone_general* zone, GEO_Point p);

/**
 * @brief      Creates the edges of the polygon from its vertices.
 *             Called once.
 * @param      zone   The polygon - represented by its vertices
 * @param      edges  A Edge pointer. points to an array of edges that is populated in this function
 * @return     returns the number of edges. (Actually returnes the number of vertices).
 */
int create_edges(Zone_general* zone, Edge** edges);

/* TODO */
/**
 * @brief      computes the minimun bounding rectangle of the polygon. (aka MBR)
 * @param      polygon  a struct with the polygon's points.
 */
void find_mbr(Zone_general* polygon);

/**
 * @brief      converts NMEA decimal value to degrees
 *             (used for conversion of lat lon values)
 * @param[in]  x     the decimal value extracted as-is from NMEA
 * @return     a value converted from dec to deg
 */
double to_deg(double x);

#if 0
bool isDroneGoingOffBorder(FullGPSData* location, Zone_general* zone_gen);
#endif

#endif /* GPSINTERFACE_H */