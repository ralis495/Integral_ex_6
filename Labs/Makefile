.PHONY: all clean arc

all: integral

functions.o: functions.asm
	nasm -f elf32 -o functions.o functions.asm

main.o: main.c
	gcc -m32 -c -O3 -std=c99 main.c -o main.o -fno-PIC

integral: main.o functions.o
	gcc -m32 main.o functions.o -o integral -no-pie -lm

clean:
	rm -f *.o integral

arc:
	mkdir -p ARC
	zip -r ARC/project.zip integral main.c Makefile functions.asm