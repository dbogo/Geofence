#ifndef TYPES_H
#define TYPES_H

#ifdef __cplusplus
extern "C" {
#endif
	
typedef enum { false, true } bool;
typedef unsigned char uint8_t;

#define TIME_TO_WAIT_SEC 1

typedef struct {
	double latitude; // value from 0.0 to 90.0
	double longitude; // value from 0.0 to 180.0 NOTE:east/west !
	float speed;
	float altitude;
	double course;
} GPSSamp;

typedef struct {
	GPSSamp p1;
	GPSSamp p2;
	float altitude; // TODO
} Zone;

typedef struct {
	double latitude; // Latitude e.g: 4124.8963 (XXYY.ZZKK.. DEG, MIN, SEC.SS)
	char lat; // Latitude e.g: N
	double longitude; // Longitude e.g: 08151.6838 (XXXYY.ZZKK.. DEG, MIN, SEC.SS)
	char lon; // Longitude e.g: W
	uint8_t quality;  // Quality 0, 1, 2
	uint8_t satellites; // Number of satellites: 1,2,3,4,5...
	float altitude; // Altitude e.g: 280.2 (Meters above mean sea level)
} gga;

typedef struct {
	double latitude;
	char lat;
	double longitude;
	char lon;
	float speed;
	double course;
} rmc;


#ifdef __cplusplus
}
#endif

#endif /* TYPES_H */

