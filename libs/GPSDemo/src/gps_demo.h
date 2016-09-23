#ifndef GPS_DEMO_H
#define GPS_DEMO_H

#include "../../../src/GPSInterface.h"

/**
 * @brief      basically gets GPS data
 * @param      samp       struct with neccessary data
 * @param[in]  passToLog  true if we want to log the stuff
 * @return     the ID of the sentence we got on this call.
 */
 int getGPSSample_DEMO(FullGPSData* samp, bool passToLog);

#endif /* GPS_DEMO_H */

