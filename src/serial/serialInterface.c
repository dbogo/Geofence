#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>

#include "serialInterface.h"

int open_port(void){
	char* fileName = "/dev/ttyACM0";
	int fileDescriptor = open(fileName, O_RDWR | O_NOCTTY | O_NDELAY);
	if(fileDescriptor < 0){
		printf("open_port: Unable to open %s.\n", fileName);
		fileName = "/dev/ttyAMA0";
		printf("trying to open %s instead...\n", fileName);
		fileDescriptor = open(fileName, O_RDWR | O_NOCTTY | O_NDELAY);
		if(fileDescriptor < 0)
			printf("open_port: Unable to open %s.\n", fileName);
		else 
			fcntl(fileDescriptor, F_SETFL, 0);
	}else
		fcntl(fileDescriptor, F_SETFL, 0);
	
	return(fileDescriptor);
}

/* Using the fileDescriptor that's obtained from open_port(), this function
 * reads the characters from the associated file, one by one, until the end of the
 * sentence, and places them into the dedicated buffer.
 */
int fetch_sentence_from_gps(int fd, char* buffer){
	char c;
	int i = 0; // index of the current end of the string
	while(c != '\n'){
		if(read(fd, &c, 1) < 0){
			perror("Error: fetch_sentence_from_gps()");
			return -1;
		} else {
			buffer[i] = c;
			i++;
		}
	}
	return i;
}
