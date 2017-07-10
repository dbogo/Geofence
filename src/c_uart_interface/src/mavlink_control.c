#include "mavlink_control.h"

int main(int argc, char** argv){
	autopilot_intialize();

	serial_start(argv[1]);

	while (1) {
		commands();
	}

	return 0;
}

// Scheduler
void commands(void){
		 operation(10);
		// operation_extended(10);
		// square_operation(7);
		// circle_operation(5);
		// automatic_takeoff(10);
		// flight_control_sequence(10);
}
