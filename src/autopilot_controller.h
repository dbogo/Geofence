#ifndef AUTOPILOT_CONTROLLER_H
#define AUTOPILOT_CONTROLLER_H

#include <time.h>
#include "mavlink_interface/inc/commands.h"
#include <types.h>

int takeover_control(time_t *commanderTimestamp);

int stop_autopilot();

int return_to_zone(GEO_Point home);
int release_control();
#endif /* AUTOPILOT_CONTROLLER_H */
