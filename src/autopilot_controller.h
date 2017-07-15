#ifndef AUTOPILOT_CONTROLLER
#define AUTOPILOT_CONTROLLER

#include <mavlink_interface/inc/interface.h>


void update_autopilot(void);

int takeover_control();


#endif /* AUTOPILOT_CONTROLLER */