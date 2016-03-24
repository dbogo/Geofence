/* 
 * File:   rpi_gps_demo.h
 * Author: bogo
 */

#ifndef RPI_GPS_DEMO_H
#define RPI_GPS_DEMO_H

#include "../types.h"

//NOTE: SHOULD THOSE BE 'extern' ????

/* This function gets raw NMEA sentences from GPS module.
 * e.g $GPRMC,081836,A,3751.65,S,14507.36,E,000.0,360.0,130998,011.3,E*62
 * all NMEA sentences start with $GP (indicating a GPS signal), followed by
 * the type of the sentences (e.q GGA, MRC, RTE ...).
 * different sentences include different information. (time, available satellites, 
 * latitude, longitude, alt, checksum etc...
 * 
 * an NMEA sentence is parsed and converted to custom data type, then returned.
 */
extern GPSSamp* RPiGetGPSSample(void);
//extern void RPiGetGPSSample(GPSSamp* sample, char* nmea, bool hasgga, bool hasrmc);

extern void parse_gga(gga* samp, char *nmea);
extern void parse_rmc(rmc* samp, char *nmea);

char* generate_nmea_sentence(void);

#endif /* RPI_GPS_DEMO_H */

