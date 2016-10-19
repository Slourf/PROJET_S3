# define _XOPEN_SOURCE 500

# include <err.h>
# include <stdio.h>
# include <stdlib.h>

# include"pixel_operations.h"
# include<SDL/SDL.h>
# include<SDL/SDL_image.h>


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
            if (lumi < 256 / 2)
                lumi = 0;
            else
                lumi = 255;
            pixel = SDL_MapRGB(img->format, lumi, lumi, lumi);
            putpixel(img, x, y, pixel);
        }
    }
    return *img;
}

long** build_matrix(size_t x, size_t y) {
    long **mat;
    mat = malloc(x * sizeof (long *));
    for (size_t i = 0; i < x; ++i) {
        mat[i] = malloc(y * sizeof (long));
    }
    return mat;
}	


void build_img_matrix(SDL_Surface *img, long **mat) {
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

void print_dynmat_line(long **mat, size_t x, size_t y_top, size_t y) {
	for (size_t j = y_top; j < y; ++j) {
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
	//		printf("x = %d; y = %d;\n", i, j);
			new_mat[i][j] = old_mat[x_u + i][y_u + j];
		}
	}
	printf("copy");
}	
		
tTuple block_cut (long **mat, int width, int height) {
	int y_l = get_lower_y(mat, width, height);
	int y_u = get_upper_y(mat, width, height);
	int x_l = get_lower_x(mat, width, height);
	int x_u = get_upper_x(mat, width, height);
	
	tTuple t;
	t.x_l = x_l;
	t.x_u = x_u;
	t.y_l = y_l;
	t.y_u = y_u;
	return t;
}





tuple line_cut(long **mat,/* long ***lines,*/ int width, int height) {
	int y_top = 0;
	int w = 0;
	int b = 1;
	int c = 0;
	int nbLine = 0;

	coord *list = NULL;
	tuple t;

	for (int y = 0; y < height; ++y) {
		int x = 0;
		printf("%d\n", y);
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
			if (y + 1 == height) {
				printf("biatch\n");
                ++nbLine;

				list = realloc(list, nbLine * sizeof(coord));
				list[nbLine - 1].x = y_top;
				list[nbLine - 1].y = y;
/*
                long ***lines_realloc = realloc(lines, nbLine * sizeof(long **));
				if (lines_realloc != NULL)
					lines = lines_realloc;
				free(lines_realloc);
                lines[nbLine - 1] = build_matrix(width, 1 + y - y_top);
				 
                
				copy(mat, lines[nbLine - 1], width - 1, 0, y + 1, y_top); 
                printf("size = %d; y_l = %d; y_top = %d;\n", y - y_top, y, y_top);
                print_dynmat(lines[nbLine - 1], width, 1 + y - y_top);*/
			}
			b = 1;
		}
		else {
			if (b == 1) {
				printf("biatch\n");
				++nbLine;
				
				list = realloc(list, nbLine * sizeof(coord));
				list[nbLine - 1].x = y_top;
				list[nbLine - 1].y = y;

			/*	long ***lines_realloc = NULL;
				lines_realloc = realloc(lines, nbLine * sizeof(long **));
				if (lines_realloc != NULL) {
					lines = lines_realloc;
					printf("realloc");
					printf("az = %ld\n", **lines[nbLine - 1]);
				}
			//	print_dynmat(lines[nbLine - 1], width, y - y_top);
				
				lines[nbLine - 1] = build_matrix(width, y - y_top);

				printf("check");
				
				copy(mat, lines[nbLine - 1], width - 1, 0, y, y_top);
				printf("size = %d; y_l = %d; y_top = %d;\n", y - y_top, y, y_top);
				print_dynmat(lines[nbLine - 1], width, y - y_top);*/
				b = 0;
			}
			w = 1;
		}
	}
	t.coord = list;
	t.length = nbLine;
	return t;
}
				











int char_cut(long **mat, int width, int height);


/*
int* cut(long **mat, int ligne) {
	long **new_mat = build_matrix(mat->w, mat->h - ligne);
    int b = 1;
    for (int i = 0; i < new_mat->w; ++i) {
        int j = 0;
        while (j < mat->w

}*/

int main() {
	init_sdl();
    char *path = malloc(256);
    printf("Veuillez entrer le chemin de votre image : ");
    scanf("%256s", path);
	
	/*Génération de la matrice*/
	SDL_Surface *img = load_image(path);
    free(path);
	display_image(img);
    long **mat_img = build_matrix(img->w, img->h);
    *img = to_black_white(img);
    build_img_matrix(img, mat_img);
    print_dynmat(mat_img, img->w, img->h);
	
	/*Premier découpage*/
	tTuple t = block_cut(mat_img, (int)img->w, (int)img->h);
	printf("x_l = %d; x_u = %d; y_l = %d; y_u = %d;\n", t.x_l, t.x_u, t.y_l, t.y_u);
	long **block = build_matrix(t.x_l - t.x_u + 1, t.y_l - t.y_u + 1);
	copy(mat_img, block, t.x_l, t.x_u, t.y_l, t.y_u);
	print_dynmat(block, t.x_l - t.x_u + 1, t.y_l - t.y_u + 1);
	free(mat_img);
	/*Découpage en ligne*/
//	long ***lines = NULL;
	int width = t.x_l - t.x_u + 1;
	int height = t.y_l - t.y_u + 1;
	tuple nb_lines = line_cut(block,/* lines,*/ width, height);
	printf("length = %d\n", nb_lines.length);
	for (int i = 0; i < nb_lines.length; ++i) {
		print_dynmat_line(block, width, nb_lines.coord[i].x, nb_lines.coord[i].y);
		
		printf("%d\n", i);
	}

	display_image(img);
	free(img);
	return 0;
}
