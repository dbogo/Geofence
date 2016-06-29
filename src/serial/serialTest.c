#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>

#include "serialInterface.h"

int main(int argc, char** argv){
	int fd = open_port();

	char buffer[100];
	memset(buffer, '\0', 100);

	fetch_sentence_from_gps(fd, buffer);
	printf("%s\n", buffer);
	
	return 0;
}


