
/****************************************************************************
 *
 *   Copyright (c) 2014 MAVlink Development Team. All rights reserved.
 *   Author: Trent Lukaczyk, <aerialhedgehog@gmail.com>
 *           Jaycee Lock,    <jaycee.lock@gmail.com>
 *           Lorenz Meier,   <lm@inf.ethz.ch>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file interface.cpp
 *
 *
 * @author Trent Lukaczyk, <aerialhedgehog@gmail.com>
 * @author Jaycee Lock,    <jaycee.lock@gmail.com>
 * @author Lorenz Meier,   <lm@inf.ethz.ch>
 *
 * Modifications made by :
 * Mohamed Hage Hassan, <mohamed.hagehassan@yahoo.com>
 *
 */

#include <inc/interface.h>

char control_status;
char arm_status;

int system_id;
int autopilot_id;
int companion_id;

// Current messages and current_setpoint definitions
Mavlink_Messages current_messages;
mavlink_set_position_target_local_ned_t current_setpoint;

// Initial position setpoints and first time lock

mavlink_set_position_target_local_ned_t initial_position;
mavlink_set_position_target_local_ned_t ip;

// Lock for initial position acquisation
int initial_position_lock = 0;

// Highres dependent flag timeout reset 
float highres_flag = 1;

// First read lock 
int lock_read_messages = 0;


Time_Stamps create_TimeStamps(){
	Time_Stamps ts;
	reset_timestamps(&ts);
	return ts;
}

void reset_timestamps(Time_Stamps* time_stamps){
	time_stamps->heartbeat = 0;
	time_stamps->sys_status = 0;
	time_stamps->battery_status = 0;
	time_stamps->radio_status = 0;
	time_stamps->local_position_ned = 0;
	time_stamps->global_position_int = 0;
	time_stamps->position_target_local_ned = 0;
	time_stamps->position_target_global_int = 0;
	time_stamps->highres_imu = 0;
	time_stamps->attitude = 0;
	time_stamps->command_ack = 0;
}


// Initialisation
void autopilot_initialize(void){
	// initialize attributes

	control_status = 0;      // whether the autopilot is in offboard control mode
	arm_status = 0; 		 // whether the autopilot is armed or not

	system_id    = 1; // system id
	autopilot_id = 1; // autopilot component id
	companion_id = 0; // companion computer component id

	current_messages.sysid  = system_id; // Set current system id to predefined one
	current_messages.compid = autopilot_id; // Set current autopilot id to predefined one
}


void autopilot_start(void){
	// This is used only once to define the initial position

	if (initial_position_lock == 0){
		Mavlink_Messages local_data = current_messages;
		initial_position.x        = local_data.local_position_ned.x;
		initial_position.y        = local_data.local_position_ned.y;
		initial_position.z        = local_data.local_position_ned.z;
		initial_position.vx       = local_data.local_position_ned.vx;
		initial_position.vy       = local_data.local_position_ned.vy;
		initial_position.vz       = local_data.local_position_ned.vz;
		initial_position.yaw      = local_data.attitude.yaw;
		initial_position.yaw_rate = local_data.attitude.yawspeed;
		ip = initial_position;
	}
	initial_position_lock = 1;
}


