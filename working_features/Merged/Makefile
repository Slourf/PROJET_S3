CC= gcc 


CPPFLAGS= `pkg-config gtk+-2.0 --cflags` `pkg-config --cflags sdl`
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3 
LDFLAGS=
LDLIBS= -export-dynamic `pkg-config --libs sdl` `pkg-config gtk+-2.0 --libs` -lSDL_image

SRC= gtk.c main.c rlsa.c resize.c pixel_operations.c

OBJ= ${SRC:.c=.o}

all: main

main: ${OBJ}

clean:
	rm -f *~ *.o
	rm -f main 
