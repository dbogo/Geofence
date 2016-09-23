#ifndef SERIALTEST_H
#define SERIALTEST_H

/**
 * the file descriptor of the port that will be opened using open_port().
 */
extern int streamFD;

/**
 * @brief      returns streamFD.
 */
int get_streamFD(void);


/**
 * @brief      Opens a port for serial communication.
 * @return     returns an int that's the file descriptor of the opened port.
 */
int open_port(void);

/**
 * @brief      gets one complete NMEA message from the GPS, through the serial port that its on.
 * @param[in]  fd      the file descriptor of serial port used for communication with the GPS
 * @param      buffer  the string buffer that the message will be written to.
 * @return     returns the number of charachters that were read for this NMEA sentence.
 */
int fetch_sentence_from_gps(int fd, char* buffer);

#endif /* SERIALTEST_H */