// READ
void read_messages(void){
	bool success;               // receive success flag
	bool received_all = false;  
	Time_Stamps this_timestamps = create_TimeStamps();

	highres_flag = 1; // Highres dependent flag reset timeout

	// Blocking wait for new data
	while (!received_all){ 
		// Read one message at a time and complete the current_messages structure
		mavlink_message_t message;

		// Read from serial port
		success = serial_read_message(&message);

		if(success){
			switch (message.msgid){

				case MAVLINK_MSG_ID_COMMAND_ACK:{
					// Acknowledgement check for pc version
					printf("Command acknowledgement recieved\n");
					mavlink_msg_command_ack_decode(&message, &(current_messages.command_ack));
					current_messages.time_stamps.command_ack = get_time_usec();
					this_timestamps.command_ack = current_messages.time_stamps.command_ack;
					printf("> Command ID: %d, result: %d\n",
								current_messages.command_ack.command,
								current_messages.command_ack.result);
					break;
					}

				case MAVLINK_MSG_ID_HEARTBEAT:{
					mavlink_msg_heartbeat_decode(&message, &(current_messages.heartbeat));
					// Verification of current autopilot state from heartbeat base_mode on PC version
					// printf("\n MAV mode = %u \n", current_messages.heartbeat.base_mode);
					current_messages.time_stamps.heartbeat = get_time_usec();
					this_timestamps.heartbeat = current_messages.time_stamps.heartbeat;	
					// printf("MAVLink version: %d\n", current_messages.heartbeat.mavlink_version);
					break;
					}

				case MAVLINK_MSG_ID_SYS_STATUS:{
					mavlink_msg_sys_status_decode(&message, &(current_messages.sys_status));
					current_messages.time_stamps.sys_status = get_time_usec();
					this_timestamps.sys_status = current_messages.time_stamps.sys_status;
					break;
					}

				case MAVLINK_MSG_ID_BATTERY_STATUS:{
					mavlink_msg_battery_status_decode(&message, &(current_messages.battery_status));
					current_messages.time_stamps.battery_status = get_time_usec();
					this_timestamps.battery_status = current_messages.time_stamps.battery_status;
					break;
					}

				case MAVLINK_MSG_ID_RADIO_STATUS:{
					mavlink_msg_radio_status_decode(&message, &(current_messages.radio_status));
					current_messages.time_stamps.radio_status = get_time_usec();
					this_timestamps.radio_status = current_messages.time_stamps.radio_status;
					break;
					}

				case MAVLINK_MSG_ID_LOCAL_POSITION_NED:{
					mavlink_msg_local_position_ned_decode(&message, &(current_messages.local_position_ned));
					current_messages.time_stamps.local_position_ned = get_time_usec();
					this_timestamps.local_position_ned = current_messages.time_stamps.local_position_ned;
					break;
					}

				case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:{
					mavlink_msg_global_position_int_decode(&message, &(current_messages.global_position_int));
					current_messages.time_stamps.global_position_int = get_time_usec();
					this_timestamps.global_position_int = current_messages.time_stamps.global_position_int;
					break;
					}

				case MAVLINK_MSG_ID_POSITION_TARGET_LOCAL_NED:{
					mavlink_msg_position_target_local_ned_decode(&message, &(current_messages.position_target_local_ned));
					current_messages.time_stamps.position_target_local_ned = get_time_usec();
					this_timestamps.position_target_local_ned = current_messages.time_stamps.position_target_local_ned;
					break;
					}

				case MAVLINK_MSG_ID_POSITION_TARGET_GLOBAL_INT:{
					mavlink_msg_position_target_global_int_decode(&message, &(current_messages.position_target_global_int));
					current_messages.time_stamps.position_target_global_int = get_time_usec();
					this_timestamps.position_target_global_int = current_messages.time_stamps.position_target_global_int;
					break;
					}

				case MAVLINK_MSG_ID_HIGHRES_IMU: {
					mavlink_msg_highres_imu_decode(&message, &(current_messages.highres_imu));
					current_messages.time_stamps.highres_imu = get_time_usec();
					this_timestamps.highres_imu = current_messages.time_stamps.highres_imu;
					break;
					}

				case MAVLINK_MSG_ID_ATTITUDE: {
					mavlink_msg_attitude_decode(&message, &(current_messages.attitude));
					current_messages.time_stamps.attitude = get_time_usec();
					this_timestamps.attitude = current_messages.time_stamps.attitude;
					break;
					}

				default:
					break;
				 // MAVLINK_MESSAGES
			} // end: switch msgid
		} // end: if read message

		// Loop untill the full reception of the first heartbeat and local_position_ned
		// for the first time, then depend on the highres MAVLink message for the rest
		// if (lock_read_messages == 0){
			// Check for receipt of all items
			received_all =
					// this_timestamps.heartbeat                  &&
					this_timestamps.command_ack 
			//				this_timestamps.battery_status             &&
			//				this_timestamps.radio_status               &&
							// this_timestamps.local_position_ned         
			//				this_timestamps.global_position_int        &&
			//				this_timestamps.position_target_local_ned  &&
			//				this_timestamps.position_target_global_int &&
			//				this_timestamps.highres_imu                &&
			//				this_timestamps.attitude                   &&
						;		
		// } else {
		// 	received_all = this_timestamps.highres_imu;	
		// 	if (highres_flag == 0) break;
		// }
	} 
		// lock_read_messages = 1;		
	return;
}


