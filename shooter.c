#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "fat16.h"
#include "ext2.h"

#define INFO_CMD 0
#define FIND_CMD 1
#define DELETE_CMD 2

int check_args(int input_args, int num_args);
int command_value(char *input_command);
int file_exists(char *filename);

int main(int argc, char **argv) {

	if (argc < 2) {
		write(1, "ERROR - Too few arguments\n", strlen("ERROR - Too few arguments\n"));
		return -1;
	}

	switch (command_value(argv[1])) {
		
		case INFO_CMD:;

			if (check_args(argc, 1)) {
				if (check_args(argc, 1) == -1) {
					write(1, "ERROR - Too few arguments for command '/info'.\n", strlen("ERROR - Too few arguments for command '/info'.\n"));
					write(1, "Usage:\n\t'shooter /info [file_system]'\n\n", strlen("Usage:\n\t'shooter /info [file_system]'\n\n"));
				} else {
					write(1, "ERROR - Too many arguments for command '/info'.\n", strlen("ERROR - Too many arguments for command '/info'.\n"));
					write(1, "Usage:\n\t'shooter /info [file_system]'\n\n", strlen("Usage:\n\t'shooter /info [file_system]'\n\n"));
				}

				break;
			}
			
			int fd = file_exists(argv[2]);

			if (fd != -1) {
				fat_info fat = FAT_info(fd, FAT_SHOW);

				if (!fat.error) close(fd);
				else {
	
					ext_info ext = EXT2_info(fd, EXT_SHOW);
					if (!ext.error) close(fd); 
					else write(1, "ERROR - File system is not EXT2 nor FAT16\n", strlen("ERROR - File system is not EXT2 nor FAT16\n"));
				}
			}
		break;

		case FIND_CMD:;
			
			if (check_args(argc, 2)) {
				if (check_args(argc, 2) == -1) {
					write(1, "ERROR - Too few arguments for command '/find'.\n", strlen("ERROR - Too few arguments for command '/find'.\n"));
					write(1, "Usage:\n\t'shooter /find [file_system] [file_name]'\n\n", strlen("Usage:\n\t'shooter /find [file_system] [file_name]'\n\n"));
				} else {
					write(1, "ERROR - Too many arguments for command '/find'.\n", strlen("ERROR - Too many arguments for command '/find'.\n"));
					write(1, "Usage:\n\t'shooter /find [file_system] [file_name]'\n\n", strlen("Usage:\n\t'shooter /find [file_system] [file_name]'\n\n"));
				}

				break;
			}

			int fd2 = file_exists(argv[2]);

			if (fd2 != -1) {
				fat_info fat = FAT_info(fd2, FAT_CHECK);
				if (!fat.error) {
					FAT_find(fd2, fat, argv[3], FAT_SHOW);
					close(fd2);
				} else {

					ext_info ext = EXT2_info(fd2, EXT_CHECK);
					if (!ext.error) {
						EXT2_find(fd2, ext, argv[3], EXT_SHOW);
						close(fd2);
					} else write(1, "ERROR - File system is not EXT2 nor FAT16\n", strlen("ERROR - File system is not EXT2 nor FAT16\n"));
				}
			}
		break;
		
		case DELETE_CMD:;
		
			if (check_args(argc, 2)) {
				if (check_args(argc, 2) == -1) {
					write(1, "ERROR - Too few arguments for command '/delete'.\n", strlen("ERROR - Too few arguments for command '/delete'.\n"));
					write(1, "Usage:\n\t'shooter /delete [file_system] [file_name]'\n\n", strlen("Usage:\n\t'shooter /delete [file_system] [file_name]'\n\n"));
				} else {
					write(1, "ERROR - Too many arguments for command '/delete'.\n", strlen("ERROR - Too many arguments for command '/delete'.\n"));
					write(1, "Usage:\n\t'shooter /delete [file_system] [file_name]'\n\n", strlen("Usage:\n\t'shooter /delete [file_system] [file_name]'\n\n"));
				}

				break;
			}

			int fd3 = file_exists(argv[2]);

			if (fd3 != -1) {
				fat_info fat = FAT_info(fd3, FAT_CHECK);
				if (!fat.error) {
					FAT_delete(fd3, fat, argv[3]);
					close(fd3);
				} else {
					
					ext_info ext = EXT2_info(fd3, EXT_CHECK);
					if (!ext.error) {
						EXT2_delete(fd3, ext, argv[3]);
						close(fd3);
					} else write(1, "ERROR - File system is not EXT2 nor FAT16\n", strlen("ERROR - File system is not EXT2 nor FAT16\n"));
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

int check_args(int input_args, int num_args) {
	if (input_args < num_args+2) return -1;
	if (input_args > num_args+2) return 1;
	return 0;
}

int command_value(char *input_command) {

	if (!strcmp(input_command, "/info")) return INFO_CMD;
	if (!strcmp(input_command, "/find")) return FIND_CMD;
	if (!strcmp(input_command, "/delete")) return DELETE_CMD;

	return -1;
}

int file_exists (char *filename) {
	int fd = open(filename, O_RDWR);

	if (fd == -1) {
		char *buffer;
		buffer = malloc(strlen(filename) + strlen("ERROR - File '' could not be opened\n") + 1);

		sprintf(buffer, "ERROR - File '%s' could not be opened\n", filename);

		write(1, buffer, strlen(buffer));

		return -1;
	}

	return fd;
}
