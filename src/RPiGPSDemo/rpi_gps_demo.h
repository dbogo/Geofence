#ifndef RPI_GPS_DEMO_H
#define RPI_GPS_DEMO_H

#include <src/GPSInterface.h>

/**
 * @brief      Gets information from GPS
 * @param      samp       struct with necessary data
 * @param[in]  passToLog  true if we want to log the stuff
 * @return     the ID of the sentence that was received on this call.
 */
 int getGPSSample_RPI(full_gps_data_t* samp, bool passToLog);

#endif /* RPI_GPS_DEMO_H */

