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
#define EXT_MAGIC SUPERBLOCK + 56
#define EXT_BLOCKS_COUNT SUPERBLOCK + 4
#define EXT_R_BLOCKS_COUNT SUPERBLOCK + 8
#define EXT_FREE_BLOCKS SUPERBLOCK + 12
#define EXT_FREE_INODES SUPERBLOCK + 16
#define EXT_BPG SUPERBLOCK + 32
#define EXT_FPG SUPERBLOCK + 36
#define EXT_IPG SUPERBLOCK + 40
#define EXT_FIRST_INODE SUPERBLOCK + 84
#define EXT_INODE_SIZE SUPERBLOCK + 88
#define EXT_BLOCK_SIZE SUPERBLOCK + 24
#define EXT_FIRST_BLOCK SUPERBLOCK + 20
#define EXT_VOLUME_NAME SUPERBLOCK + 120
#define EXT_LAST_MOUNT SUPERBLOCK + 44
#define EXT_LAST_WRITE SUPERBLOCK + 48
#define EXT_LAST_CHECK SUPERBLOCK + 64

#define BG_INODE_TABLE 8
#define I_BLOCK 40
#define DIR_REC 4
#define DIR_NAME_L 6
#define DIR_TYPE 7
#define DIR_NAME 8

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

typedef struct {
	int found;
	int ext_dir_entry;
}ext_file_info;

void EXT2_delete(int fd, ext_info info, char *filename);
ext_file_info EXT2_find(int fd, ext_info info, char *filename, int mode);
ext_file_info EXT2_deep_find(int fd, ext_info info, char *filename, int inode, int mode);
uint32_t get_inode_size(int fd, ext_info info, int inode);
ext_info EXT2_info(int fd, int flag);
char * unixt_to_date(uint32_t time);

#endif
