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

struct tTuple {
	int x_l, x_u;
	int y_l, y_u;
};

struct coord { int x, y; };

struct tuple {
    coord *coord;
    int length;
};

void wait_for_keypressed(void);

void init_sdl(void);

SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);
SDL_Surface to_black_white(SDL_Surface *img);

long** build_matrix(size_t x, size_t y);

# endif
