# define _XOPEN_SOURCE 500

# include <err.h>
# include <stdio.h>
# include <stdlib.h>

# include"pixel_operations.h"
# include<SDL/SDL.h>
# include<SDL/SDL_image.h>

# include "resize.h"
# include "rlsa.h"
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

long** build_matrix(size_t w, size_t h) {
    long **mat;
    mat = malloc(w * sizeof (long *));
    for (size_t i = 0; i < w; ++i) {
        mat[i] = malloc(h * sizeof (long));
    }
    return mat;
}	

void free_matrix(long **mat, int w) {
	for (int i = 0; i < w; ++i)
		free(*(mat + i));
	free(mat);
}


void img2mat(SDL_Surface *img, long **mat) {
    for (int x = 0; x < img->w; ++x) {
        for (int y = 0; y < img->h; ++y) {
            Uint8 r, g, b;
            Uint32 pixel = getpixel(img, x, y);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
			if (r < 128)
				mat[x][y] = 1;
			else
            	mat[x][y] = 0;
        }
    }
}

void print_dynmat(long **mat, size_t x, size_t y) {
    for (size_t j = 0; j < y; ++j) {
        for (size_t i = 0; i < x; ++i)
            printf("%ld", mat[i][j]);
        printf("\n");
    }
}


int get_upper_y(long **mat, int width, int height) {
	int b = 1;
	int y = 0;
	int lineY = 0;

	while (b == 1 && y < height) {
		int x = 0;
							     
		while (b == 1 && x < width) {
			if(mat[x][y] == 1)
				b = 0;
			++x;
		}
		if (b == 1)
			++lineY;
		++y;
	}
	return lineY;
}   

int get_lower_y(long **mat, int width, int height) {
	int b = 1;
	int y = height - 1;
	int lineY = height - 1;

	while (b == 1 && y >= 0) {
		int x = 0;
		
		while (b == 1 && x < width) {
			if (mat[x][y] == 1)
				b = 0;
			++x;
		}
		if (b == 1)
			--lineY;
		--y;
	}
	return lineY;
}

int get_upper_x(long **mat, int width, int height) {
    int b = 1;
    int x = 0;
    int columnX = 0;

    while (b == 1 && x < width) {
        int y = 0;
    
        while (b == 1 && y < height) {
            if(mat[x][y] == 1)
                b = 0;
            ++y;
        }
        if (b == 1)
            ++columnX;
        ++x;
    }   
    return columnX;
}   

int get_lower_x(long **mat, int width, int height) {
    int b = 1;
    int x = width - 1;
    int columnX = width - 1;

    while (b == 1 && x >= 0) {
        int y = 0;
    
        while (b == 1 && y < height) {
            if (mat[x][y] == 1)
                b = 0;
            ++y;
        }
        if (b == 1)
           --columnX;
        --x;
    }   
    return columnX;
}

void copy(long **old_mat, long **new_mat, int x_l, int x_u, int y_l, int y_u) {

	for (int i = 0; i <= (x_l - x_u); ++i) {
		for(int j = 0 ; j <= (y_l - y_u); ++j) {
			new_mat[i][j] = old_mat[x_u + i][y_u + j];
		}
	}
	
}	
		
struct tTuple block_cut (long **mat, int width, int height) {
	int y_l = get_lower_y(mat, width, height);
	int y_u = get_upper_y(mat, width, height);
	int x_l = get_lower_x(mat, width, height);
	int x_u = get_upper_x(mat, width, height);
	
