
/*
 						AT THE EQUATOR
		One degree of latitude =  110.57 km or  68.71 mi
		One minute of latitude =    1.84 km or   1.15 mi
		One second of latitude =   30.72 m  or 100.77 ft
		One degree of longitude = 111.32 km or  69.17 mi
		One minute of longitude =   1.86 km or   1.15 mi
		One second of longitude =  30.92 m  or 101.45 ft
  
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
struct GPS_actions{
	int (*getGPS)(full_gps_data_t*, bool, void*);
};
typedef struct GPS_actions GPS_actions_t;


/**
 * @brief the signed area of the triangle loc,P1,P2
 *
 * @param[in]  p1        Polygon's first point
 * @param[in]  p2        Polygon's second point
 * @param      location  The query point
 *
 * @return     the determinant
 */
float det(geo_point_t p1, geo_point_t p2, geo_point_t location);

/**
 * @brief      assigns the correct library for the getGPS function pointer.
 *             either an RPi or a demo implementation of the getGPSSample() will be used.
 * @return     0 on success.
 */
int GPS_init(GPS_actions_t *gpsHandler);

/**
 * @brief      runs tests for different kinds of possible geofence violations,
 *             if one of the tests is failed (i.e some kind of violation has occured), 
 *             there's a breach.
 * Winding Number algorithm for test of Point-In-Polygon inclusion.
 *             see http://geomalgorithms.com/a03-_inclusion.html
 * @param      location  the location of the test points (coordinates)
 * @param      zone      the Polygon - represented by its vertices
 * @return     returns true if the point is inside the polygon.
 *             if the point is inside the polygon, the winding number is non-zero.
 *             a detailed description of the algorithm is in the link.
 */
bool geofence_breached(full_gps_data_t* location, zone_t* zone);

/**
 * TODO: min and max alt.
 * @brief      checks if the altitude in question is within the geofence's height.
 *
 * @param      zone      The Polygon. the zone_t struct contains the limit altitude	
 * @param[in]  altitude  The altitude of the point in question.
 *
 * @return     one of the constants describing the status, according to the outcome of the height test.
 */
int geofence_alt_check(zone_t* zone, double altitude);

/**
 * @brief      checks if the point in question is inside or outside the geofence polygon.
 *             uses the Winding-Number algorithm for test of Point-In-Polygon inclusion.
 *             see http://geomalgorithms.com/a03-_inclusion.html
 * @param      zone  The geofence polygon
 * @param[in]  p     the POI
 * @return     one of the constants describing the status, according to the outcome of the PiP test.
 */
int geofence_polygon_check(zone_t* zone, geo_point_t p);

/**
 * @brief      Creates the edges of the polygon from its vertices.
 *             Called once.
 * @param      zone   The polygon - represented by its vertices
 * @param      edges  A edge_t pointer. points to an array of edges that is populated in this function
 * @return     returns the number of edges. (Actually returns the number of vertices).
 */
int create_edges(zone_t* zone, edge_t** edges);

/**
 * @brief      computes the minimum bounding rectangle of the polygon. (aka MBR)
 * @param      polygon  a struct with the polygon's points.
 */
void find_mbr(zone_t* polygon);

/**
 * @brief      converts NMEA decimal value to degrees
 *             (used for conversion of lat lon values)
 * @param[in]  x     the decimal value extracted as-is from NMEA
 * @return     a value converted from dec to deg
 */
double to_deg(double x);

#endif /* GPSINTERFACE_H */