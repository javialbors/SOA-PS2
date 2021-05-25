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

#define ROOT_INODE 2

#define EXT2_FT_FILE 1
#define EXT2_FT_DIR 2

enum {
	EXT_SHOW = 1,
	EXT_CHECK = 2
};

typedef struct {
	int error;
	char filesystem[5];
	uint32_t inodes_count;
	uint32_t blocks_count;
	uint32_t reserved_blocks;
	uint32_t free_blocks;
	uint32_t free_inodes;
	uint32_t blocks_per_group;
	uint32_t frags_per_group;
	uint32_t inodes_per_group;
	uint32_t first_inode;
	uint16_t inode_size;
	uint32_t block_size;
	uint32_t first_block;
	char volume_name[17];
	uint32_t last_mount;
	uint32_t last_write;
	uint32_t last_check;
}ext_info;

void EXT2_find(int fd, ext_info info, char *filename, int inode);
int EXT2_deep_find(int fd, ext_info info, char *filename, int inode, int level);
uint32_t get_inode_size(int fd, ext_info info, int inode);
ext_info EXT2_info(int fd, int flag);
char * unixt_to_date(uint32_t time);

#endif