// Write
void autopilot_write(void){
	// signal startup
	// prepare an initial setpoint, just stay put 
	// void setpoint
	mavlink_set_position_target_local_ned_t set_point;
	set_point.type_mask = MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_VELOCITY & MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_YAW_RATE;
	set_point.coordinate_frame = MAV_FRAME_LOCAL_NED;
	set_point.vx       = 0.0;
	set_point.vy       = 0.0;
	set_point.vz       = 0.0;
	set_point.yaw_rate = 0.0;
	
	// set position target
	current_setpoint = set_point;

	autopilot_write_setpoint();

	return;
}


void autopilot_write_setpoint(void){

	// pull from position target
	mavlink_set_position_target_local_ned_t set_point = current_setpoint;

	// double check some system parameters
	if (!set_point.time_boot_ms)
		set_point.time_boot_ms = (uint32_t) (get_time_usec()/1000);
	set_point.target_system    = system_id;
	set_point.target_component = autopilot_id;

	//   ENCODE
	mavlink_message_t message;
	mavlink_msg_set_position_target_local_ned_encode(system_id, companion_id, &message, &set_point);

	//   WRITE
	autopilot_write_message(message);
	return;
}


void autopilot_write_message(mavlink_message_t message){
	// Write the message to serial port
	serial_write_message(&message);
	return;
}


void autopilot_update_setpoint(mavlink_set_position_target_local_ned_t setpoint){
	// Update setpoint
	current_setpoint = setpoint;
}


// Offboard Control

bool enable_offboard_control(void){
	// Should only send this command once
	if ( control_status == false ){
		//   TOGGLE OFF-BOARD MODE

		// Sends the command to go off-board
		int success = toggle_offboard_control( true );

		// Check the command was written
		if ( success ){
			control_status = true;
			printf("Offb Cont Enabled\n");
		}
	} else { 
		printf("offb cont already enabled\n");
	}
	return control_status;
}


bool disable_offboard_control(void){
	// Should only send this command once
	if ( control_status == true ){
		
		//   TOGGLE OFF-BOARD MODE

		// Sends the command to stop off-board
		int success = toggle_offboard_control(false);

		// Check the command was written
		if ( success ){
			control_status = false;
			printf("Offb Cont disabled\n");
		}
	} else {
		printf("offb cont already disabled\n");
	}
	return control_status; 
}


int toggle_offboard_control(bool flag){
	// Prepare command for off-board mode
	mavlink_command_long_t com = { 0 };
	com.target_system    = system_id;
	com.target_component = autopilot_id;
	com.command          = MAV_CMD_NAV_GUIDED_ENABLE;
	com.confirmation     = true;
	com.param1           = (float) flag; // flag >0.5 => start, <0.5 => stop

	// Encode
	mavlink_message_t message;
	mavlink_msg_command_long_encode(system_id, companion_id, &message, &com);

	// Send the message
	int len = serial_write_message(&message);

	return len;
}


// Arm/disarm Control
void autopilot_arm(void){
	// Should only send this command once
	// if ( arm_status == false ){
		// ARM 
		int success = toggle_arm_disarm( 1 );
		// Check the command was written
		if ( success ) {
			arm_status = true;
			printf("Autopilot sucessfully armed\n");
		}
	// } 
}


