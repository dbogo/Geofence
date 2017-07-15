#include <mavlink_control.h>
#include <unistd.h>

int main(int argc, char **argv)
{

	autopilot_initialize();
	serial_start(argv[1]);

	
	autopilot_start();
	// autopilot_write_helper();
	// if (check_offboard_control()){
	// 	printf("Offboard Control Enabled\n");
		check_offboard_control();
		enable_offboard_control();
		read_messages();
		// sleep(1);
		// check_offboard_control();
		// read_messages();
		sleep(1);
	// } else {
	// 	// printf("Offboard Cont. disabled.\n");
		// disable_offboard_control();
	// }
	check_arm_disarm();
	sleep(1);

	autopilot_arm();
	autopilot_arm();
	read_messages();
	sleep(2);

	// autopilot_arm();
	// sleep(1);
	check_arm_disarm();
	sleep(5);
	autopilot_disarm();
	read_messages();
	

	// while (1) {
	// 	commands();
	// }

	return 0;
}

// Scheduler
// void commands(void){
// 		 operation(10);
// 		// operation_extended(10);
// 		// square_operation(7);
// 		// circle_operation(5);
// 		// automatic_takeoff(10);
// 		// flight_control_sequence(10);
// }
