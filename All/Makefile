CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic-errors

all:main # Reseau

main: utility_SDL.o PreTreatment.o Segmentation.o interface.o Reseau.o #main.o
	gcc *.c -o main -lm -lSDL2-2.0 -lSDL2_image -lSDL2_ttf

#Reseau: XOR.o
#	gcc *.c -o main -lm

interface: interface.o
	gcc interface.c PreTreatment.c utility_SDL.c Segmentation.c -o interface -lSDL2-2.0 -lSDL2_image -lSDL2_ttf

clean:
	rm -f *.o
	rm -f main
	rm -f *~

clean_save:
	rm -f *.o
	rm -f main
	rm -f *~
	rm -f save.txt

