//main.h

# ifndef MAIN_H_
# define MAIN_H_

# include <err.h>
# include <stdio.h>
# include <stdlib.h>

# include "pixel_operations.h"
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>

# include "resize.h"

typedef struct {
	int x_l;
	int x_u;
	int y_l;
	int y_u;
} tTuple;

typedef struct {
    int x;
    int y;
} coord;

typedef struct {
    coord *coord;
    int length;
} tuple;

void wait_for_keypressed(void);

void init_sdl(void);

SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);
SDL_Surface to_black_white(SDL_Surface *img);

long** build_matrix(size_t x, size_t y);

# endif
