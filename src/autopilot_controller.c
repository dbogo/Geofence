#include <unistd.h>
#include "autopilot_controller.h"
#include <mavlink_interface/inc/interface.h>

void update_autopilot(void){
	/**
	 * TODO
	 */
	return;
}

int takeover_control(time_t *commanderTimestamp){
	pre_arm_void_commands();
	offboard_control_sequence();
	usleep(10000);
	arm_sequence();
	sleep(10000);

	stop_autopilot();
	usleep(200);
	return 0;
}

int stop_autopilot(){
	mavlink_set_position_target_local_ned_t stopping_set_point;
	set_velocity(0, 0, 0, &stopping_set_point);
	autopilot_update_setpoint(stopping_set_point);
	autopilot_write_setpoint();
	return 0;
}


// void arm_sequence(){
// 	if (current_messages.heartbeat.base_mode != ARMED_BASE_MODE && arm_lock == 0){
// 			printf("Arming\n");
// 			autopilot_arm();
// 			arm_lock = 1;
// 	}
// 	usleep(100);
// }