#ifndef PARSER_H
#define PARSER_H

#include "../../../src/GPSInterface.h"

#define CHECKSUM_OK 0
#define CHECKSUM_ERR 1

int parse_nmea(char* sentence, FullGPSData* samp);
int validate_checksum(char* nmea);

bool is_nmea_txt(char* nmea);

static void parse_gga(gga* samp, char *nmea);
static void parse_rmc(rmc* samp, char *nmea);

static void parse_rmc_new(rmc* samp, char *nmea);
static void parse_gga_new(gga* samp, char *nmea);

static void parse_gsa(gsa* samp, char* nmea);
static void parse_gll(gll* samp, char* nmea);
static void parse_vtg(vtg* samp, char* nmea);

#endif /* PARSER_H */