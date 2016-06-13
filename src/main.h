
#ifndef SAMPLE_TYPE_H
#define SAMPLE_TYPE_H

#include "types.h"
#include "logInterface.h"
// TODO: possibly some kind of event handling (for debuggin)

//static Log_Master logMaster;

/**
 * @brief      manages the pause of loop for a certain time. for now
 *             using the nanosleep() function. 		
 * @param[in]  toleratesInterrupt  if true, the nanosleep() function will be
 *             called again to finish waiting, in an event of premature stop
 *				by a signal from process..
 * @return     value returned by nanosleep() upon successful of failed sleep/
 */
int suspend_loop(bool toleratesInterrupt);


/**
 * @brief      initialize some important stuff
 * @param      zone       pointer to Zone_general strcut
 * @param      logMaster  pointer to Log_Master struct
 */
void init(Zone_general* zone, Log_Master* logMaster);

#endif /* SAMPLE_TYPE_H */
