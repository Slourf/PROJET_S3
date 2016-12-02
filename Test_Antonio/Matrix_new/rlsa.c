# include <stdlib.h>
# include <stdio.h>

# include "pixel_operations.h"
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include "main.h"
//////////////////////////////////////////
/*Struct init*/
//////////////////////////////////////////


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
void rlsa_vec(struct matrix *ori, struct matrix *mat, int c) {
	int w = mat->w;
	int h = mat->h;

	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < h; ++j) {
			if (ori->data[i][j] == 0) {
				int n = 0;
				int x = 0;
				int b = 1;
				while (b && j - x >= 0 && n < c) {
					if (ori->data[i][j - x] == 0)
						++n;
					else
						b = 0;
					++x;
				}
				b = 1;
				x = 0;
				while (b && j + x < h && n < c) {
					if (ori->data[i][j + x] == 0)
						++n;
					else
						b = 0;
					++x;
				}
				if (n == c)
					mat->data[i][j] = 0;
				else
					mat->data[i][j] = 1;
			}
			else
				mat->data[i][j] = 1;
		}
	}
}


void rlsa_hor(struct matrix *ori, struct matrix *mat, int c) {
	int h = mat->h;
	int w = mat->w;
    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            if (ori->data[i][j] == 0) {
                int n = 0;
                int x = 0;
                int b = 1;
                while (b && i - x >= 0 && n < c) {
                    if (ori->data[i - x][j] == 0)
                        ++n;
                    else
                        b = 0;
                    ++x;
                }   
                b = 1;
                x = 0;
                while (b && i + x < w && n < c) {
                    if (ori->data[i + x][j] == 0)
                        ++n;
                    else
                        b = 0;
                    ++x;
                }   
                if (n == c)
                    mat->data[i][j] = 0;
                else
                    mat->data[i][j] = 1;
            }   
            else
                mat->data[i][j] = 1;
        }   
    }   
}

void rlsa_merge(struct matrix *mat_img, struct matrix *mat_vec, struct matrix *mat_hor) {
	size_t h = mat_img->h;
	size_t w = mat_img->w;

	for (size_t y = 0; y < h; ++y) {	
		for (size_t x = 0; x < w; ++x) {
			if (mat_hor->data[x][y] && mat_vec->data[x][y])
				mat_img->data[x][y] = 1;
			else
				mat_img->data[x][y] = 0;
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
	struct matrix *m = build_matrix(img->w, coord.coord[i].y - coord.coord[i].x + 1);
        copy(img, m, img->w - 1, 0, coord.coord[i].x);
	m->w = img->w - 1;
	m->h = coord.coord[i].y - coord.coord[i].x;
        lines->mat[i] = m;
    }
}

struct block* block_rlsa_cut(struct matrix *matrix, struct block* list) {
	struct block *block_list = malloc(sizeof (struct block));
	struct tuple lines = line_cut(matrix); //FIXEME HERE
	struct lines *stored_lines = init_lines(lines.length);
	stock_lines_rlsa(stored_lines, matrix, lines);
	for (size_t i = 0; i < stored_lines->size; ++i) {
		struct matrix *curr = *(stored_lines->mat + i);
		struct tuple columns = char_cut(curr);
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
void display(struct matrix *mat) {
	size_t h = mat->h;
	size_t w = mat->w;
	
	SDL_Surface *img = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);	
	for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {
		Uint32 pixel = getpixel(img, x, y);
		if (mat->data[x][y] == 1)
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
	struct matrix *mat_img = build_matrix(img->w, img->h);
	struct matrix *mat_vec = build_matrix(img->w, img->h);
	struct matrix *mat_hor = build_matrix(img->w, img->h);

	img2mat(img, mat_img);

	/*Horizontal treatment*/
	rlsa_hor(mat_img, mat_hor, img->w / 10);
	display(mat_hor);

	/*Vertical treatment*/
	rlsa_vec(mat_img, mat_vec, img->h/5);
	display(mat_vec);

	/*Merge of the two matrix + Polish with Horizontal treatment*/
	rlsa_merge(mat_img, mat_vec, mat_hor);
	rlsa_hor(mat_img, mat_img, 4);
	display(mat_img);
	
	struct matrix *m = build_matrix(img->w, img->h);
	m = mat_img;
	printf("passseg\n");

	block_rlsa_cut(m, NULL);
	printf("passrlsa\n");
	free_matrix(mat_vec);
	free_matrix(mat_hor);	
	return m;
}

