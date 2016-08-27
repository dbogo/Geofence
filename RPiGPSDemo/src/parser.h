#ifndef PARSER_H
#define PARSER_H

#include "../../src/GPSInterface.h"

NMEA_id parse_nmea(char* sentence, GPSSamp* samp);
bool is_nmea_txt(char* nmea);

static void parse_gga(gga* samp, char *nmea);
static void parse_rmc(rmc* samp, char *nmea);

static void parse_rmc_new(rmc* samp, char *nmea);
static void parse_gga_new(gga* samp, char *nmea);

static void parse_gsa(gsa* samp, char* nmea);
static void parse_gll(gll* samp, char* nmea);
static void parse_vtg(vtg* samp, char* nmea);

#endif /* PARSER_H */