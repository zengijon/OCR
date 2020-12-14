CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic-errors
LDFLAGS = -lm -lSDL2-2.0 -lSDL2_image -lSDL2_ttf
SRC = interface.c \
	PreTreatment.c \
	utility_SDL.c \
	Segmentation.c \
	interface.c \
	Reseau.c \
	vector.c

OBJ = $(SRC:.c=.o)

all:main

main: $(OBJ)
	  $(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o
	rm -f main
	rm -f *~

