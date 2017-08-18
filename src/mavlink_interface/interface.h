
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
 * @file interface.h
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

#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include <types.h>
#include "mfunctions.h"
#include <src/types.h>
#include <src/serial/serialInterface.h>

                                                // bit number  876543210987654321
#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_POSITION     0b0000110111111000
#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_VELOCITY     0b0000110111000111
#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_ACCELERATION 0b0000110000111111
#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_FORCE        0b0000111000111111
#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_YAW_ANGLE    0b0000100111111111
#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_YAW_RATE     0b0000010111111111

#define TIMEOUT_MS 500  // milliseconds to wait until a command_ack timeout.

/**
 * The following constants are possible values of the armed/offb. cont. states as 
 * available in the base_mode field of the HEARTBEAT message.
 */
#define ARMED_BASE_MODE             209
#define DISARMED_BASE_MODE          81
#define OFFBOARD_CONTROL_BASE_MODE  157

 /**
  * timestamps of every message in Mavlink_Messagess
  */
typedef struct Time_Stamps{
    uint64_t heartbeat;
    uint64_t sys_status;
    uint64_t battery_status;
    uint64_t radio_status;
    uint64_t local_position_ned;
    uint64_t global_position_int;
    uint64_t global_pos_cov;
    uint64_t position_target_local_ned;
    uint64_t position_target_global_int;
    uint64_t highres_imu;
    uint64_t attitude;
    uint64_t command_ack;
} Time_Stamps;


/**
 * Holds the last messages that were received from the autopilot
 */
typedef struct Mavlink_Messages {
    int sysid;
    int compid;
    mavlink_heartbeat_t heartbeat; // Heartbeat
    mavlink_sys_status_t sys_status; // System Status
    mavlink_battery_status_t battery_status; // Battery Status
    mavlink_radio_status_t radio_status; // Radio Status
    mavlink_local_position_ned_t local_position_ned; // Local Position
    mavlink_global_position_int_t global_position_int; // Global Position
    mavlink_global_position_int_cov_t global_pos_cov; // A more accurate global position data
    mavlink_position_target_local_ned_t position_target_local_ned; // Local Position Target
    mavlink_position_target_global_int_t position_target_global_int; // Global Position Target
    mavlink_highres_imu_t highres_imu; // HiRes IMU
    mavlink_attitude_t attitude; // Attitude
    mavlink_command_ack_t command_ack; // Command acknowledgment
    Time_Stamps time_stamps; // Time Stamps
} Mavlink_Messages;


typedef struct autopilot_info {
    bool control_status;
    bool arm_status;
    Mavlink_Messages current_messages;
    mavlink_set_position_target_local_ned_t current_setpoint;
} Autopilot_Info;


extern bool control_status;
extern bool arm_status;

/**
 * @brief      Read data from the GLOBAL_POSOTION_INT message
 */
void autopilot_read_global_pos(void);

/**
 * @brief      Reads data from the LOCAL_POS_NED message
 */
void autopilot_read_local_pos_ned(void);

/**
 * @brief      Writes a HIL_GPS message to the autopilot with data from a 
 *             GPS unit that is connected to the companion computer
 *
 * @param      info  the struct from which the data is pulled
 *
 * @return     The result of writing the message to the serial port
 */
int autopilot_write_gps(full_gps_data_t* info);

/**
 * @brief      A utility function to print the current global position from 
 *             a GLOBAL_POS_INT message.
 */
void print_global_pos_int(void);

/**
 * @brief      Sets the acceleration of the autopilot along the axis
 *
 * @param[in]  z     The eccleration along the z axis (vertical)
 * @param      sp    the setpoint to which the acceleration value is written
 */
void set_acceleration(float z, mavlink_set_position_target_local_ned_t *sp);

/**
 * @brief      Used to check whether the flag for offboard mode is set or not.
 *
 * @return     True if the flag is set, false otherwise
 */
bool autopilot_control_status(void);

/**
 * @brief      Zeros out all the timestamps.
 *
 * @param      ts    The TimeStamps struct
 */
void reset_timestamps(Time_Stamps* ts);

// Initialization

/**
 * @brief      Initialize system-wide important parameters and flags
 */
void autopilot_initialize(void);

/**
 * @brief      Used to define the initial position.
 *             This method is supposed to be called once on startup
 *             after the first call to read_message() (which listens for a 
 *             position message from the autopilot).
 */
void autopilot_start(void);

/**
 * @brief      Reads MAVLink messages from the autopilot until all
 *             requested messages are received (this is controlled by
 *             the received_all flag inside the function). The received messages
 *             are set in the Mavlink_Messages.
 *             This method is blocking and will not exit until
 *             all requested messages are obtained (may become an infinite loop).
 */
void read_messages(void);

/**
 * @brief      A "dummy" method that writes a void (all zeros) 
 *             setpoint to the autopilot.
 *             Used to signal startup.
 */
void autopilot_write(void);

/**
 * @brief      Writes a MAVLink message to the autopilot.
 *
 * @param[in]  message  A MAVLink message struct with the encapsulated message
 */
void autopilot_write_message(mavlink_message_t message);

/**
 * @brief      Write the current setpoint to the autopilot after it's been updated
 * (needs to be called explicitly)
 */
void autopilot_write_setpoint(void);

/**
 * @brief      Updates the current setpoint with the given setpoint.
 *
 * @param[in]  setpoint  A SET_POSITION_LOCAL_NED struct that is assigned
 * 						 to the current setpoint.
 */
void autopilot_update_setpoint(mavlink_set_position_target_local_ned_t setpoint);

// Offboard Control

