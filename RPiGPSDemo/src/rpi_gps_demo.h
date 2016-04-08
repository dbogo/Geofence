#ifndef RPI_GPS_DEMO_H
#define RPI_GPS_DEMO_H

#include "../../src/GPSInterface.h"

/* This library gets raw NMEA sentences from GPS module.
 * e.g $GPRMC,081836,A,3751.65,S,14507.36,E,000.0,360.0,130998,011.3,E*62
 * all NMEA sentences start with $GP (indicating a GPS signal), followed by
 * the type of the sentences (e.q GGA, MRC, RTE ...).
 * different sentences include different information. (time, available satellites, 
 * latitude, longitude, alt, checksum etc...)
 * 
 * an NMEA sentence is parsed and converted to custom data type, then returned.
 */

void parse_gga(gga* samp, char *nmea);
void parse_rmc(rmc* samp, char *nmea);

//NOTE: changed to extern but review this decision later.
//meant to use this function in main.c to put NMEA 
//sentences in a log file
char* generate_nmea_sentence(void);
//char* saveNMEAforLog(void);

#endif /* RPI_GPS_DEMO_H */

