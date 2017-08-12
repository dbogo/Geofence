#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "../inc/commands.h"

int arm_lock = 0;
int offboard_control_lock = 0;

int value_mg_x, value_mg_y, value_mg_z;

// Scheduler related
bool lock_ = false;
int Program_counter = 0; 

volatile float seconds = 0;
volatile float omega = 0;

time_t end;


void operation (float timer){
	time_t begin = time(NULL);
	read_messages();
	autopilot_start();
	autopilot_write_helper();

	mavlink_set_position_target_local_ned_t set_point;

	switch(Program_counter){
		case 0 :
			arm_sequence();
			break;
		case 1 :
			offboard_control_sequence();
			break;
		case 2 :
			printf("Set point 1 \n");
			set_velocity( - 0.25 , - 0.0 , - 0.50 , &set_point);
			set_yaw (ip.yaw, &set_point);
			set__(ip.x , ip.y , ip.z - 1, &set_point);
			break;
		case 3 :
			set_yaw (ip.yaw, &set_point);
			set__(- 1.0 + ip.x , ip.y, ip.z - 2, &set_point);
			break;
		case 4 : 
			printf("Current Initial position : x = %f , y = %f , z = %f\n", ip.x, ip.y, ip.z);
			printf("Current set point : x = %f, y = %f z=%f\n", ip.x, ip.y, ip.z - 0.25);
			set_yaw (ip.yaw, &set_point);
			set__(- 1.0 + ip.x , ip.y, ip.z - 2, &set_point);
			break;
		case 5 :
			set_yaw (ip.yaw, &set_point);
			set__(- 1.0 + ip.x , ip.y, ip.z - 0.75 , &set_point);
			break;
		case 6 : 
			disable_offboard_control_sequence(); 
			Program_counter = 7;
			break;
		case 7 : 
			printf("Idling..\n");
			Program_counter = 8;
			break;
		case 8 :
			disarm_sequence();
			Program_counter = 9;
			break;
		default :
			break;
	}

	program_counter_sequence(timer, &begin);

	if (Program_counter == 9) { 
		Program_counter = 0;
	}
}


void operation_extended (float timer){
	time_t begin = time(NULL);
	read_messages();
	autopilot_start();
	autopilot_write_helper();

	mavlink_set_position_target_local_ned_t set_point;

	switch(Program_counter){
		case 0 :
			arm_sequence();
			break;
		case 1 :
			offboard_control_sequence();
			break;
		case 2 :
			printf("Set point 1 \n");
			set_velocity( - 0.25 , - 0.0 , - 0.50 , &set_point);
			set_yaw (ip.yaw, &set_point);
			set__( 1.0 + ip.x , ip.y , ip.z - 1, &set_point);
			break;
		case 3 :
			set__( 1.0 + ip.x , ip.y, ip.z - 2, &set_point);
			break;
		default :
			break;
	}

	program_counter_sequence(timer, &begin);

	if (Program_counter == 4) { Program_counter = 3;}
}


void square_operation (float timer){
	time_t begin = time(NULL);
	read_messages();
	autopilot_start();
	autopilot_write_helper();

	mavlink_set_position_target_local_ned_t set_point;


	switch(Program_counter){
		case 0 :
			arm_sequence();
			break;
		case 1 :
			offboard_control_sequence();
			break;
		case 2 :
			set_velocity( - 0.25 , - 0.25 , - 0.25 , &set_point);
			set_yaw (ip.yaw, &set_point);
			set__(ip.x ,ip.x , ip.z - 2.0, &set_point);
			break;
		case 3 :
			set_yaw (ip.yaw, &set_point);
			set__(ip.x + 1.0, ip.x, ip.z - 2.0, &set_point);
			break;
		case 4 :
			set_yaw (ip.yaw, &set_point);
			set__(ip.x + 1.0, ip.x + 1.0 , ip.z - 2.0, &set_point);
			break;
		case 5 :
			set_yaw (ip.yaw, &set_point);
			set__(ip.x  ,ip.x + 1.0 , ip.z  -2.0, &set_point);
			break; 
		case 6 :
			set_yaw (ip.yaw, &set_point);
			set__(ip.x ,ip.x  , ip.z  -1.0, &set_point);
			break;
		case 7 : 
			printf("Disabled offboard control\n");

			autopilot_disable_offboard(); 
			Program_counter = 8; 
			break;
		case 8 : 
			printf("Idling..\n");
			break;
		case 9 :
			printf("Disarmed \n");
			autopilot_disarm();
			Program_counter = 10;
			break;
		default :
			break;
		}

	program_counter_sequence(timer, &begin);

	if (Program_counter == 10) { Program_counter = 10;}
}


