#ifndef AUTOPILOT_CONTROLLER_H
#define AUTOPILOT_CONTROLLER_H

#include <time.h>
#include "mavlink_interface/inc/commands.h"


void update_autopilot(void);

int takeover_control(time_t *commanderTimestamp);

int stop_autopilt();

#endif /* AUTOPILOT_CONTROLLER_H */