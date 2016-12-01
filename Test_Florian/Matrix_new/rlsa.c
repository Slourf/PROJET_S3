# include <stdlib.h>
# include <stdio.h>

# include "pixel_operations.h"
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include "rlsa.h"
# include "resize.h"
# include "main.h"
//////////////////////////////////////////
/*Struct init*/
//////////////////////////////////////////

struct matrix* init_matrix(size_t w, size_t h) {
	struct matrix *m = malloc(sizeof (struct matrix));
	m->mat = build_matrix(w, h);
	m->w = w;
	m->h = h;
	return m;
}

struct lines* init_lines(size_t size) {
	struct lines *lines = malloc(sizeof (struct lines));
	lines->mat = malloc(size * sizeof (struct matrix*));
	lines->Xori = calloc(size, sizeof (int));
	lines->Yori = calloc(size, sizeof (int));
	lines->size = size;
	return lines;
}


//////////////////////////////////////////
/*image treatement for rlsa*/
//////////////////////////////////////////
void rlsa_vec(long **ori, long **mat, int h, int w, int c) {
	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < h; ++j) {
			if (ori[i][j] == 0) {
				int n = 0;
				int x = 0;
				int b = 1;
				while (b && j - x >= 0 && n < c) {
					if (ori[i][j - x] == 0)
						++n;
					else
						b = 0;
					++x;
				}
				b = 1;
				x = 0;
				while (b && j + x < h && n < c) {
					if (ori[i][j + x] == 0)
						++n;
					else
						b = 0;
					++x;
				}
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
                while (b && i - x >= 0 && n < c) {
                    if (ori[i - x][j] == 0)
                        ++n;
                    else
                        b = 0;
                    ++x;
                }   
                b = 1;
                x = 0;
                while (b && i + x < w && n < c) {
                    if (ori[i + x][j] == 0)
                        ++n;
                    else
                        b = 0;
                    ++x;
                }   
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

/////////////////////////////////////////
/*stocking of needed data*/
/////////////////////////////////////////


struct block* append_block(struct block *block, struct block *new) {
	if (block == NULL)
		return new;
	else {
		new->next = block;
		return new;
	}
}


void stock_lines_rlsa(struct lines *lines, struct matrix *img, struct tuple coord) {
    for (int i = 0; i < coord.length; ++i) {
	struct matrix *m = init_matrix(img->w, coord.coord[i].y - coord.coord[i].x + 1);
        copy(img->mat, m->mat, img->w - 1, 0, coord.coord[i].y, coord.coord[i].x);
	m->w = img->w - 1;
	m->h = coord.coord[i].y - coord.coord[i].x;
        lines->mat[i] = m;
    }
}

struct block* block_rlsa_cut(struct matrix *matrix, struct block* list) {
	struct block *block_list = malloc(sizeof (struct block));
	printf("pass4.1\n");
	printf("w = %zu && h = %zu\n", matrix->w, matrix->h);
	struct tuple lines = line_cut(matrix->mat, matrix->w, matrix->h); //FIXEME HERE
	printf("pass4.5\n");
	struct lines *stored_lines = init_lines(lines.length);
	stock_lines_rlsa(stored_lines, matrix, lines);
	for (size_t i = 0; i < stored_lines->size; ++i) {
		struct matrix *curr = *(stored_lines->mat + i);
		struct tuple columns = char_cut(curr->mat, curr->w, curr->h);
		struct lines *stored_columns = init_lines(columns.length);
		stock_lines_rlsa(stored_columns, matrix, columns);
		printf("i = %zu\n", i);	
		struct coord c_line = *(lines.coord + i);

		(stored_lines + i)->Xori += c_line.x;
		(stored_lines + i)->Yori += c_line.y;

		if (lines.length == 1 && columns.length == 1) {
			//ajouter dans ma block_list
			//en complétant tout les champs
			list = append_block(list, block_list);
			return list;
		}
		else {
			for (size_t j = 0; j < stored_columns->size; ++j) {
				printf("j = %zu\n", j);
				printf("size_colum = %zu && %d\n", stored_columns->size, columns.length);
				struct coord c_column = *(columns.coord + j);
				(stored_columns + i)->Xori += c_column.x;
				(stored_columns + i)->Yori += c_column.y;
				struct matrix *curr = *(stored_columns->mat + j);
				block_rlsa_cut(curr, list);
			}
			return list;
			printf("pass\n");
		}
	}
	return list; 
}



////////////////////////////////////////////
/*Display*/
////////////////////////////////////////////
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
	SDL_FreeSurface(img);
}
//////////////////////////////////////////
/*Final function which will be called*/
////////////////////////////////////////

struct matrix *rlsa(SDL_Surface *img, int c) {
	if (c)
		printf("ok\n");
	/*Generating the matrix*/
	*img = to_black_white(img);
	long **mat_img = build_matrix(img->w, img->h);
	long **mat_vec = build_matrix(img->w, img->h);
	long **mat_hor = build_matrix(img->w, img->h);

	img2mat(img, mat_img);

	/*Horizontal treatment*/
	rlsa_hor(mat_img, mat_hor, img->h, img->w, img->w / 10);
	display(mat_hor, img->h, img->w);

	/*Vertical treatment*/
	rlsa_vec(mat_img, mat_vec, img->h, img->w, img->h/5 );
	display(mat_vec, img->h, img->w);

	/*Merge of the two matrix + Polish with Horizontal treatment*/
	rlsa_merge(mat_img, mat_vec, mat_hor, img->h, img->w);
	rlsa_hor(mat_img, mat_img, img->h, img->w, 4);
	display(mat_img, img->h, img->w);
	
	struct matrix *m = init_matrix(img->w, img->h);
	m->mat = mat_img;
	printf("passseg\n");

	block_rlsa_cut(m, NULL);
	printf("passrlsa\n");
	free_matrix(mat_vec, img->h);
	free_matrix(mat_hor, img->h);	
	return m;
}