void circle_operation (float timer){
	time_t begin = time(NULL);
	read_messages();
	autopilot_start();
	autopilot_write_helper();

	mavlink_set_position_target_local_ned_t set_point;

	switch(Program_counter){
		case 0 :
			arm_sequence();
			break;
		case 1 :
			offboard_control_sequence();
			break;
		case 2 :
			set_circle(ip.x + 1, ip.y + (float) omega, ip.z - 1.0 , &set_point);
			omega++;
			if (omega > 360) omega = 0;
			break;
		default : 
			break;
	}

	program_counter_sequence(timer, &begin);
	if (Program_counter == 0 || Program_counter == 3) { Program_counter = 2;}
}


void automatic_takeoff (float timer, time_t *begin){
	// time_t begin = time(NULL);
	read_messages();
	autopilot_start();
	autopilot_write_helper();

	mavlink_set_position_target_local_ned_t set_point;

	switch(Program_counter){
		case 0 :
			arm_sequence();
			break;
		case 1 :
			offboard_control_sequence();
			break;
		case 2 :
			printf("Current Initial position : x = %f , y = %f , z = %f\n", ip.x, ip.y, ip.z);
			printf("Current set point : x = %f, y = %f z=%f\n", ip.x, ip.y, ip.z - 1);
			set_yaw (ip.yaw, &set_point);
			set_velocity( - 0.25 , - 0.0 , - 0.50 , &set_point);
			set__( ip.x , ip.y , ip.z - 2.00, &set_point);
			break;
		case 3 : 
			printf("Current set point : x = %f, y = %f z=%f\n", ip.x, ip.y, ip.z - 0.25);
			set_yaw (ip.yaw, &set_point);
			set__( ip.x , ip.y , ip.z - 2.00 , &set_point);
			break;
		case 4 : 
			printf("Current set point : x = %f, y = %f z=%f\n", ip.x, ip.y, ip.z - 0.25);
			set_yaw (ip.yaw, &set_point);
			set__( ip.x , ip.y , ip.z - 1.00 , &set_point);
			break;
		case 5 : 
			printf("Current set point : x = %f, y = %f z=%f\n", ip.x, ip.y, ip.z - 0.25);

			set_yaw (ip.yaw, &set_point);
			set__( ip.x, ip.y , ip.z - 0.50 , &set_point);
			break;
		case 6 : 
			disable_offboard_control_sequence();
			Program_counter = 7; 
			break;
		case 7 : 
			printf("Idling..\n");
			Program_counter = 8;
			break;
		case 8 :
			disarm_sequence();
			Program_counter = 9;
			break;
		default :
			break;
		}
		
		program_counter_sequence(timer, begin);

		if (Program_counter == 9) { Program_counter = 9;}
}


