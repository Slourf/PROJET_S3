# include <stdlib.h>
# include <stdio.h>

# include "pixel_operations.h"
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>

# include "resize.h"
# include "main.h"


void rlsa_vec(long **ori, long **mat, int h, int w, int c) {
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			if (ori[j][i] == 0) {
				int n = 0;
				int x = 0;
				int b = 1;
				while (b && j - x >= 0 && n < c) {
					if (ori[j - x][i] == 0)
						++n;
					else
						b = 0;
					++i;
				}
				b = 1;
				i = 0;
				while (b && j + x < h && n < c) {
					if (ori[j + x][i] == 0)
						++n;
					else
						b = 0;
				++i;
				}
				if (n == c)
					mat[j][i] = 0;
				else
					mat[j][i] = 1;
			}
			mat[j][i] = 1;
		}
	}
}


long** rlsa(char *path, int c) {
    init_sdl();

    /*Generating the matrix*/
    SDL_Surface *img = load_image(path);
    free(path);
    *img = to_black_white(img);
    long **mat_img = build_matrix(img->w, img->h);
    long **mat_vec = build_matrix(img->w, img->h);
    //long **mat_hor = build_matrix(img->w, img->h);
    img2mat(img, mat_img);
    rlsa_vec(mat_img, mat_vec, img->h, img->w, c); 
    print_dynmat(mat_vec, img->w, img->h);
	return mat_img;
}

