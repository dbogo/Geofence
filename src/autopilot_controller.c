#include <unistd.h>
#include <stdio.h>
#include "autopilot_controller.h"
#include <mavlink_interface/inc/interface.h>
#include <mavlink_interface/inc/commands.h>

void controller_take_control(void){
	// Autopilot rejects mode switch if no setpoints are sent in advance
	for(int i = 0; i < 10; ++i){
		autopilot_write();
		usleep(500);
	}
	autopilot_enable_offboard();
	usleep(1000);
}

void controller_stop_drone(void){
	autopilot_write();
}

#if 0
void return_to_zone(geo_point_t home){
	return;
}
#endif

void controller_release_control(void){
	autopilot_write();
	usleep(10000);
	disable_offboard_control_sequence();
}
