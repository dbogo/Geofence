
#ifndef SAMPLE_TYPE_H
#define SAMPLE_TYPE_H

#include "types.h"

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

//function prototypes
//SampInfo getGPSSample();
bool isSampleInRange(GPSSamp* samp, Zone* zone);
void handleEvents();

#endif /* SAMPLE_TYPE_H */

