#include "ext2.h"

void EXT2_find(int fd, ext_info info, char *filename, int inode) {
	if (!EXT2_deep_find(fd, info, filename, inode, 0)) {
		write(1, "ERROR - File not found\n", strlen("ERROR - File not found\n"));
	}
}

int EXT2_deep_find(int fd, ext_info info, char *filename, int inode, int level) {
	
	int block_group = (inode-1) / info.inodes_per_group;
	int inode_index = (inode-1) % info.inodes_per_group;
	int group_base_offset = block_group * (info.blocks_per_group * info.block_size);
	int bg_offset = group_base_offset + (SUPERBLOCK*2);
	
	lseek(fd, bg_offset + 8, SEEK_SET);
	uint32_t bg_inode_table_block;
	read(fd, &bg_inode_table_block, 4);
	
	int inode_table = group_base_offset + (bg_inode_table_block * info.block_size);
	int inode_offset = inode_table + (inode_index * info.inode_size);
	
	uint32_t i_blocks;
	lseek(fd, inode_offset + 28, SEEK_SET);
	read(fd, &i_blocks, 4);

	//Iterar segun i_blocks?
	uint32_t data_block;
	lseek(fd, inode_offset + 40 /*+ (i*4)*/, SEEK_SET);
	read(fd, &data_block, 4);
		
	int inode_data_block = data_block * info.block_size;
	uint16_t rec_len = 0;

	do {
		lseek(fd, inode_data_block + rec_len, SEEK_SET);
		uint32_t ind;
		read(fd, &ind, 4);

		lseek(fd, inode_data_block + rec_len + 6, SEEK_SET);
		uint8_t name_len;	
		read(fd, &name_len, 1);

		lseek(fd, inode_data_block + rec_len + 7, SEEK_SET);
		uint8_t filetype;
		read(fd, &filetype, 1);

		char *name = malloc(name_len + 1);
		lseek(fd, inode_data_block + rec_len + 8, SEEK_SET);
		read(fd, name, name_len);
		name[name_len] = '\0';
		
		if (filetype == EXT2_FT_FILE) {
			if (!strcmp(name, filename)) {
				char output[39];
				sprintf(output, "File found. Occupies %d bytes\n", get_inode_size(fd, info, ind));
				write(1, output, strlen(output));
				free(name);
				return 1;
			}
		} else if (filetype == EXT2_FT_DIR) {
			if (strcmp(name, ".") && strcmp(name, "..")) {
				if (EXT2_deep_find(fd, info, filename, ind, level+1)) {
					free(name);				
					return 1;
				}
			}
		}
		
		free(name);
		
		lseek(fd, inode_data_block + rec_len + 4, SEEK_SET);
		uint16_t rec;
		read(fd, &rec, 2);
		
		rec_len += rec;
	} while(rec_len < info.block_size);

	return 0;
}

uint32_t get_inode_size(int fd, ext_info info, int inode) {
	
	int block_group = (inode-1) / info.inodes_per_group;
	int inode_index = (inode-1) % info.inodes_per_group;
	int group_base_offset = block_group * (info.blocks_per_group * info.block_size);
	int bg_offset = group_base_offset + (SUPERBLOCK*2);
	
	lseek(fd, bg_offset + 8, SEEK_SET);
	uint32_t bg_inode_table_block;
	read(fd, &bg_inode_table_block, 4);
	
	int inode_table = group_base_offset + (bg_inode_table_block * info.block_size);
	int inode_offset = inode_table + (inode_index * info.inode_size);

	lseek(fd, inode_offset + 4, SEEK_SET);
	uint32_t inode_size;
	read(fd, &inode_size, 4);

	return inode_size;
}

