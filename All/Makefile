CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic-errors

all:main

main: utility_SDL.o main.o PreTreatment.o Segmentation.o
	gcc *.c -o main -lSDL2-2.0 -lSDL2_image

interface: interface.o
	gcc interface.c -o interface -lSDL2-2.0 -lSDL2_image -lSDL2_ttf

clean:
	rm -f *.o
	rm -f main
	rm -f *~
