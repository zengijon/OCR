CC = gcc
CFLAGS = -Wall -Wextra -pedantic-errors

all:main

main: utility_SDL.o main.o PreTreatment.o Segmentation.o
	gcc *.c -o main -lSDL2-2.0 -lSDL2_image

clean:
	rm -f *.o
	rm -f main
	rm -f *~
