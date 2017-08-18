#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char uint8_t;

#define OUTSIDE 0
#define INSIDE 1

/* min & max values (meters), for valid for geofence altitude boundaries */
#define MIN_ALT -500
#define MAX_ALT  10000

/**
 * @brief Represents a smaller, truncated version of the data gathered from GPS.
 * 
 * This struct contains only the most frequently used parameters.
 */
typedef struct GPSSamp{
	double lat; // value from 0.0 to 90.0
	double lon; // value from 0.0 to 180.0 NOTE:east/west !
	double spd;
	double alt;
	double course;
} GPS_samp_t;

/**
 * @brief Represents a single 2D point
 */
typedef struct GEO_point{
	double lon;
	double lat;
} geo_point_t;

/**
 * @brief Minimum bounding rectangle of a certain polygon
 */
typedef struct MBR{
	geo_point_t p1;
	geo_point_t p2;
} mbr_t;


/**
 * @brief A straight line between two points
 */
typedef struct Edge{
	geo_point_t p1;
	geo_point_t p2;
} edge_t;


/**
 * @brief Represents a single polygon
 */
typedef struct Zone{
	size_t numVertices;
	double alt;
	mbr_t mbr;
	geo_point_t* vertices;
} zone_t;


/**
 * @brief The entire collection of relevant data, gathered from GPS
 * 
 * lat_deg and lon_deg are the values of a point on the map where google map
 * would show a point. lat/lon data that's received from NMEA sentences is in
 * decimal, and not in degrees, hence the need for a differentiation.
 */
typedef struct {
	double latitude;
	double longitude;

	// lat_deg and lon_deg is the point where google maps shows a point.
	// since NMEA data is decimal and not degrees, a conversion is needed.
	double lat_deg; 
	double lon_deg;

	unsigned char lat;
	unsigned char lon;
	double alt;
	double course;
	double spdKph;

	uint8_t quality;
	uint8_t satellites; /*!< number of satellites being tracked currently */
	unsigned char fixType;
	long fixTime;

	double pdop;
	double hdop;
	double vdop;
	double spdKnots;
	bool status; 
} full_gps_data_t;


/**
 * @brief Relevant data from $GPGGA
 */
typedef struct {
	double latitude;
	unsigned char lat;
	double longitude;
	unsigned char lon;
	uint8_t quality;
	uint8_t satellites;
	double altitude;
} gga;


/**
 * @brief Relevant data from $GPGSA
 */
typedef struct {
	unsigned char fixType; // 1 = no fix; 2 = 2D fix; 3 = 3D fix
	double pdop;
	double hdop;
	double vdop;
} gsa;


/**
 * @brief Relevant data from $GPVTG
 */
typedef struct {
	double spdKnots;
	double spdKph;
} vtg;


/**
 * @brief Relevant data from $GPGLL
 */
typedef struct {
	double latitude;
	unsigned char lat;
	double longitude;
	unsigned char lon;
	unsigned long fixTime;
	bool status; // A (active) or V (void)  as true/false
} gll;


/**
 * @brief Relevant data from $GPRMC
 */
typedef struct {
	double latitude;
	unsigned char lat;
	double longitude;
	unsigned char lon;
	double speed;
	double course;
} rmc;

#ifdef __cplusplus
}
#endif

#endif /* TYPES_H */
