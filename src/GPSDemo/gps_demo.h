#ifndef GPS_DEMO_H
#define GPS_DEMO_H

#include <src/GPSInterface.h>

/**
 * @brief      Creates random GPS data for simulation
 * @param      samp       struct with necessary data
 * @param[in]  passToLog  true if we want to log the stuff
 * @return     the ID of the sentence we got on this call.
 */
int getGPSSample_DEMO(full_gps_data_t* samp, bool passToLog);

#endif /* GPS_DEMO_H */

