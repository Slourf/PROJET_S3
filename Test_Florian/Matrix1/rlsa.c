# include <stdlib.h>
# include <stdio.h>

# include "pixel_operations.h"
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>

# include "resize.h"
# include "main.h"


void rlsa_vec(long **ori, long **mat, int h, int w, int c) {
	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < h; ++j) {
			if (ori[i][j] == 0) {
				int n = 0;
				int x = 0;
				int b = 1;
		//		printf("loop\n");
				while (b && j - x >= 0 && n < c) {
					if (ori[i][j - x] == 0)
						++n;
					else
						b = 0;
					++x;
				}
		//		printf("while1\n");
				b = 1;
				x = 0;
				while (b && j + x < h && n < c) {
					if (ori[i][j + x] == 0)
						++n;
					else
						b = 0;
					++x;
				}
		//		printf("while2\n");
				if (n == c)
					mat[i][j] = 0;
				else
					mat[i][j] = 1;
			}
			else
				mat[i][j] = 1;
		}
	}
}


void rlsa_hor(long **ori, long **mat, int h, int w, int c) {
    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            if (ori[i][j] == 0) {
                int n = 0;
                int x = 0;
                int b = 1;
        //      printf("loop\n");
                while (b && i - x >= 0 && n < c) {
                    if (ori[i - x][j] == 0)
                        ++n;
                    else
                        b = 0;
                    ++x;
                }   
        //      printf("while1\n");
                b = 1;
                x = 0;
                while (b && i + x < w && n < c) {
                    if (ori[i + x][j] == 0)
                        ++n;
                    else
                        b = 0;
                    ++x;
                }   
        //      printf("while2\n");
                if (n == c)
                    mat[i][j] = 0;
                else
                    mat[i][j] = 1;
            }   
            else
                mat[i][j] = 1;
        }   
    }   
}

void rlsa_merge(long **mat_img, long **mat_vec, long **mat_hor, size_t h, size_t w) {
	for (size_t y = 0; y < h; ++y) {	
		for (size_t x = 0; x < w; ++x) {
			if (mat_hor[x][y] && mat_vec[x][y])
				mat_img[x][y] = 1;
			else
				mat_img[x][y] = 0;
		}
	}
}



void display(long **mat, size_t h, size_t w) {
	
	SDL_Surface *img = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);	
	for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {

			Uint32 pixel = getpixel(img, x, y);
            if (mat[x][y] == 1)
            	pixel = SDL_MapRGB(img->format, 0, 0, 0);
			else
                pixel = SDL_MapRGB(img->format, 255, 255, 255);
			putpixel(img, x, y, pixel);
        }
    }   
	display_image(img);
}


long** rlsa(SDL_Surface *img, int c) {
	if (c)
		printf("ok\n");
    /*Generating the matrix*/
	*img = to_black_white(img);
    long **mat_img = build_matrix(img->w, img->h);
    long **mat_vec = build_matrix(img->w, img->h);
    long **mat_hor = build_matrix(img->w, img->h);
    
	img2mat(img, mat_img);
   	
	rlsa_hor(mat_img, mat_hor, img->h, img->w, img->w / 30);
	display(mat_hor, img->h, img->w);

	rlsa_vec(mat_img, mat_vec, img->h, img->w, img->h / 10);
	display(mat_vec, img->h, img->w);
	
	rlsa_merge(mat_img, mat_vec, mat_hor, img->h, img->w);	
	display(mat_img, img->h, img->w);

	return mat_img;
}

