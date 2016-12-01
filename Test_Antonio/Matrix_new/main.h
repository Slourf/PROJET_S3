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
    struct coord *coord;
    int length;
};

struct chat {
    int length;
    long ***line;
    int nbline;
};
void wait_for_keypressed(void);

void init_sdl(void);
void img2mat(SDL_Surface *img, long **mat);

SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);
SDL_Surface to_black_white(SDL_Surface *img);

long** build_matrix(size_t x, size_t y);
void print_dynmat(long **mat, size_t x, size_t y);
void free_matrix(long **mat, int w);
struct tTuple block_cut (long **mat, int width, int height);
void copy(long **old_mat, long **new_mat, int x_l, int x_u, int y_l, int y_u);
struct tuple line_cut(long **mat, int width, int height);
struct tuple char_cut(long **mat, int width, int height);
void stock_lines(long ***lines, long** img, int width, struct tuple coord);
void stock_char(struct chat *chat, long ***lines, struct tuple nb_line, int width, int char_size);
struct chat *cut(SDL_Surface *img);
# endif
