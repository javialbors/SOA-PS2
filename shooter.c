#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "fat16.h"
#include "ext2.h"

int command_value(char *input_command);
int file_exists(char *filename);

int main(int argc, char **argv) {
	
	if (argc < 3) {
		write(1, "ERROR - Too few arguments\n", strlen("ERROR - Too few arguments\n"));
		return -1;
	}

	if (argc > 3) {
		write(1, "ERROR - Too many arguments\n", strlen("ERROR - Too many arguments\n"));
		return -1;
	}

	switch (command_value(argv[1])) {
		
		case 0:;
			
			int fd = file_exists(argv[2]);
			if (fd > 0) {
				
				if (FAT_info(fd)) {
				} else {
				}
			}
		break;

		default:;
			char *buffer;
			buffer = malloc(strlen(argv[1]) + strlen("ERROR - Command '' not found\n") + 1);

			sprintf(buffer, "ERROR - Command '%s' not found\n", argv[1]);

			write(1, buffer, strlen(buffer)); 
		break;
	}

	return 0;
}

int command_value(char *input_command) {

	if (!strcmp(input_command, "/info")) return 0;

	return -1;
}

int file_exists (char *filename) {
	int fd = open(filename, O_RDONLY);

	if (fd == -1) {
		char *buffer;
		buffer = malloc(strlen(filename) + strlen("ERROR - File '' could not be opened\n") + 1);

		sprintf(buffer, "ERROR - File '%s' could not be opened\n", filename);

		write(1, buffer, strlen(buffer));
	}

	return fd;
}
