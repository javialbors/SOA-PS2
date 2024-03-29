#ifndef _FAT16_H_
#define _FAT16_H_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define FAT_NAME 3
#define FAT_BPS 11
#define FAT_SPC 13
#define FAT_RESERVED 14
#define FAT_NFATS 16
#define FAT_ROOT_ENTRIES 17
#define FAT_SIZE 22
#define FAT_LABEL 43
#define FAT_SYSTYPE 54

#define ENTRY_EXTENSION 0x08
#define ENTRY_ATTRIBUTE 0x0B
#define ENTRY_CLUSTER_START 0x1A
#define ENTRY_SIZE 0x1C

#define FAT_ENTRY 32 
#define FAT_FILE 32
#define FAT_SUBDIR 16
#define FAT_LONG_NAME 15
#define FAT_DELETED_FILE 0xE5

enum {
	FAT_SHOW = 1,
	FAT_CHECK = 2
};

typedef struct {
	int error;
	char filesystem[9];
	char system_name[9];
	uint16_t sector_size;
	uint8_t sectors_per_cluster;
	uint16_t reserved;
	uint8_t nfats;
	uint16_t max_root_entries;
	uint16_t sectors_per_fat;
	char label[12];
}fat_info;

typedef struct {
	int found;
	int fat_location;
}fat_file_info;

fat_info FAT_info(int fd, int flag);
fat_file_info FAT_find(int fd, fat_info info, char *filename, int mode);
fat_file_info FAT_deep_find(int fd, fat_info info, char *filename, int base_offset, int cluster_offset, int mode);
void FAT_delete(int fd, fat_info info, char *filename);
void reformat (char *file);

#endif
