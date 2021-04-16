all: compilation
compilation: shooter.o
	gcc shooter.c -o shooter
