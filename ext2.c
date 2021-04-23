#include "ext2.h"

char * unixt_to_date(uint32_t time) {
	time_t t = time;
	struct tm *time_struct;

	time_struct = localtime(&t);

	char *timeformat;
	timeformat = (char *) malloc(100);

	strftime(timeformat, 100, "%c", time_struct);

	return timeformat;
}

int EXT2_info(int fd, int flag) {
	
	if (lseek(fd, 0, SEEK_CUR) == -1) return 0;
	
	char filesystem[5];
	char inodes_count[11];
	char blocks_count[11];
	char reserved_blocks[11];
	char free_blocks[11];
	char free_inodes[11];
	char blocks_per_group[11];
	char frags_per_group[11];
	char inodes_per_group[11];
	char first_inode[11];
	char inode_size[6];
	char block_size[11];
	
	char volume_name[17];
	char *last_mount;
	char *last_write;
	char *last_check;

	uint16_t fs;
	lseek(fd, SUPERBLOCK + 56, SEEK_SET);
	read(fd, &fs, 2);

	if (fs != MAGIC_NUMBER) return 0;
	
	if (flag != EXT_SHOW) {
		close(fd);
		return 1;
	}

	strcpy(filesystem, "EXT2");

	uint32_t inc;
	lseek(fd, SUPERBLOCK, SEEK_SET);
	read(fd, &inc, 4);
	sprintf(inodes_count, "%d", inc);

	uint32_t bc;
	lseek(fd, SUPERBLOCK + 4, SEEK_SET);
	read(fd, &bc, 4);
	sprintf(blocks_count, "%d", bc);

	uint32_t rb;
	lseek(fd, SUPERBLOCK + 8, SEEK_SET);
	read(fd, &rb, 4);
	sprintf(reserved_blocks, "%d", rb);

	uint32_t fb;
	lseek(fd, SUPERBLOCK + 12, SEEK_SET);
	read(fd, &fb, 4);
	sprintf(free_blocks, "%d", fb);

	uint32_t fi;
	lseek(fd, SUPERBLOCK + 16, SEEK_SET);
	read(fd, &fi, 4);
	sprintf(free_inodes, "%d", fi);

	uint32_t bpg;
	lseek(fd, SUPERBLOCK + 32, SEEK_SET);
	read(fd, &bpg, 4);
	sprintf(blocks_per_group, "%d", bpg);

	uint32_t fpg;
	lseek(fd, SUPERBLOCK + 36, SEEK_SET);
	read(fd, &fpg, 4);
	sprintf(frags_per_group, "%d", fpg);

	uint32_t ipg;
	lseek(fd, SUPERBLOCK + 40, SEEK_SET);
	read(fd, &ipg, 4);
	sprintf(inodes_per_group, "%d", ipg);
	
	uint32_t fin;
	lseek(fd, SUPERBLOCK + 84, SEEK_SET);
	read(fd, &fin, 4);
	sprintf(first_inode, "%d", fin);

	uint16_t is;
	lseek(fd, SUPERBLOCK + 88, SEEK_SET);
	read(fd, &is, 2);
	sprintf(inode_size, "%d", is);

	uint32_t bs;
	lseek(fd, SUPERBLOCK + 24, SEEK_SET);
	read(fd, &bs, 4);
	bs = 1024 << bs;
	sprintf(block_size, "%d", bs);

	lseek(fd, SUPERBLOCK + 120, SEEK_SET);
	read(fd, volume_name, 16);
//	volume_name[16] = '\0';
	
	uint32_t lm;
	lseek(fd, SUPERBLOCK + 44, SEEK_SET);
	read(fd, &lm, 4);
	last_mount = unixt_to_date(lm);

	uint32_t lw;
	lseek(fd, SUPERBLOCK + 48, SEEK_SET);
	read(fd, &lw, 4);
	last_write = unixt_to_date(lw);
	
	uint32_t lc;
	lseek(fd, SUPERBLOCK + 64, SEEK_SET);
	read(fd, &lc, 4);
	last_check = unixt_to_date(lc);
	
	write(1, "\n------ Filesystem Information ------\n\n", strlen("\n------ Filesystem Information ------\n\n"));
	
	write(1, "Filesystem: ", strlen("Filesystem: "));
	write(1, filesystem, strlen(filesystem));

	write(1, "\n\nINODE INFO\n", strlen("\n\nINODE INFO\n"));
	write(1, "Inode size: ", strlen("Inode size: "));
	write(1, inode_size, strlen(inode_size));
	write(1, "\nNumber of inodes: ", strlen("\nNumber of inodes: "));
	write(1, inodes_count, strlen(inodes_count));
	write(1, "\nFirst inode: ", strlen("\nFirst inode: "));
	write(1, first_inode, strlen(first_inode));
	write(1, "\nFree inodes: ", strlen("\nFree inodes: "));
	write(1, free_inodes, strlen(free_inodes));
	write(1, "\nInodes per group: ", strlen("\nInodes per group: "));
	write(1, inodes_per_group, strlen(inodes_per_group));

	write(1, "\n\nBLOCK INFO\n", strlen("\n\nBLOCK INFO\n"));
	write(1, "Block size: ", strlen("Block size: "));
	write(1, block_size, strlen(block_size));
	write(1, "\nReserved blocks: ", strlen("\nReserved blocks: "));
	write(1, reserved_blocks, strlen(reserved_blocks));
	write(1, "\nFree blocks: ", strlen("\nFree blocks: "));
	write(1, free_blocks, strlen(free_blocks));
	write(1, "\nTotal blocks: ", strlen("\nTotal blocks: "));
	write(1, blocks_count, strlen(blocks_count));
	write(1, "\nBlocks per group: ", strlen("\nBlocks per group: "));
	write(1, blocks_per_group, strlen(blocks_per_group));
	write(1, "\nFragments per group: ", strlen("\nFragments per group: "));
	write(1, frags_per_group, strlen(frags_per_group));

	write(1, "\n\nVOLUME INFO\n", strlen("\n\nVOLUME INFO\n"));
	write(1, "Volume name: ", strlen("Volume name: "));
	write(1, volume_name, strlen(volume_name));
	write(1, "\nLast check: ", strlen("\nLast check: "));
	write(1, last_check, strlen(last_check));
	write(1, "\nLast mount: ", strlen("\nLast mount: "));
	write(1, last_mount, strlen(last_mount));
	write(1, "\nLast write: ", strlen("\nLast write: "));
	write(1, last_write, strlen(last_write));

	write(1, "\n\n", 2);

	close(fd);

	return 1;
}
