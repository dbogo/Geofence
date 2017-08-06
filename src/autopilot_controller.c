#include <unistd.h>
#include <stdio.h>
#include "autopilot_controller.h"
#include <mavlink_interface/inc/interface.h>

int takeover_control(time_t *commanderTimestamp){
	// pre_arm_void_commands();
	offboard_control_sequence();
	usleep(10000);
	// arm_sequence();
	stop_autopilot();
	
	usleep(200);
	return 0;
}

int stop_autopilot(){
	mavlink_set_position_target_local_ned_t set_point;
	for(int i = 0; i < 30; i++){
		// printf("setting zero vals to copter!\n");
		// set_yaw(0, &set_point);
		// set_position(
		// 	current_messages.local_position_ned.x,
		// 	current_messages.local_position_ned.y,
		// 	current_messages.local_position_ned.z,
		// 	&set_point
		// );

		set_acceleration(0.3, &set_point);

		// set_velocity(0, 0, 0, &set_point);
		autopilot_update_setpoint(set_point);
		autopilot_write_setpoint();
		// printf("Current Initial position : x = %f , y = %f , z = %f\n", ip.x, ip.y, ip.z);
		// printf("%d: Current set point : x = %f, y = %f z=%f\n", i, ip.x, ip.y, ip.z - 0.25);
		// set_yaw (ip.yaw, &set_point);
		// set__(- 1.0 + ip.x , ip.y, ip.z - 2, &set_point);
		// autopilot_write();
		sleep(100000);
	}

	return 0;
}

int return_to_zone(GEO_Point home){
	// mavlink_set_position_target_global_int_t
	return 0;
}

int release_control(){
	autopilot_write();
	usleep(10000);
	disable_offboard_control_sequence();
	return 0;
}
