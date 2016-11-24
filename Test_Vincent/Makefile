CC=gcc
CPPFLAGS=-MMD
CFLAGS=-Wall -Wextra -std=c99 -O2
LDFLAGS=
LDLIBS=-lm

SRC= XOR_function.c
OBJ=${SRC:.c=.o}
DEP=${SRC:.c=.d}
PRG=${SRC:.c=}

all: ${PRG}

XOR: XOR_function

SEG: 

clean:
	${RM} ${OBJ} ${DEP} ${PRG}

-include ${DEP}
