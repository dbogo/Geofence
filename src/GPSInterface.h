#ifndef GPSINTERFACE_H
#define GPSINTERFACE_H

#include "types.h"

#define FULL_SAMPLE	1
#define REGISTERED_GGA 2
#define REGISTERED_RMC 3
#define UNRECOGNIZED_NMEA_FORMAT 9


// 'generic' function of a GPS sample, implemented by different libraries
extern int getGPSSample(GPSSamp* samp);

#endif /* GPSINTERFACE_H */

