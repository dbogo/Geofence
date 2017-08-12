#ifndef PARSER_H
#define PARSER_H

#include <src/GPSInterface.h>

#define CHECKSUM_OK 0
#define CHECKSUM_ERR 1

/**
 * @brief      Parses a given NMEA sentence according to it's type, and updates
 *             the FullGPSData struct with the extracted data.
 *
 * @param      sentence  The nmea sentence that will be parsed.
 * @param      samp      Pointer to a struct that will hold the extracted information.
 *
 * @return     An integer corresponding to the type of the sentence.
 */
int parse_nmea(char *sentence, FullGPSData* samp);

int validate_checksum(char *nmea);

bool is_nmea_txt(char *nmea);

void parse_gga(gga* samp, char *nmea);
void parse_rmc(rmc* samp, char *nmea);

void parse_rmc_new(rmc* samp, char *nmea);
void parse_gga_new(gga* samp, char *nmea);

void parse_gsa(gsa* samp, char* nmea);
void parse_gll(gll* samp, char* nmea);
void parse_vtg(vtg* samp, char* nmea);

#endif /* PARSER_H */
