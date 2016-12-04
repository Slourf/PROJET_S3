# define _XOPEN_SOURCE 500

# include <err.h>
# include <stdio.h>
# include <stdlib.h>

# include"pixel_operations.h"
# include<SDL/SDL.h>
# include<SDL/SDL_image.h>

# include "main.h"


void wait_for_keypressed(void) {
  SDL_Event             event;
  // Infinite loop, waiting for event
  for (;;) {
    // Take an event
    SDL_PollEvent( &event );
    // Switch on event type
    switch (event.type) {
    // Someone pressed a key -> leave the function
    case SDL_KEYDOWN: return;
    default: break;
    }
  // Loop until we got the expected event
  }
}



void init_sdl(void) {
  // Init only the video part
  if( SDL_Init(SDL_INIT_VIDEO)==-1 ) {
    // If it fails, die with an error message
    errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
  }
  // We don't really need a function for that ...
}


SDL_Surface* load_image(char *path) {
  SDL_Surface          *img;
  // Load an image using SDL_image with format detection
  img = IMG_Load(path);
  if (!img)
    // If it fails, die with an error message
    errx(3, "can't load %s: %s", path, IMG_GetError());
  return img;
}



SDL_Surface* display_image(SDL_Surface *img) {
  SDL_Surface          *screen;
  // Set the window to the same size as the image
  screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
  if ( screen == NULL ) {
    // error management
    errx(1, "Couldn't set %dx%d video mode: %s\n",
         img->w, img->h, SDL_GetError());
  }
 
  /* Blit onto the screen surface */
  if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
    warnx("BlitSurface error: %s\n", SDL_GetError());
 
  // Update the screen
  SDL_UpdateRect(screen, 0, 0, img->w, img->h);
 
  // wait for a key
  wait_for_keypressed();
 
  // return the screen for further uses
  return screen;
}

SDL_Surface to_black_white(SDL_Surface *img) {
    for (int x = 0; x < img->w; ++x) {
        for (int y = 0; y < img->h; ++y) {
            Uint8 r, g, b; 
            Uint32 pixel = getpixel(img, x, y);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            r *= 0.3f;
            g *= 0.59f;
            b *= 0.11f;
            float lumi = r + g + b;
            if (lumi < 90)
                lumi = 0;
            else
                lumi = 255;
            pixel = SDL_MapRGB(img->format, lumi, lumi, lumi);
            putpixel(img, x, y, pixel);
        }
    }
    return *img;
}

struct matrix* build_matrix(size_t w, size_t h) {
    struct matrix *m = malloc(sizeof (struct matrix));
	long **mat;
    mat = calloc(w, sizeof (long *));
	for (size_t i = 0; i < w; ++i)
		mat[i] = calloc(h, sizeof (long));
	m->data = mat;
	m->h = h;
	m->w = w;
    return m;
}	

struct line* build_line(size_t length) {
	struct line *l = calloc(1, sizeof (struct line));
	l->size = length;
	l->mat = calloc(length, sizeof (struct matrix*));
	return l;
}

struct text* build_text(size_t length) {
	struct text *txt = calloc(1, sizeof (struct text));
	txt->size = length;
	txt->line = calloc(length, sizeof (struct line*));
	printf("len = %zu\n", txt->size);
	return txt;
}

void free_matrix(struct matrix *mat) {
	for (size_t i = 0; i < mat->w; ++i)
		free(*(mat->data + i));
	free(mat);
}


void img2mat(SDL_Surface *img, struct matrix *mat) {
    for (int x = 0; x < img->w; ++x) {
        for (int y = 0; y < img->h; ++y) {
            Uint8 r, g, b;
            Uint32 pixel = getpixel(img, x, y);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
			if (r < 128)
				mat->data[x][y] = 1;
			else
            	mat->data[x][y] = 0;
        }
    }
}

void print_dynmat(struct matrix *mat) {
    for (size_t j = 0; j < mat->h; ++j) {
        for (size_t i = 0; i < mat->w; ++i) 
            printf("%ld", mat->data[i][j]);
        printf("\n");
    }
}


int get_upper_y(struct matrix *mat) {
	int b = 1;
	size_t y = 0;
	int lineY = 0;

	while (b == 1 && y < mat->h) {
		size_t x = 0;
							     
		while (b == 1 && x < mat->w) {
			if(mat->data[x][y] == 1)
				b = 0;
			++x;
		}
		if (b == 1)
			++lineY;
		++y;
	}
	return lineY;
}   

