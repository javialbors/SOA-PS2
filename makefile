all: compilation
fat16.o: fat16.c fat16.h
	gcc -c fat16.c
ext2.o: ext2.c ext2.h
	gcc -c ext2.c
compilation: shooter.o fat16.o ext2.o
	gcc shooter.c fat16.c ext2.c -o shooter
