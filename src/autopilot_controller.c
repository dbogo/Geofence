#include <unistd.h>
#include <stdio.h>
#include "autopilot_controller.h"
#include <mavlink_interface/inc/interface.h>

void controller_take_control(void){
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

void return_to_zone(geo_point_t home){
	return;
	// mavlink_set_position_target_global_int_t
}

void controller_release_control(void){
	autopilot_write();
	usleep(10000);
	disable_offboard_control_sequence();
}