int get_lower_y(struct matrix *mat) {
	int b = 1;
	int y = mat->h - 1;
	int lineY = mat->h - 1;

	while (b == 1 && y >= 0) {
		size_t x = 0;
		
		while (b == 1 && x < mat->w) {
			if (mat->data[x][y] == 1)
				b = 0;
			++x;
		}
		if (b == 1)
			--lineY;
		--y;
	}
	return lineY;
}

int get_upper_x(struct matrix *mat) {
    int b = 1;
    size_t x = 0;
    int columnX = 0;

    while (b == 1 && x < mat->w) {
        size_t y = 0;
    
        while (b == 1 && y < mat->h) {
            if(mat->data[x][y] == 1)
                b = 0;
            ++y;
        }
        if (b == 1)
            ++columnX;
        ++x;
    }   
    return columnX;
}   

int get_lower_x(struct matrix *mat) {
    int b = 1;
    int x = mat->w - 1;
    int columnX = mat->w - 1;

    while (b == 1 && x >= 0) {
        size_t y = 0;
    
        while (b == 1 && y < mat->h) {
            if (mat->data[x][y] == 1)
                b = 0;
            ++y;
        }
        if (b == 1)
           --columnX;
        --x;
    }   
    return columnX;
}

void copy(struct matrix *old_mat, struct matrix *new_mat, int x_l, int x_u, int y_u) {

	for (int i = 0; i <= (x_l - x_u); ++i) {
		for(size_t j = 0 ; j < new_mat->h; ++j) {
			new_mat->data[i][j] = old_mat->data[x_u + i][y_u + j];
		}
	}
}	
		
struct tTuple block_cut (struct matrix *mat) {
	int y_l = get_lower_y(mat);
	int y_u = get_upper_y(mat);
	int x_l = get_lower_x(mat);
	int x_u = get_upper_x(mat);
	
	struct tTuple t;
	if (y_l <= y_u) {
		t.x_l = mat->w;
		t.x_u = 0;
		t.y_l = mat->h;
		t.y_u = 0;
	}
	else {
		t.x_l = x_l;
		t.x_u = x_u;
		t.y_l = y_l;
		t.y_u = y_u;
	}
	return t;
}

struct tuple line_cut(struct matrix *mat) {
	int y_top = 0;
	int w = 0;
	int b = 1;
	int c = 0;
	int nbLine = 0;

	struct coord *list = NULL;
	struct tuple t;
	for (size_t y = 0; y < mat->h; ++y) {
		size_t x = 0;
		c = 0;
		while (x < mat->w && c == 0) {
			if (mat->data[x][y] == 1) {
				c = 1;
			}
			++x;
		}
		if(c == 1) {
			if (w == 1) {
				y_top = y;
				w = 0;
			}
			if (y + 1 == mat->h){
        			++nbLine;

				list = realloc(list, nbLine * sizeof(struct coord));
				list[nbLine - 1].x = y_top;
				list[nbLine - 1].y = y;
			}
			b = 1;
		}
		else {
			if (b == 1) {
				++nbLine;
				
				list = realloc(list, nbLine * sizeof(struct coord));
				list[nbLine - 1].x = y_top;
				list[nbLine - 1].y = y;

				b = 0;
			}
			w = 1;
		}
	}
	t.coord = list;
	t.length = nbLine;
	return t;
}
				
void stock_lines(struct line *line, struct matrix *img, struct tuple coord) {
	for (int i = 0; i < coord.length; ++i) {
		struct matrix *m = build_matrix(img->w, coord.coord[i].y - coord.coord[i].x + 1);
		copy(img, m, img->w - 1, 0, coord.coord[i].x);
		line->mat[i] = m;
	}
}


