#include "fat16.h"

void FAT_delete(int fd, fat_info info, char *filename) {
	fat_file_info file_info = FAT_find(fd, info, filename, FAT_CHECK);
	
	if (file_info.found) {
		int e5 = FAT_DELETED_FILE;
		lseek(fd, file_info.fat_location, SEEK_SET);
		write(fd, &e5, 1);

		char *output;
		output = malloc(strlen("File '' has been removed\n") + strlen(filename));
		sprintf(output, "File '%s' has been removed\n", filename);
		write(1, output, strlen(output));
		free(output);
	}
}

fat_file_info FAT_find(int fd, fat_info info, char *filename, int mode) {
	int base_offset = (info.reserved + (info.nfats * info.sectors_per_fat)) * info.sector_size;
	
	fat_file_info file_info = FAT_deep_find(fd, info, filename, base_offset, base_offset, mode);

	if (!file_info.found) {
		write(1, "ERROR - File not found\n", strlen("ERROR - File not found\n"));
	}

	return file_info;
}

fat_file_info FAT_deep_find(int fd, fat_info info, char *filename, int base_offset, int cluster_offset, int mode) {
	int max_offset = info.max_root_entries * FAT_ENTRY;
		
	int current = 0;
	int i=0;
	do {
		char file[9];
		lseek(fd, cluster_offset + (FAT_ENTRY*i), SEEK_SET);
		read(fd, file, 8);
		file[8] = '\0';
		
		if (file[0] == FAT_DELETED_FILE) {
			i++;
			current += FAT_ENTRY;
			continue;
		}

		reformat(file);

		uint8_t attribute;
		lseek(fd, cluster_offset + (FAT_ENTRY*i) + ENTRY_ATTRIBUTE, SEEK_SET);
		read(fd, &attribute, 1);
		
		char file_with_ext[13];

		if (attribute == 0) break;
		
		if (attribute == FAT_LONG_NAME) {
			i++;
			current += FAT_ENTRY;
			continue;
		}
		
		if (attribute == FAT_FILE || attribute == FAT_SUBDIR) {
			
			uint16_t cluster;
			lseek(fd, cluster_offset + (FAT_ENTRY*(i)) + ENTRY_CLUSTER_START, SEEK_SET);
			read(fd, &cluster, 2);

			int data_offset = ((base_offset / info.sector_size) + info.sectors_per_fat + ((cluster - 2) * info.sectors_per_cluster)) * info.sector_size;

			if (attribute == FAT_FILE) {
				char ext[4];
				lseek(fd, cluster_offset + (FAT_ENTRY*i) + ENTRY_EXTENSION, SEEK_SET);
				read(fd, ext, 3);
				ext[3] = '\0';
			
				reformat(ext);
				if (strcmp(ext, "")) sprintf(file_with_ext, "%s.%s", file, ext);
				else (strcpy(file_with_ext, file));
				
				if (!strcmp(file_with_ext, filename)) {
					if (mode == FAT_SHOW) {
						uint32_t file_size;
						lseek(fd, cluster_offset + (FAT_ENTRY*(i)) + ENTRY_SIZE, SEEK_SET);
						read(fd, &file_size, 4);

						char output[39];
						sprintf(output, "File found. Occupies %d bytes\n", file_size);
						write(1, output, strlen(output));
					}

					fat_file_info file_info;
					file_info.found = 1;
					file_info.fat_location = cluster_offset + (FAT_ENTRY*i);

					return file_info;
				}
			} 
		
			if (attribute == FAT_SUBDIR) {
				if (strcmp(file, ".") && strcmp(file, "..")) {
					
					fat_file_info file_info = FAT_deep_find(fd, info, filename, base_offset, data_offset, mode);
					if (file_info.found) return file_info;
				}	
			}
		}
		
		i++;
		current += FAT_ENTRY;	
	} while (current < max_offset);
	
	fat_file_info file_info;
	file_info.found = 0;

	return file_info;
}

