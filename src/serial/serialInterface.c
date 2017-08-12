#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>
#include <stdbool.h>

#include "serialInterface.h"
#include <src/logInterface.h>

int streamFD = 0;

mavlink_status_t status;
bool msgReceived = false;

extern volatile float seconds;

struct termios oldtio, newtio;
static int fd;
const char* RS232_DEVICE_const;

int open_gps_port(const char *portname){
	streamFD = open(portname, O_RDWR | O_NOCTTY | O_NDELAY);
	if(streamFD < 0){
		log_err(&logMaster, "open_port: Unable to open /dev/ttyACM0");
	} else {
		fcntl(streamFD, F_SETFL, 0);
	}
	
	return(streamFD);
}

int open_telem_port(const char* portname){
	RS232_DEVICE_const = portname;
	printf("Open port : %s\n", portname);
	fd = open(portname, O_RDWR | O_NOCTTY);

	if (fd < 0){
		perror(portname);
		// exit(-1);
		return -1;
	}

	tcgetattr(fd, &oldtio);							  /* save current serial port settings */
	bzero(&newtio, sizeof(newtio));					  /* clear struct for new port settings */
	newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD; /* _no_ CRTSCTS */
	newtio.c_iflag = IGNPAR;						  // | ICRNL |IXON;
	newtio.c_oflag = IGNPAR;						  //ONOCR|ONLRET|OLCUC;
	newtio.c_cc[VTIME] = 0;							  /* inter-character timer unused */
	newtio.c_cc[VMIN] = 1;							  /* blocking read until 1 character arrives */
	printf("Succeeded\n");
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);
	return 1;
}

/* Using the fileDescriptor that's obtained from open_port(), this function
 * reads the characters from the associated file, one by one, until the end of the
 * sentence, and places them into the dedicated buffer.
 */
int fetch_sentence_from_gps(int fd, char* buffer){
	// TODO: benchmark and look for a fester way.
	char c = '\0';
	int i = 0; // index of the current end of the string
	while(read(fd, &c, 1) > 0 && c != '\n'){
			buffer[i] = c;
			i++;
	}
	return i;
}

int serial_read_message(mavlink_message_t* message){
	msgReceived = mavlink_parse_char(MAVLINK_COMM_1, usart_recv_blocking(), message, &status);
	return msgReceived;
}

int usart_recv_blocking(){
	char c;
	read(fd, &c, 1);
	return (c);
}

int serial_write_message(const mavlink_message_t* message){
	uint8_t buff[300];
	int bytesWritten;
	unsigned len = mavlink_msg_to_send_buffer(buff, message);

	for (unsigned short i = 0; i < sizeof(buff); i++)	{
		write(fd, &buff[i], 1);
	}

	bytesWritten = len;
	len = 0;
	return bytesWritten;
}

int get_time_sec(struct timeval *tv, struct timezone *tz){
	return gettimeofday(tv, tz);
}

int get_streamFD(void){
	return streamFD;
}

#ifdef DEBUG
int handle_quit_serial(){
	int close1 = close(fd);
	int close2 = close(streamFD);
	return close1 && close2;
}
#endif