struct tuple char_cut(struct matrix *mat)  
{       
    int x_top = 0;
    int w = 0;
    int b = 0;
    int c = 0;
    int nbchar = 0;
    int blank_count = 0;
    int aver_size_char = 0;
	int not_first = 0;
    struct coord *list = NULL;
    struct tuple t;
    for (size_t x = 0; x < mat->w; ++x) {

		size_t y = 0;
        c = 0;
        while (y < mat->h && c == 0) {
	    	if (mat->data[x][y] == 1) {
				c = 1;
        	}
            ++y;
    	}
    	if(c == 1) {
	    	if (w == 1) {
				if (blank_count > aver_size_char + 1) {
		   			++nbchar;
		   			list = realloc(list, nbchar * sizeof(struct coord));
		   			list[nbchar - 1].x = x_top+blank_count-1;
		   			list[nbchar - 1].y = x - 1;
		   			blank_count = 0;
					not_first = 1;
				}
				x_top = x;
            	w = 0;
            }
	    	if (x + 1 == mat->w){
	       		++nbchar;
	       		aver_size_char = (aver_size_char * (nbchar - 1) + x - x_top) / nbchar;
	       		list = realloc(list, nbchar * sizeof(struct coord));
               	list[nbchar - 1].x = x_top;
               	list[nbchar - 1].y = x;
            }
        	b = 1;
    	}
    	else {
			++blank_count;
            if (blank_count > aver_size_char + 1 && not_first) { 
                ++nbchar;
                list = realloc(list, nbchar * sizeof(struct coord));
                list[nbchar - 1].x = x_top + blank_count - 1;
                list[nbchar - 1].y = x;
                blank_count = 0;
            }

			if (b == 1) {
				++nbchar;
				aver_size_char = (aver_size_char * (nbchar - 1) + x - x_top) / nbchar;
            	list = realloc(list, nbchar * sizeof(struct coord));
            	list[nbchar - 1].x = x_top;
            	list[nbchar - 1].y = x;
				blank_count = 0;
            	b = 0;
        	}
        	w = 1;
    	}
 	}
    t.coord = list;
    t.length = nbchar;
    return t;
}


void stock_char(struct text *text, struct line *line, struct tuple nb_line, int char_size) {
	for (int j = 0; j < nb_line.length; ++j) {
		struct tuple char_in_line = char_cut(line->mat[j]);
		struct line *l = build_line(char_in_line.length);
		for (int i = 0; i < char_in_line.length; ++i) {
			struct matrix *m = 	build_matrix(char_in_line.coord[i].y - 	
						char_in_line.coord[i].x + 1,
						nb_line.coord[j].y - 
						nb_line.coord[j].x + 1);
			copy(line->mat[j], m, char_in_line.coord[i].y ,char_in_line.coord[i].x, 0);
			struct tTuple t = block_cut(m);
			struct matrix *block = build_matrix(t.x_l - t.x_u + 1, 
							t.y_l - t.y_u + 1);
			copy(m, block, t.x_l - 1, t.x_u, t.y_u);
			l->mat[i] = resize_char(block, char_size);
			print_dynmat(l->mat[i]);
			printf("\n");
		
		}
		text->line[j] = l;
	}
	printf("DONE\n");
}

void free_lines(struct line *l){
	for (size_t i = 0; i < l->size; ++i)
		free_matrix(l->mat[i]);
	free(l);	
}

void free_text(struct text *text){
	for (size_t i = 0; i < text->size; ++i) {
		free_lines(text->line[i]);
	}
	free(text);
}

struct text *cut(SDL_Surface *img) {

	/*Generating the matrix*/
	display_image(img);
  	struct matrix *mat_img = build_matrix(img->w, img->h);
  	*img = to_black_white(img);
  	img2mat(img, mat_img);
	//print_dynmat(mat_img, img->w, img->h);
	printf("\n");
	display_image(img);
	rlsa(img, mat_img);
	/*First cutting*/
	struct tTuple t = block_cut(mat_img);
	struct matrix *block = build_matrix(t.x_l - t.x_u + 1, t.y_l - t.y_u + 1);
	copy(mat_img, block, t.x_l, t.x_u, t.y_u);
	print_dynmat(block);
//	printf("\n");

	free_matrix(mat_img);
	/*Line cutting*/
	struct tuple nb_lines = line_cut(block);

	struct line *lines = build_line(nb_lines.length);
	stock_lines(lines, block, nb_lines);

	for (size_t i = 0; i < lines->size; ++i) {
		print_dynmat(lines->mat[i]);
		printf("\n");
	}

	free_matrix(block);
	/*Characters cutting*/
	struct text *text = build_text(nb_lines.length);
	stock_char(text, lines, nb_lines, 15);
	free_lines(lines);
	free(nb_lines.coord);
	return text;
}


int main() {
  	char *path = malloc(256);
  	printf("Please enter the path to your image: ");
  	if(scanf("%256s", path) != 1)
        return -1; 
	init_sdl();

	SDL_Surface *img = load_image(path);
	struct text *cutted = cut(img);
	SDL_FreeSurface(img);
	free(path);
	free_text(cutted);
	printf("FINISHED\n");
	return 0;

}
