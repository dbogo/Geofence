#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

int open_port(void){
	char* fileName = "/dev/ttyAMA0";
	int fileDescriptor = open(fileName, O_RDWR | O_NOCTTY | O_NDELAY);
	if(fileDescriptor == -1)
		printf("open_port: Unable to open %s", fileName);
	else
		fcntl(fileDescriptor, F_SETFL, 0);
	
	return(fileDescriptor);
}

int main(int argc, char** argv){
	int fd = open_port();

	char buffer[100];
	memset(buffer, "\0", 100);

	int n = 0;
	while(n < 100){
		read(fd, buffer, 10);
		printf("%s\n", buffer);
	}
	
	return 0;
}


