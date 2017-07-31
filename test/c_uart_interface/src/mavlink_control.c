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
	for(int i = 0; i < 30; i++){
		// printf("Current Initial position : x = %f , y = %f , z = %f\n", ip.x, ip.y, ip.z);
		printf("%d: Current set point : x = %f, y = %f z=%f\n", i, ip.x, ip.y, ip.z - 0.25);
		set_yaw (ip.yaw, &set_point);
		set__(- 1.0 + ip.x , ip.y, ip.z - 2, &set_point);
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
		autopilot_write_helper();
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
