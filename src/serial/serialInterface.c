#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>

#include "serialInterface.h"

int streamFD = 0;

int open_port(void){
	char* fileName = "/dev/ttyACM0";
	streamFD = open(fileName, O_RDWR | O_NOCTTY | O_NDELAY);
	if(streamFD < 0){
		printf("open_port: Unable to open %s.\n", fileName);
		fileName = "/dev/ttyAMA0";
		printf("trying to open %s instead...\n", fileName);
		streamFD = open(fileName, O_RDWR | O_NOCTTY | O_NDELAY);
		if(streamFD < 0)
			printf("open_port: Unable to open %s.\n", fileName);
		else 
			fcntl(streamFD, F_SETFL, 0);
	}else
		fcntl(streamFD, F_SETFL, 0);
	
	return(streamFD);
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

int get_streamFD(void){
	return streamFD;
}