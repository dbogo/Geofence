#ifndef INIT_H
#define INIT_H

#include "types.h"
#include "logInterface.h"

#define NO_ARGS 1
#define FOPEN_FAIL 2
#define AMBIGUOUS_ARGV 3
#define ALL_ARGV_INIT_OK 0

extern FILE* argvInputFile;

/**
 * @brief      Initialize some important stuff
 * @param      zone       pointer to Zone_general strcut
 * @param      logMaster  pointer to Log_Master struct
 * @param      edges      double pointer to the Edge struct. pointer to the array of edges 
 * 						  of the polygon represented by zone.
 */
void init(FullGPSData* gpsData, Zone_general* zone, Log_Master* logMaster, Edge** edges);

/**
 * @brief      Recieves argc, argv from main and parses that input
 * @param      zone  A struct with the geofence information (polygon, count, etc.)
 * @param[in]  argc  argc
 * @param      args  argv. should contain the name of the file with the inputs.
 * @return     return different exit codes depending on the situatio.
 */
int parse_input_args(Zone_general* zone, int argc, char** args);

/**
 * @brief      Parses a single line that contains vertex coordinates.
 * @param      str   the line of text that is read from the input file
 * @return     returns a struct of the point, initialized with the coordinates from the line.
 */
GEO_Point parse_line(char* str);

#endif /* INIT_H */