ext_info EXT2_info(int fd, int flag) {
	
	ext_info info;

	if (lseek(fd, 0, SEEK_CUR) == -1) {
		info.error = 1;
		return info;
	}
	
	uint16_t fs;
	lseek(fd, SUPERBLOCK + 56, SEEK_SET);
	read(fd, &fs, 2);

	if (fs != MAGIC_NUMBER) {
		info.error = 1;
		return info;
	}

	info.error = 0;
	strcpy(info.filesystem, "EXT2");

	lseek(fd, SUPERBLOCK, SEEK_SET);
	read(fd, &info.inodes_count, 4);
	char inodes_count[11];
	sprintf(inodes_count, "%d", info.inodes_count);

	lseek(fd, SUPERBLOCK + 4, SEEK_SET);
	read(fd, &info.blocks_count, 4);
	char blocks_count[11];
	sprintf(blocks_count, "%d", info.blocks_count);

	lseek(fd, SUPERBLOCK + 8, SEEK_SET);
	read(fd, &info.reserved_blocks, 4);
	char reserved_blocks[11];
	sprintf(reserved_blocks, "%d", info.reserved_blocks);

	lseek(fd, SUPERBLOCK + 12, SEEK_SET);
	read(fd, &info.free_blocks, 4);
	char free_blocks[11];
	sprintf(free_blocks, "%d", info.free_blocks);

	lseek(fd, SUPERBLOCK + 16, SEEK_SET);
	read(fd, &info.free_inodes, 4);
	char free_inodes[11];
	sprintf(free_inodes, "%d", info.free_inodes);

	lseek(fd, SUPERBLOCK + 32, SEEK_SET);
	read(fd, &info.blocks_per_group, 4);
	char blocks_per_group[11];
	sprintf(blocks_per_group, "%d", info.blocks_per_group);

	lseek(fd, SUPERBLOCK + 36, SEEK_SET);
	read(fd, &info.frags_per_group, 4);
	char frags_per_group[11];
	sprintf(frags_per_group, "%d", info.frags_per_group);

	lseek(fd, SUPERBLOCK + 40, SEEK_SET);
	read(fd, &info.inodes_per_group, 4);
	char inodes_per_group[11];
	sprintf(inodes_per_group, "%d", info.inodes_per_group);
	
	lseek(fd, SUPERBLOCK + 84, SEEK_SET);
	read(fd, &info.first_inode, 4);
	char first_inode[11];
	sprintf(first_inode, "%d", info.first_inode);

	lseek(fd, SUPERBLOCK + 88, SEEK_SET);
	read(fd, &info.inode_size, 2);
	char inode_size[6];
	sprintf(inode_size, "%d", info.inode_size);

	lseek(fd, SUPERBLOCK + 24, SEEK_SET);
	read(fd, &info.block_size, 4);
	info.block_size = 1024 << info.block_size;
	char block_size[11];
	sprintf(block_size, "%d", info.block_size);
	
	lseek(fd, SUPERBLOCK + 20, SEEK_SET);
	read(fd, &info.first_block, 4);
	char first_block[11];
	sprintf(first_block, "%d", info.first_block);

	lseek(fd, SUPERBLOCK + 120, SEEK_SET);
	read(fd, info.volume_name, 16);
//	volume_name[16] = '\0';
	
	lseek(fd, SUPERBLOCK + 44, SEEK_SET);
	read(fd, &info.last_mount, 4);
	char *last_mount = unixt_to_date(info.last_mount);

	lseek(fd, SUPERBLOCK + 48, SEEK_SET);
	read(fd, &info.last_write, 4);
	char *last_write = unixt_to_date(info.last_write);
	
	lseek(fd, SUPERBLOCK + 64, SEEK_SET);
	read(fd, &info.last_check, 4);
	char *last_check = unixt_to_date(info.last_check);
	
	if (flag == EXT_SHOW) {
		write(1, "\n------ Filesystem Information ------\n\n", strlen("\n------ Filesystem Information ------\n\n"));
	
		write(1, "Filesystem: ", strlen("Filesystem: "));
		write(1, info.filesystem, strlen(info.filesystem));

		write(1, "\n\nINODE INFO\n", strlen("\n\nINODE INFO\n"));
		write(1, "Inode size: ", strlen("Inode size: "));
		write(1, inode_size, strlen(inode_size));
		write(1, "\nNumber of inodes: ", strlen("\nNumber of inodes: "));
		write(1, inodes_count, strlen(inodes_count));
		write(1, "\nFirst inode: ", strlen("\nFirst inode: "));
		write(1, first_inode, strlen(first_inode));
		write(1, "\nInodes per group: ", strlen("\nInodes per group: "));
		write(1, inodes_per_group, strlen(inodes_per_group));
		write(1, "\nFree inodes: ", strlen("\nFree inodes: "));
		write(1, free_inodes, strlen(free_inodes));

		write(1, "\n\nBLOCK INFO\n", strlen("\n\nBLOCK INFO\n"));
		write(1, "Block size: ", strlen("Block size: "));
		write(1, block_size, strlen(block_size));
		write(1, "\nReserved blocks: ", strlen("\nReserved blocks: "));
		write(1, reserved_blocks, strlen(reserved_blocks));
		write(1, "\nFree blocks: ", strlen("\nFree blocks: "));
		write(1, free_blocks, strlen(free_blocks));
		write(1, "\nTotal blocks: ", strlen("\nTotal blocks: "));
		write(1, blocks_count, strlen(blocks_count));
		write(1, "\nFirst block: ", strlen("\nFirst block: "));
		write(1, first_block, strlen(first_block));
		write(1, "\nBlocks per group: ", strlen("\nBlocks per group: "));
		write(1, blocks_per_group, strlen(blocks_per_group));
		write(1, "\nFragments per group: ", strlen("\nFragments per group: "));
		write(1, frags_per_group, strlen(frags_per_group));

		write(1, "\n\nVOLUME INFO\n", strlen("\n\nVOLUME INFO\n"));
		write(1, "Volume name: ", strlen("Volume name: "));
		write(1, info.volume_name, strlen(info.volume_name));
		write(1, "\nLast check: ", strlen("\nLast check: "));
		write(1, last_check, strlen(last_check));
		write(1, "\nLast mount: ", strlen("\nLast mount: "));
		write(1, last_mount, strlen(last_mount));
		write(1, "\nLast write: ", strlen("\nLast write: "));
		write(1, last_write, strlen(last_write));

		write(1, "\n\n", 2);
	}

	free(last_mount);
	free(last_write);
	free(last_check);

	return info;
}

char * unixt_to_date(uint32_t time) {
	time_t t = time;
	struct tm *time_struct;

	time_struct = localtime(&t);

	char *timeformat;
	timeformat = (char *) malloc(100);

	strftime(timeformat, 100, "%c", time_struct);

	return timeformat;
}
