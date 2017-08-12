#ifndef AUTOPILOT_CONTROLLER_H
#define AUTOPILOT_CONTROLLER_H

#include <time.h>
#include "mavlink_interface/inc/commands.h"
#include <types.h>

void controller_take_control();

void controller_stop_drone();

void return_to_zone(geo_point_t home);
void controller_release_control();
#endif /* AUTOPILOT_CONTROLLER_H */
