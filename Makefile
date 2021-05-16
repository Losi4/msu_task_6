all: main
clean:
	rm -f *.o
main: main.o functions.o
	gcc -m32 main.o functions.o -lm -o main
main.o: main.c
	gcc -m32 main.c -c
functions.o: functions.asm
	nasm -f elf32 functions.asm
