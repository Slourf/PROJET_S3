CC= gcc

NAME= myguiapp
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3 
GTKFLAGS= -export-dynamic `pkg-config --cflags --libs gtk+-2.0`

SRCS= gtk.c 

# top-level rule to create the program.
all: main

# compiling the source file.
main: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(GTKFLAGS) -o gtk

# cleaning everything that can be automatically recreated with "make".
clean:
	rm -f *~ *.o
	rm -f gtk
