#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

	
typedef enum { false, true } bool;
typedef unsigned char uint8_t;

#define OUTSIDE 0
#define INSIDE 1

/* NOTE: Pay attention to to calculations where course is in radians or in degrees ! */
typedef struct {
	float latitude; // value from 0.0 to 90.0
	float longitude; // value from 0.0 to 180.0 NOTE:east/west !
	float speed;
	float altitude;
	float course;
} GPSSamp;

/* A representation of a single 2D point on the surface. */
typedef struct {
	float longitude;
	float latitude;
} GEO_Point;

// deprecated
#if 0
/* A TEMPORARY implementaion of a Geo-zone.
	implemented as a rectangle */
typedef struct {
	GEO_Point p1;
	GEO_Point p2;
	float altitude; // NOTE: Zone is a 3D shape
} Zone;
#endif

/* A straight line connectring two points in 2D */
typedef struct {
	GEO_Point p1;
	GEO_Point p2;
} Edge;

/* An arbitrary Geo-zone that is a simple polygon.
	may consist of more than 2 vertices. */
typedef struct {
	size_t numVertices;
	float altitude;
	GEO_Point* vertices;
} Zone_general;

typedef struct {
	float latitude;
	float longitude;
	unsigned char lat;
	unsigned char lon;
	float altitude;
	float course;
	float spdKph;

	uint8_t quality;
	uint8_t satellites;
	unsigned char fixType;
	long fixTime;

	float pdop;
	float hdop;
	float vdop;
	float spdKnots;
	bool status; 
} FullGPSData;

typedef struct {
	float latitude; // Latitude e.g: 4124.8963 (XXYY.ZZKK.. DEG, MIN, SEC.SS)
	unsigned char lat; // Latitude e.g: N
	float longitude; // Longitude e.g: 08151.6838 (XXXYY.ZZKK.. DEG, MIN, SEC.SS)
	unsigned char lon; // Longitude e.g: W
	uint8_t quality;  // Quality 0, 1, 2
	uint8_t satellites; // Number of satellites: 1,2,3,4,5...
	float altitude; // Altitude e.g: 280.2 (Meters above mean sea level)
} gga;

typedef struct {
	unsigned char fixType; // 1 = no fix; 2 = 2D fix; 3 = 3D fix
	float pdop;
	float hdop;
	float vdop;
} gsa;

typedef struct {
	float spdKnots;
	float spdKph;
} vtg;

typedef struct {
	float latitude;
	unsigned char lat;
	float longitude;
	unsigned char lon;
	unsigned long fixTime;
	bool status; // A (activce) or V (void)  as true/false
} gll;

typedef struct {
	float latitude;
	unsigned char lat;
	float longitude;
	unsigned char lon;
	float speed;
	float course;
} rmc;

#ifdef __cplusplus
}
#endif

#endif /* TYPES_H */