	struct tTuple t;
	if (y_l <= y_u) {
		t.x_l = width;
		t.x_u = 0;
		t.y_l = height;
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





struct tuple line_cut(long **mat, int width, int height) {
	int y_top = 0;
	int w = 0;
	int b = 1;
	int c = 0;
	int nbLine = 0;

	struct coord *list = NULL;
	struct tuple t;

	for (int y = 0; y < height; ++y) {
		int x = 0;
		c = 0;
		while (x < width && c == 0) {
			if (mat[x][y] == 1) {
				c = 1;
			}
			++x;
		}
		if(c == 1) {
			if (w == 1) {
				y_top = y;
				w = 0;
			}
			if (y + 1 == height){
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
				
void stock_lines(long ***lines, long** img, int width, struct tuple coord) {
	for (int i = 0; i < coord.length; ++i) {
		long **m = build_matrix(width, coord.coord[i].y - coord.coord[i].x + 1);
		copy(img, m, width - 1, 0, coord.coord[i].y, coord.coord[i].x);
		lines[i] = m;
	}
}


struct tuple char_cut(long **mat, int width, int height)  
{       
    int x_top = 0;
    int w = 0;
    int b = 0;
    int c = 0;
    int nbchar = 0;
    int blank_count = 0;
    int aver_size_char = 0;
    struct coord *list = NULL;
    struct tuple t;
    for (int x = 0; x < width; ++x) {
	int y = 0;
        c = 0;
        while (y < height && c == 0) {
	    if (mat[x][y] == 1) {
		c = 1;
            }
            ++y;
    	}
    	if(c == 1) {
	    if (w == 1) {
		if (blank_count > aver_size_char) {
		   ++nbchar;
		   list = realloc(list, nbchar * sizeof(struct coord));
		   list[nbchar - 1].x = x_top+blank_count-1;
		   list[nbchar - 1].y = x-1;
		   blank_count = 0;
		}
		x_top = x;
            	w = 0;
            }
	    if (x + 1 == width){
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


void stock_char(long ****chat, long ***lines, struct tuple nb_line, int width, int char_size) {
	for (int j = 0; j < nb_line.length; ++j) {
		struct tuple char_in_line = char_cut(lines[j], width, 
				nb_line.coord[j].y - nb_line.coord[j].x + 1);
		long ***line_char = calloc(char_in_line.length, sizeof(long **));
		for (int i = 0; i < char_in_line.length; ++i) {
			long **m = 	build_matrix(char_in_line.coord[i].y - 	
						char_in_line.coord[i].x + 2,
						nb_line.coord[j].y - 
						nb_line.coord[j].x + 2);

			int y_l = nb_line.coord[j].y - nb_line.coord[j].x;
			copy(lines[j], m, char_in_line.coord[i].y,
					char_in_line.coord[i].x,y_l, 0);
			struct tTuple t = block_cut(m, 
			char_in_line.coord[i].y - char_in_line.coord[i].x, y_l);
			long **block = build_matrix(t.x_l - t.x_u + 1, 
							t.y_l - t.y_u + 1);
			copy(m, block, t.x_l, t.x_u, t.y_l, t.y_u);
			line_char[i] = resize_char(block, t.x_l - t.x_u + 1, t.y_l - t.y_u + 1, char_size);;
			chat[j] = line_char;
			print_dynmat(line_char[i], char_size, char_size);
			printf("\n");
		}
	}
}


long**** cut(SDL_Surface *img) {

	/*Generating the matrix*/
	display_image(img);
  	long **mat_img = build_matrix(img->w, img->h);
  	*img = to_black_white(img);
  	img2mat(img, mat_img);
	//print_dynmat(mat_img, img->w, img->h);
	printf("\n");
	display_image(img);

	/*First cutting*/
	struct tTuple t = block_cut(mat_img, (int)img->w, (int)img->h);
	long **block = build_matrix(t.x_l - t.x_u + 1, t.y_l - t.y_u + 1);
	copy(mat_img, block, t.x_l, t.x_u, t.y_l, t.y_u);

//	print_dynmat(block, t.x_l - t.x_u + 1, t.y_l - t.y_u + 1);
//	printf("\n");

	free_matrix(mat_img, img->w);

	/*Line cutting*/
	int width = t.x_l - t.x_u + 1;
	int height = t.y_l - t.y_u + 1;
	struct tuple nb_lines = line_cut(block, width, height);

	long ***lines = calloc(nb_lines.length, sizeof(long **));
	stock_lines(lines, block, width, nb_lines);
	
/*
	for (int i = 0; i < nb_lines.length; ++i) {
		print_dynmat(lines[i],width,nb_lines.coord[i].y-nb_lines.coord[i].x);
		printf("\n");
	}
	*/
	free_matrix(block, t.x_l - t.x_u);
	/*Characters cutting*/
	long ****chat = calloc(nb_lines.length, sizeof(long ***));
	stock_char(chat, lines, nb_lines, width, 15);	
	free(lines);
	return chat;
}


int main() {
  	char *path = malloc(256);
  	printf("Please enter the path to your image: ");
  	if(scanf("%256s", path) != 1)
        return -1; 
	init_sdl();

	SDL_Surface *img = load_image(path);
	long ****cutted = cut(img);
	struct matrix *mat_rlsa = rlsa(img, 10);
	free_matrix(mat_rlsa->mat,img->w);
	free(mat_rlsa);	
SDL_FreeSurface(img);
	free(path);
	free(cutted);
	return 0;

}
