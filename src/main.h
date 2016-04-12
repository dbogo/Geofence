
#ifndef SAMPLE_TYPE_H
#define SAMPLE_TYPE_H

#include "types.h"

bool isSampleInRange(GPSSamp* samp, Zone* zone);
bool isDroneGoingOffBorder(GPSSamp* samp, Zone* zone);
// TODO: possibly some kind of event handling (for debuggin)

#endif /* SAMPLE_TYPE_H */
