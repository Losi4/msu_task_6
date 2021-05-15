all: main
clean:
	rm main.o functions.o
main: main.o functions.o
	gcc -m32 -o main main.o functions.o
main.o: main.c
	gcc -m32 -c -o main.o main.c
functions.o: functions.asm
	nasm -f elf32 -o functions.o functions.asm
