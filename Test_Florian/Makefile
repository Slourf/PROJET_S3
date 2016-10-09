# Simple Makefile

# Compilers vars
CC=gcc
CPPFLAGS= -MMD
CFLAGS= -Wall -Wextra -std=c99 -O2
LDFLAGS=
LDLIBS=

SRC= gtk.c
DEP= ${SRC:.c=.d}
PRG= ${SRC:.c=}

all: ${PRG}

-include ${DEP}

clean:
        rm -f ${DEP}
        rm -f ${PRG}

# END Makefile

