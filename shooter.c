#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int command_value(char *input_command);

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
	
		case 0:

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
