CC=gcc
CPPFLAGS=-MMD `pkg-config --cflags sdl`
CFLAGS=-Wall -Wextra -std=c99 -O2 -g
LDFLAGS=
LDLIBS=-lm `pkg-config --libs sdl` -lSDL_image

SRC= IntToBin_Net.c
OBJ=${SRC:.c=.o}
CUT= main.o pixel_operations.o
DEP=${SRC:.c=.d}
PRG=${SRC:.c=}

all: IntToBin_Net main

main: ${CUT}

IntToBin_net: IntToBin_Net

clean:
	${RM} ${OBJ} ${DEP} ${PRG}

-include ${DEP}