/**
 * @brief      Disables the offboard control of the autopilot.
 *
 * @return     The offboard control status: true for active offboard control
 *                 and false for disabled offboard control.
 */
bool autopilot_disable_offboard(void);

/**
 * @brief      Enables the offboard control of the autopilot, setting the appropriate flag.
 *
 * @return     Similar to disable_offboard_control() but the inverse 
 *                     (true for successfully enabling offboard control).
 */
bool autopilot_enable_offboard(void);

/**
 * @brief      The function that's used by enable/disable_offbard_control() 
 *             to actually write the appropriate command to the autopilot.
 *             The function will wait for a command_ack() from the autopilot.
 *
 * @param[in]  flag  Indicated whether to toggle offboard control on or off.
 *
 * @return     1 for a successful command. 0 for failure.
 */
int toggle_offboard_control(bool flag);

/**
 * @brief      Arm the autopilot.
 */
void autopilot_arm(void);

/**
 * @brief      Disarm the autopilot.
 */
void autopilot_disarm(void);

/**
 * @brief      Used by autopilot_arm/disarm() in a similar manner
 *             to toggle_offboard_control(). The function will wait for a command_ack 
 *             from the autopilot.
 *
 * @param[in]  flag  Indicated on or off toggle.
 *
 * @return     1 for a successful command. 0 for failure.
 */
int toggle_arm_disarm(bool flag);

/**
 * @brief      Listens to a COMMAND_ACK message from the autopilot to verify
 *             the successful delivery of a command. Blocks execution until an
 *             acknowledgment is received.
 *
 * @param[in]  cmd  The CMD_ID of the command to check acknowledgment for 
 *                  (as defined in the corresponding mavlink header).
 *
 * @return     1 if the autopilot confirmed the acknowledgment, 0 otherwise.
 */
int verify_command_ack(int cmd);

/**
 * @brief      A convenience function to check if the autopilot is in offboard control or not.
 *
 * @return     true if the autopilot is under offboard control, false otherwise.
 */
int check_offboard_control(void);

/**
 * @brief      Similar to check_offboard_control().
 *
 * @return     true if armed and false if disarmed.
 */
int check_arm_disarm(void);

/**
 * @brief      The function that's used by check_offboard_control() and
 *             check_arm_disarm() to determine the status.
 *
 * @param[in]  COMMAND_ID  The ID of the command as defined by the MAVLink protocol.
 *
 * @return     1 for a positive command_ack. 0 otherwise.
 */
int check_message(uint16_t COMMAND_ID);

/**
 * @brief      Update the full_gps_data_t based on information from the GLOBAL_POSITION_INT_COV message.
 *
 * @param      gpsData  Pointer to a full_gps_data_t struct
 *
 * @return     1 for success.
 */
int get_gps_from_autopilot(full_gps_data_t *gpsData);

/**
 * @brief      Used to send "dummy" setpoints to the autopilot.
 *             This method is meant to be used before switching to offboard mode.
 *             It's required that the setpoint streaming will already begin before 
 *             switching to offboard, otherwise the autopilot will reject the mode switch.
 *
 * @return     0 for success.
 */
int pre_arm_void_commands(void);

/**
 * @brief      Receive a HEARTBEAT message from the autopilot.
 *             Used to verify that the autopilot is up and running and is connected.
 *
 * @return     1 for success
 */
int autopilot_ok(void);


#ifdef DEBUG
/**
 * @brief      Used to shutdown everything autopilot-related (like disarming and
 *             disabling offboard control)
 *             This is called upon a SIGINT to avoid autopilot failsafes.
 *
 * @return     0 for success.
 */
int handle_quit_autopilot(void);
#endif

/**
 * @brief      Sets the desired position to a setpoint in a local NED frame.
 *
 * @param[in]  x     The x (North) component of the target setpoint.
 * @param[in]  y     The y (East) component of the target setpoint.
 * @param[in]  z     The z (Down) component of the target setpoint.
 * @param      sp    The setpoint to which the values will be written.
 */
void set_position(float x, float y, float z, mavlink_set_position_target_local_ned_t* sp);

/**
 * @brief      A convenirence function that does the three steps together: Create
 *             a position setpoint according to passed values, updates the autopilot
 *             interface internal setpoint variable with the new setpoint, and
 *             writes the setpoint. 
 *
 * @param[in]  x          The x (North) component of the target setpoint.
 * @param[in]  y          The y (East) component of the target setpoint.
 * @param[in]  z          The z (Down) component of the target setpoint.
 * @param      set_point  The setpoint to which the values will be written.
 */
void set__(float x, float y, float z, mavlink_set_position_target_local_ned_t* set_point);

/**
 * @brief      Sets the velocity for the autopilot
 *
 * @param[in]  vx         Velocity along the North axis.
 * @param[in]  vy         Velocity along the East axis.
 * @param[in]  va         Velocity along the Down axis.
 * @param      set_point  The setpoint to which the values will be written
 */
void set_velocity(float vx, float vy, float va, mavlink_set_position_target_local_ned_t* set_point);

/**
 * @brief      Sets the yaw for the autopilot
 *
 * @param[in]  yaw   The yaw in radians.	
 * @param      sp    The setpoint to which the values will be written
 */
void set_yaw(float yaw, mavlink_set_position_target_local_ned_t* sp);

void position_and_speed_set(float x, float y, float z ,float vx, float vy, float vz, mavlink_set_position_target_local_ned_t* final_set_point);

void set_circle(float R, float theta, float z, mavlink_set_position_target_local_ned_t* set_point);

/**
 * @brief      Gets the time since epoch in microseconds
 */
uint64_t get_time_usec(void);

#endif // _INTERFACE_H_
