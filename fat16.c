#include "fat16.h"
#include <stdlib.h>

int FAT_info(int fd) {

	if (lseek(fd, 0, SEEK_CUR) == -1) return 0;

    char filesystem[9];
    char system_name[9];
    char sector_size[6];
    char sectors_per_cluster[4];
    char reserved[6];
    char nFats[4];
    char max_root_entries[4];
    char sectors_per_fat[6];
    char label[12];
   
    lseek(fd, 54, SEEK_SET);
    read(fd, filesystem, 8);
	filesystem[8] = '\0';
		
	if (!(filesystem[0] == 'F' && filesystem[1] == 'A' && filesystem[2] == 'T')) return 0;

	lseek(fd, 3, SEEK_SET);
    read(fd, system_name, 8);
	system_name[8] = '\0';
    
    uint16_t bps;
    lseek(fd, 11, SEEK_SET);
    read(fd, &bps, 2);
    sprintf(sector_size, "%d", bps);

    uint8_t spc;
    lseek(fd, 13, SEEK_SET);
    read(fd, &spc, 1);
    sprintf(sectors_per_cluster, "%d", spc);

    uint16_t rs;
    lseek(fd, 14, SEEK_SET);
    read(fd, &rs, 2);
    sprintf(reserved, "%d", rs);

    uint8_t fats;
    lseek(fd, 16, SEEK_SET);
    read(fd, &fats, 1);
    sprintf(nFats, "%d", fats);

    uint16_t mre;
    lseek(fd, 17, SEEK_SET);
    read(fd, &mre, 2);
    sprintf(max_root_entries, "%d", mre);

    uint16_t spf;
    lseek(fd, 22, SEEK_SET);
    read(fd, &spf, 2);
    sprintf(sectors_per_fat, "%d", spf);

    lseek(fd, 43, SEEK_SET);
    read(fd, label, 11);
	label[11] = '\0';

    write(1, "\n------ Filesystem Information ------\n\n", strlen("\n------ Filesystem Information ------\n\n"));
    write(1, "Filesystem: ", strlen("Filesystem: "));
    write(1, filesystem, strlen(filesystem));
    write(1, "\nSystem Name: ", strlen("\nSystem Name: "));
    write(1, system_name, strlen(system_name));
    write(1, "\nSector size: ", strlen("\nSector size: "));
    write(1, sector_size, strlen(sector_size));
    write(1, "\nSectors per cluster: ", strlen("\nSectors per cluster: "));
    write(1, sectors_per_cluster, strlen(sectors_per_cluster));
    write(1, "\nReserved sectors: ", strlen("\nReserved sectors: "));
    write(1, reserved, strlen(reserved));
    write(1, "\nNumber of FATs: ", strlen("\nNumber of FATs: "));
    write(1, nFats, strlen(nFats));
    write(1, "\nMaxRootEntries: ", strlen("\nMaxRootEntries: "));
    write(1, max_root_entries, strlen(max_root_entries));
    write(1, "\nSectors per FAT: ", strlen("\nSectors per FAT: "));
    write(1, sectors_per_fat, strlen(sectors_per_fat));
    write(1, "\nLabel: ", strlen("\nLabel: "));
    write(1, label, strlen(label));
    write(1, "\n", 1);

    close(fd);

	return 1;
}
