
#ifndef SAMPLE_TYPE_H
#define SAMPLE_TYPE_H

#include "types.h"

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

bool isSampleInRange(GPSSamp* samp, Zone* zone);
// TODO: void handleEvents(void);

#endif /* SAMPLE_TYPE_H */
