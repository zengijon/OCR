CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic-errors

all:main

main: TestReseau.o
	gcc *.c -o main -lm

clean:
	rm -f *.o
	rm -f main
	rm -f *~

clean_save:
	rm -f *.o
	rm -f main
	rm -f *~
	rm -f save.txt
