
#ifndef SAMPLE_TYPE_H
#define SAMPLE_TYPE_H

#include "types.h"

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

#define LOG_FILE "operation.log"
//function prototypes
//SampInfo getGPSSample();
bool isSampleInRange(GPSSamp* samp, Zone* zone);
void handleEvents();

int finiLogSystem();
void logEvent(const log4c_category_t* logObj, int logPriority, const char* format);
int initLogSystem(const log4c_category_t* logObj, const char* instanceName, const char* appenderName);

#endif /* SAMPLE_TYPE_H */

