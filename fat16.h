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

int FAT_info(int fd, int flag);

#endif