void autopilot_disarm(void){
	// Should only send this command once
	if ( arm_status == true ){
		// DISARM 
		int success = toggle_arm_disarm( 0 );
		// Check the command was written
		if ( success ){
			arm_status = false;
			printf("Autopilot sucessfully disarmed\n");
		}
	} 
}


int toggle_arm_disarm( bool flag ){
	// Prepare command for arming/disarming
	mavlink_command_long_t autopilot_status = { 0 };
	autopilot_status.target_system    = system_id;
	autopilot_status.target_component = autopilot_id;
	autopilot_status.command          = MAV_CMD_COMPONENT_ARM_DISARM;
	autopilot_status.confirmation     = true;
	autopilot_status.param1           = (float) flag; // flag = 1 => arm, flag = 0 => disarm

	// Encode
	mavlink_message_t message;
	mavlink_msg_command_long_encode(system_id, companion_id, &message, &autopilot_status);

	// Send the message
	int len = serial_write_message(&message);
	return len;
}

// MAVLink messages acknowledgement

// NEEDS PX4 Master version or stable v1.4 
int check_offboard_control(void){
	// check offboard control message reception
	int success = check_message(MAV_CMD_NAV_GUIDED_ENABLE);

	if (success){
		printf("Offboard control checked\n");
		return 1;
	} else {
		printf("Offboard control check failed\n");
		return 0;
	}
}


int check_arm_disarm(void){
	// check arm/disarm message reception
	int success = check_message(MAV_CMD_COMPONENT_ARM_DISARM);

	if (success){
		printf("Arm/disarm checked\n");
		return 1;
	} else {
		printf("Arm/disarm check failed\n");
		return 0;
	}
}


int check_message(uint16_t COMMAND_ID){
	if((current_messages.command_ack.command == COMMAND_ID) &&
	   (current_messages.command_ack.result == MAV_RESULT_ACCEPTED)){
		return 1;
	} else {
		return 0;
	}
}


// Control
// Set position function and masks
void set_position(float x, float y, float z, mavlink_set_position_target_local_ned_t* set_position){
	set_position->type_mask = MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_POSITION;
	set_position->coordinate_frame = MAV_FRAME_LOCAL_NED;

	set_position->x = x; 
	set_position->y = y; 
	set_position->z = z;
}


// Set velocity function and masks
void set_velocity(float vx, float vy, float vz, mavlink_set_position_target_local_ned_t* sp){
	sp->type_mask = MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_VELOCITY;
	sp->coordinate_frame = MAV_FRAME_LOCAL_NED;

	sp->vx = vx;
	sp->vy = vy;
	sp->vz = vz;
}


void set_yaw(float yaw, mavlink_set_position_target_local_ned_t* sp){
	sp->type_mask &= MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_YAW_ANGLE ;
	sp->yaw = yaw;
}


// Set position, update setpoint and send the message 
void set__(float x, float y, float z, mavlink_set_position_target_local_ned_t* final_set_point){
	set_position(x, y, z, final_set_point);
	autopilot_update_setpoint(*final_set_point);
	autopilot_write_setpoint();
}


// Set position and velocity, update setpoint and send the message 
void position_and_speed_set(float x, float y, float z ,float vx, float vy, float vz, mavlink_set_position_target_local_ned_t* final_set_point){
	set_position(x, y, z, final_set_point);
	set_velocity(vx, vy, vz, final_set_point);
	autopilot_update_setpoint(*final_set_point);
	autopilot_write_setpoint();
}


// Draw a circle with R and theta (angle) coordinates from current position
void set_circle(float R, float theta, float z, mavlink_set_position_target_local_ned_t* set_point){
	set__( (R * tan_2pi(theta))/Beta(theta)  , R / Beta(theta), z, set_point);
}


// Time 
uint64_t get_time_usec(void){
	static struct timeval _time_stamp;
	get_time_sec(&_time_stamp, NULL);
	return _time_stamp.tv_sec*1000000 + _time_stamp.tv_usec;
}