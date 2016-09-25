#ifndef RPI_GPS_DEMO_H
#define RPI_GPS_DEMO_H

#include "src/GPSInterface.h"

/* This library gets raw NMEA sentences from the GPS module. */

 #if 0 
 char* generate_nmea_sentence(void); 
 #endif

/**
 * @brief      basically gets GPS data
 * @param      samp       struct with neccessary data
 * @param[in]  passToLog  true if we want to log the stuff
 * @return     the ID of the sentence we got on this call.
 */
 int getGPSSample_RPI(FullGPSData* samp, bool passToLog);

#endif /* RPI_GPS_DEMO_H */

