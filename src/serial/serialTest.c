#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>

int open_port(void){
	char* fileName = "/dev/ttyACM0";
	int fileDescriptor = open(fileName, O_RDWR | O_NOCTTY | O_NDELAY);
	if(fileDescriptor < 0)
		printf("open_port: Unable to open %s", fileName);
	else
		fcntl(fileDescriptor, F_SETFL, 0);
	
	return(fileDescriptor);
}

void get_sentence(int fd, char* buffer){
	char* c;
	while(*c != '\n'){
		if(read(fd, c, 1) < 0){
			perror("Error: ");
		} else {
			strcat(buffer, c);
		}
	}
}


int main(int argc, char** argv){
	int fd = open_port();

	char* buffer = malloc(100 * sizeof(char));
	memset(buffer, '\0', sizeof(buffer));

#if 0
	int n = 0;
	//while(n < 100){
		n = read(fd, buffer, 500);
		if(n < 0)
			perror("Error: ");
		else
			printf("%d\n", n);
		printf("%s\n", buffer);
		memset(buffer, '\0', 500);
	//}
#endif
	get_sentence(fd, buffer);
	printf("%s\n", buffer);
	
	return 0;
}


