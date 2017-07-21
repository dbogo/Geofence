#include "autopilot_controller.h"

void update_autopilot(void){
	/**
	 * TODO
	 */
	return;
}

int takeover_control(time_t *commanderTimestamp){
	automatic_takeoff(10, commanderTimestamp);
	// read_messages();
	// offboard_control_sequence();
	
	// autopilot_start();
	// autopilot_write_helper();
	// usleep(1000);
	// arm_sequence();	
	// usleep(1000);
	// arm_sequence();
	// check_arm_disarm();
	// // offboard_control_sequence();
	// // enable_offboard_control();
	// int i = 0;
	// while(i < 10){
	// 	autopilot_write_helper();
	// 	usleep(30000);
	// 	i++;
	// }
	return 0;
	// drone_stop();
}

// void arm_sequence(){
// 	if (current_messages.heartbeat.base_mode != ARMED_BASE_MODE && arm_lock == 0){
// 			printf("Arming\n");
// 			autopilot_arm();
// 			arm_lock = 1;
// 	}
// 	usleep(100);
// }