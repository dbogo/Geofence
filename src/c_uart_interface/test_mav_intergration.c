
#include "inc/interface.h"
#include "inc/mavlink_control.h"
#include "inc/serial_port.h"


int main(int argc, char** argv){

	autopilot_initialize();

	serial_start(argc[1]);

	read_messages();
	autopilot_start();
	autopilot_write_helper();

	if (check_offboard_control()){
		printf("Offboard Control Enabled\n");
	} else {
		printf("Offboard Cont. disabled.\n");
	}
}