fat_info FAT_info(int fd, int flag) {

	fat_info info;
	
	if (lseek(fd, 0, SEEK_CUR) == -1) {
		info.error = 1;
		return info;
	}
    
	lseek(fd, FAT_SYSTYPE, SEEK_SET);
    read(fd, info.filesystem, 8);
	info.filesystem[8] = '\0';
		
	if (!(info.filesystem[0] == 'F' && info.filesystem[1] == 'A' && info.filesystem[2] == 'T')) {
		info.error = 1;
		return info;
	}

	info.error = 0;
	
	lseek(fd, FAT_NAME, SEEK_SET);
    read(fd, info.system_name, 8);
	info.system_name[8] = '\0';
    
    lseek(fd, FAT_BPS, SEEK_SET);
    read(fd, &info.sector_size, 2);

    lseek(fd, FAT_SPC, SEEK_SET);
    read(fd, &info.sectors_per_cluster, 1);

    lseek(fd, FAT_RESERVED, SEEK_SET);
    read(fd, &info.reserved, 2);

    lseek(fd, FAT_NFATS, SEEK_SET);
    read(fd, &info.nfats, 1);

    lseek(fd, FAT_ROOT_ENTRIES, SEEK_SET);
    read(fd, &info.max_root_entries, 2);

    lseek(fd, FAT_SIZE, SEEK_SET);
    read(fd, &info.sectors_per_fat, 2);

    lseek(fd, FAT_LABEL, SEEK_SET);
    read(fd, info.label, 11);
	info.label[11] = '\0';
	
	if (flag == FAT_SHOW) {
		char sector_size[6];
	    sprintf(sector_size, "%d", info.sector_size);

		char sectors_per_cluster[4];
	    sprintf(sectors_per_cluster, "%d", info.sectors_per_cluster);

		char reserved[6];
	    sprintf(reserved, "%d", info.reserved);

		char nfats[4];
	    sprintf(nfats, "%d", info.nfats);

		char max_root_entries[6];
	    sprintf(max_root_entries, "%d", info.max_root_entries);

		char sectors_per_fat[6];
	    sprintf(sectors_per_fat, "%d", info.sectors_per_fat);

	    write(1, "\n------ Filesystem Information ------\n\n", strlen("\n------ Filesystem Information ------\n\n"));
    	write(1, "Filesystem: ", strlen("Filesystem: "));
  		write(1, info.filesystem, strlen(info.filesystem));
	    write(1, "\nSystem Name: ", strlen("\nSystem Name: "));
	    write(1, info.system_name, strlen(info.system_name));
   		write(1, "\nSector size: ", strlen("\nSector size: "));
	    write(1, sector_size, strlen(sector_size));
	    write(1, "\nSectors per cluster: ", strlen("\nSectors per cluster: "));
	    write(1, sectors_per_cluster, strlen(sectors_per_cluster));
	    write(1, "\nReserved sectors: ", strlen("\nReserved sectors: "));
	    write(1, reserved, strlen(reserved));
	    write(1, "\nNumber of FATs: ", strlen("\nNumber of FATs: "));
	    write(1, nfats, strlen(nfats));
    	write(1, "\nMaxRootEntries: ", strlen("\nMaxRootEntries: "));
	    write(1, max_root_entries, strlen(max_root_entries));
	    write(1, "\nSectors per FAT: ", strlen("\nSectors per FAT: "));
	    write(1, sectors_per_fat, strlen(sectors_per_fat));
	    write(1, "\nLabel: ", strlen("\nLabel: "));
    	write(1, info.label, strlen(info.label));
	    write(1, "\n\n", 2);
	}

	return info;
}

void reformat(char *file) {
	for (int i=0; i<8; i++) {
		if (file[i] == ' ') {
			file[i] = '\0';
			return;
		}
	}
}
