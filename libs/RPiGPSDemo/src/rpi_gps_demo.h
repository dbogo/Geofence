#ifndef RPI_GPS_DEMO_H
#define RPI_GPS_DEMO_H

#include <src/GPSInterface.h>

/* This library gets raw NMEA sentences from the GPS module. */

/**
 * @brief      basically gets GPS data
 * @param      samp       struct with neccessary data
 * @param[in]  passToLog  true if we want to log the stuff
 * @return     the ID of the sentence we got on this call.
 */
 int getGPSSample_RPI(FullGPSData* samp, bool passToLog, void* userData);

#endif /* RPI_GPS_DEMO_H */

