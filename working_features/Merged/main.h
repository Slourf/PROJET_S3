//main.h

# ifndef MAIN_H_
# define MAIN_H_

# include <err.h>
# include <stdio.h>
# include <stdlib.h>

# include "pixel_operations.h"
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>

struct tTuple {
	int x_l, x_u;
	int y_l, y_u;
};

struct coord { int x, y; };


struct tuple {
    struct coord *coord;
    int length;
};

struct text {
	struct line **line;
    size_t size;
};

struct block {
        size_t Xori, Yori, w, h;
        struct block *next;
};

struct matrix {
        long **data;
        size_t h;
        size_t w;
};

struct lines {
        struct matrix **mat;
        int *Xori, *Yori;
        size_t size;
};

struct line {
	struct matrix **mat;
	size_t size;
};

void wait_for_keypressed(void);

void init_sdl(void);
void img2mat(SDL_Surface *img, struct matrix *mat);

SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);
SDL_Surface to_black_white(SDL_Surface *img);

struct matrix* build_matrix(size_t x, size_t y);
struct line* build_line(size_t length);
struct text* build_text(size_t length);

void print_dynmat(struct matrix *mat);
void free_matrix(struct matrix *mat);
void free_line(struct line *l);
void free_text(struct text *text);

struct tTuple block_cut (struct matrix *mat);
void copy(struct matrix *old_mat, struct matrix *new_mat, int x_l,
int x_u, int y_u);
struct tuple line_cut(struct matrix *mat);
struct tuple char_cut(struct matrix *mat);
void stock_lines(struct line *line, struct matrix *img, struct tuple coord);
void stock_char(struct text *text, struct line *line, struct tuple nb_line,
int char_size);
struct text *cut(SDL_Surface *img);


/////////////////////////////////////////////
///////////////////RLSA//////////////////////
/////////////////////////////////////////////

struct matrix* init_matrix(size_t w, size_t h);
struct lines* init_lines(size_t size);
void rlsa_vec(struct matrix *ori, struct matrix *mat, int c);
void rlsa_hor(struct matrix *ori, struct matrix *mat, int c);
void rlsa_merge(struct matrix *mat_img, struct matrix *mat_vec,
struct matrix *mat_hor);
struct block* append_block(struct block *block, struct block *new);
struct lines* stock_lines_rlsa(struct matrix *img, struct tuple coord) ;
struct block* block_rlsa_cut(struct matrix *matrix, struct block* list,
int x, int y);
void display(struct matrix *mat);

void rlsa(SDL_Surface *img, struct matrix *mat);

///////////////////////////////////////////////
///////////////////Resize//////////////////////
///////////////////////////////////////////////

struct matrix* resize_char(struct matrix *mat, int size);

# endif
