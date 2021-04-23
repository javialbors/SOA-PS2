#ifndef _EXT2_H_
#define _EXT2_H_

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

#define SUPERBLOCK 1024
#define MAGIC_NUMBER 0xEF53

enum {
	EXT_SHOW = 1,
	EXT_CHECK = 2
};

int EXT2_info(int fd, int flag);

#endif
