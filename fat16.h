#ifndef _FAT16_H_
#define _FAT16_H_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

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

fat_info FAT_info(int fd, int flag);
void FAT_find(int fd, fat_info info, char *filename);
void reformat (char *file);

#endif
