all: compilation
fat16.o: fat16.c fat16.h
	gcc -c fat16.c
compilation: shooter.o fat16.o
	gcc shooter.c fat16.c -o shooter
