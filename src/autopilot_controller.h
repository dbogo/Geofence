#ifndef AUTOPILOT_CONTROLLER_H
#define AUTOPILOT_CONTROLLER_H


/**
 * @brief      Takes control over the drone by setting the flight controller
 *             to Offboard mode.
 */
void controller_take_control(void);

/**
 * @brief      Stops the drone by sending a setpoint command with 0 velocity
 *             in all vectors.
 */
void controller_stop_drone(void);

#if 0
void return_to_zone(geo_point_t home);
#endif

/**
 * @brief      Releases control of the autopilot by exiting Offboard mode.
 */
void controller_release_control();

#endif /* AUTOPILOT_CONTROLLER_H */
