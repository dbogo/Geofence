#ifndef PARSER_H
#define PARSER_H

#include "../../src/GPSInterface.h"

int parse_nmea(char* sentence, GPSSamp* samp);

static void parse_gga(gga* samp, char *nmea);
static void parse_rmc(rmc* samp, char *nmea);

#endif /* PARSER_H */