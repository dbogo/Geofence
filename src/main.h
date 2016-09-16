
#ifndef SAMPLE_TYPE_H
#define SAMPLE_TYPE_H

#include "types.h"
#include "utils.h"
#include "logInterface.h"

#define NO_ARGS -1

/**
 * @brief      initialize some important stuff
 * @param      zone       pointer to Zone_general strcut
 * @param      logMaster  pointer to Log_Master struct
 * @param      edges      double pointer to the Edge struct. pointer to the array of edges 
 * 						  of the polygon represented by zone.
 */
void init(FullGPSData* gpsData, Zone_general* zone, Log_Master* logMaster, Edge** edges);

int parse_input_args(int argc, char** args);


#endif /* SAMPLE_TYPE_H */
