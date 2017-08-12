#ifndef SERIALTEST_H_
#define SERIALTEST_H_

#include <sys/time.h>
#include <mavlink/common/mavlink.h>

extern int gps_fd;
extern const char* RS232_DEVICE_const;

#define USART3 3
#define BAUDRATE B57600
#define RS232_DEVICE "dev/ttyUSB0"

/**
 * @brief      returns gps_fd.
 */
int get_gps_fd(void);

/**
 * @brief      Opens a port for serial communication.
 * @return     returns an int that's the file descriptor of the opened port.
 */
int open_gps_port(const char* portname);

/**
 * @brief      Open a serial connection on a specified port
 * @param[in]  portname  The name of the serial port
 * @return     -1 for error and 1 for success.
 */
int open_telem_port(const char* portname);

/**
 * @brief      gets one complete NMEA message from the GPS, through the serial port that its on.
 * @param[in]  fd      the file descriptor of serial port used for communication with the GPS
 * @param      buffer  the string buffer that the message will be written to.
 * @return     returns the number of characters that were read for this NMEA sentence.
 */
int fetch_sentence_from_gps(int fd, char* buffer);

/**
 * @brief      Blocking read of one byte from the serial port.
 *
 * @return     The read byte.
 */
int usart_recv_blocking(void);

/**
 * @brief      Read one byte at a time from the MAVLink stream on the serial port
 *             and populate the mavlink message struct as new bytes arrive.
 *
 * @param      message  The MAVLink message struct which is populated with the 
 *                      parse data that's received from the serial port.
 *
 * @return     1 if a complete MAVLink message could be read, 0 otherwise.
 */
int serial_read_message(mavlink_message_t* message);

/**
 * @brief      Write a mavlink message to the serial port.
 *
 * @param[in]  message  The MAVLink message to be written.
 *
 * @return     The number of bytes written to the serial port (the size of the message).
 */
int serial_write_message(const mavlink_message_t* message);

/**
 * @brief      Gets the number of seconds and milliseconds since epoch.
 *
 * @param      tv    the timeval struct (sys/time.h)
 * @param      tz    the timezone struct
 *
 * @return     0 for successful write to structs and -1 for failure.
 */
int get_time_sec(struct timeval *tv, struct timezone *tz);

#ifdef DEBUG
/**
 * @brief      Closes the opened serial ports.
 *             Used in the signal handler to allow peaceful termination when a SIGINT is received
 *
 * @return     The result of close() on the opened ports (&&)
 */
int handle_quit_serial(void);
#endif


#endif /* SERIALTEST_H_ */