void flight_control_sequence (float timer){
	time_t begin = time(NULL);
	read_messages();
	autopilot_start();
	autopilot_write_helper();

	mavlink_set_position_target_local_ned_t set_point;

	switch(Program_counter){
			case 0 :
				arm_sequence();
				break;
			case 1 :
				offboard_control_sequence();
				break;
			case 2 :
				printf("Current Initial position : x = %f , y = %f , z = %f\n", ip.x, ip.y, ip.z);
				printf("Current set point : x = %f, y = %f z=%f\n", ip.x, ip.y, ip.z - 1);
				set_velocity( - 0.25 , - 0.0 , - 0.50 , &set_point);
				set__( ip.x , ip.y , ip.z - 2.00, &set_point);
				break;
			case 3 : 
				printf("Current Initial position : x = %f , y = %f , z = %f\n", ip.x, ip.y, ip.z);
				printf("Current set point : x = %f, y = %f z=%f\n", ip.x, ip.y, ip.z - 0.25);
				set__( ip.x - 2.00 , ip.y , ip.z -2.00 , &set_point);
				break;
			case 4 : 
				printf("Current Initial position : x = %f , y = %f , z = %f\n", ip.x, ip.y, ip.z);
				printf("Current set point : x = %f, y = %f z=%f\n", ip.x, ip.y, ip.z - 0.25);
				set__( ip.x , ip.y , ip.z - 2.00 , &set_point);
				break;
			case 5 : 
				printf("Current Initial position : x = %f , y = %f , z = %f\n", ip.x, ip.y, ip.z);
				printf("Current set point : x = %f, y = %f z=%f\n", ip.x, ip.y, ip.z - 0.25);	
				set__( ip.x , ip.y - 2.00 , ip.z - 2.00, &set_point);
				break;
			case 6 : 
				printf("Current Initial position : x = %f , y = %f , z = %f\n", ip.x, ip.y, ip.z);
				printf("Current set point : x = %f, y = %f z=%f\n", ip.x, ip.y, ip.z - 0.25);
				set__( ip.x , ip.y + 0.00 , ip.z - 2.00 , &set_point);
				break;
			case 7 : 
				printf("Current Initial position : x = %f , y = %f , z = %f\n", ip.x, ip.y, ip.z);
				printf("Current set point : x = %f, y = %f z=%f\n", ip.x, ip.y, ip.z - 0.25);
				set__( ip.x + 0.00 , ip.y , ip.z - 0.50 , &set_point);
				break;
			case 8 : 
				disable_offboard_control_sequence();
				Program_counter = 9; 
				break;
			case 9 : 
				printf("Idling..\n");
				Program_counter = 10;
				break;
			case 10 :
				disarm_sequence();
				Program_counter = 11;
				break;
			default : break;
		}
		
		program_counter_sequence(timer, &begin);

		if (Program_counter == 11) { Program_counter = 11;}
}

// Flight functions 
void arm_sequence (void){
	if (current_messages.heartbeat.base_mode != ARMED_BASE_MODE && !arm_lock){
		printf("Arming\n");
		autopilot_arm();
		arm_lock = 1;
	}
	usleep(100);
}


void offboard_control_sequence(void){
	printf("Enable offboard control\n");

	autopilot_write();
	usleep(200);
	autopilot_enable_offboard();
	usleep(200);
	autopilot_write();

	// if (current_messages.heartbeat.base_mode != OFFBOARD_CONTROL_BASE_MODE){
	// 	control_status = false;
	// 	autopilot_write();
	// 	enable_offboard_control();
	// 	autopilot_write();
	// }		
		
	// usleep(100);
	Program_counter = 2;
}


void disable_offboard_control_sequence(void){
	printf("Disabled offboard control\n");
	autopilot_disable_offboard(); 
} 


void disarm_sequence(void){
	printf("Disarmed \n");
	autopilot_disarm();

	if (current_messages.heartbeat.base_mode == ARMED_BASE_MODE){
		autopilot_disarm();
	}
}


void program_counter_sequence(float timer, time_t* begin){
	end = time(NULL);

	if ((end - *begin) >= timer){
		*begin = time(NULL);
		printf("Operation : %d \n", Program_counter);
		Program_counter++;
	}
}


void autopilot_write_helper(void){
	if (!lock_)
		autopilot_write();		
	lock_ = true;
}
