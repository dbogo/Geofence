/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   types.h
 * Author: bogo
 *
 * Created on March 22, 2016, 3:02 PM
 */

#ifndef TYPES_H
#define TYPES_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include <inttypes.h>
	
typedef enum { false, true } bool;
typedef unsigned char uint8_t;


typedef struct {
	float latitude; // value from 0.0 to 90.0
	float longitude; // value from 0.0 to 180.0 NOTE:east/west !
	float speed;
	float altitude;
	float course;
} GPSSamp;

typedef struct {
	GPSSamp p1;
	GPSSamp p2;
	float altitude; // TODO
} Zone;

typedef struct {
	float latitude; // Latitude e.g: 4124.8963 (XXYY.ZZKK.. DEG, MIN, SEC.SS)
	char lat; // Latitude e.g: N
	float longitude; // Longitude e.g: 08151.6838 (XXXYY.ZZKK.. DEG, MIN, SEC.SS)
	char lon; // Longitude e.g: W
	uint8_t quality;  // Quality 0, 1, 2
	uint8_t satellites; // Number of satellites: 1,2,3,4,5...
	float altitude; // Altitude e.g: 280.2 (Meters above mean sea level)
} gga;

typedef struct {
	float latitude;
	char lat;
	float longitude;
	char lon;
	float speed;
	float course;
} rmc;


#ifdef __cplusplus
}
#endif

#endif /* TYPES_H */

