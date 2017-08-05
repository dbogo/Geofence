#include <mavlink_control.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	time_t begin = time(NULL);
	autopilot_initialize();

	serial_start("/dev/ttyUSB0");
	printf("Opened serial connection.\n");
	read_messages(); // first read to make sure that there is a connection
	printf("Acquired initial read\n");
	autopilot_start();
	printf("locked data from read_messages()\n");

	send_pre_arm_void_commands();
	// arm_sequence();
	
	usleep(200000); // 200ms = 5Hz
	
	offboard_control_sequence();
	usleep(200000);

	mavlink_set_position_target_local_ned_t set_point;
	for(int i = 0; i < 15; i++){
		printf("setting zero vals to copter!\n");
		// set_yaw(0, &set_point);
		set_position(
			current_messages.local_position_ned.x,
			current_messages.local_position_ned.y,
			current_messages.local_position_ned.z,
			&set_point
		);

		// set_velocity(0, 0, 0, &set_point);
		autopilot_update_setpoint(set_point);
		autopilot_write_setpoint();
		// printf("Current Initial position : x = %f , y = %f , z = %f\n", ip.x, ip.y, ip.z);
		// printf("%d: Current set point : x = %f, y = %f z=%f\n", i, ip.x, ip.y, ip.z - 0.25);
		// set_yaw (ip.yaw, &set_point);
		// set__(- 1.0 + ip.x , ip.y, ip.z - 2, &set_point);
		// autopilot_write();
		usleep(100000);
	}

	// disarm_sequence();
	usleep(200000);

	disable_offboard_control_sequence();
	usleep(200000);

	return 0;
}

void send_pre_arm_void_commands(){
	for(int i = 0; i < 20; i++){
		autopilot_write();
		usleep(20000);
	}
}

// int autopilot_ok(){
// 	mavlink_message_t msg;
// 	while(!msg.msgid == MAVLINK_MSG_ID_HEARTBEAT){
// 		serial_read_message(&msg);
// 	}
// 	return 1;
// }

// Scheduler
// void commands(void){
// 		 operation(10);
// 		// operation_extended(10);
// 		// square_operation(7);
// 		// circle_operation(5);
// 		// automatic_takeoff(10);
// 		// flight_control_sequence(10);
// }
