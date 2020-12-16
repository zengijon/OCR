CC = gcc
CFLAGS = `pkg-config --cflags --libs sdl gtk+-3.0` -Wall -Wextra -std=c99 -pedantic-errors
LDFLAGS = -lm -lSDL2-2.0 -lSDL2_image -lSDL2_ttf
LDLIBS = `pkg-config --libs sdl gtk+-3.0` 
SRC = main.c \
	PreTreatment.c \
	utility_SDL.c \
	Segmentation.c \
	Reseau.c

OBJ = $(SRC:.c=.o)

all:main

main: $(OBJ)
	  $(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f *.o
	rm -f main
	rm -f *~

save_clean:
	rm -f *.o
	rm -f main
	rm -f *~
	rm -f save.txt	

