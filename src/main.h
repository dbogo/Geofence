
#ifndef SAMPLE_TYPE_H
#define SAMPLE_TYPE_H

#include "types.h"
#include "logInterface.h"
// TODO: possibly some kind of event handling (for debuggin)

static Log_Master logMaster;

int suspend_loop(bool toleratesInterrupt);
void init(Zone_general* zone, Log_Master* logMaster);

#endif /* SAMPLE_TYPE_H */
