#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>   // Standard input/output definitions
#include <unistd.h>  // UNIX standard function definitions

#include <common/mavlink.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>                  /* declaration of bzero() */
#include <fcntl.h>
#include <termios.h>

#include <sys/time.h>
#include <time.h>

#define USART3 3

int init_rs232();
void free_rs232();
void sendcmd(int, char*);

#define BAUDRATE B57600

// Modify it back to /dev/ttyUSB0 or the required port
#define RS232_DEVICE "/dev/ttyUSB0" 
extern const char* RS232_DEVICE_const;


int usart_recv_blocking(int i);

// Initialisation
void serial_start(const char* portname);

// Read
int serial_read_message(mavlink_message_t* message);

// Write
int serial_write_message(const mavlink_message_t* message);

// Time related
int get_time_sec(struct timeval *tv, struct timezone *tz);


#endif // SERIAL_PORT_H_
