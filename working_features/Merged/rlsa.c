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

void supress_image(struct matrix *img, int x_l, int x_r, int y_t, int y_b) {
	for (int i = x_l; i <= x_r; ++i) {
		for (int j = y_t; j <= y_b; ++j) {
			img->data[i][j] = 0;
		}
	}
}


struct lines* stock_lines_rlsa(struct matrix *img, struct tuple coord) {
	struct lines *lines = init_lines(coord.length);
	for (int i = 0; i < coord.length; ++i) {
		struct matrix *m = build_matrix(img->w,
		coord.coord[i].y - coord.coord[i].x + 1);
		copy(img, m, img->w - 1, 0, coord.coord[i].x);
		m->w = img->w - 1;
		m->h = coord.coord[i].y - coord.coord[i].x;
		lines->mat[i] = m;
	}
	return lines;
}

struct lines* stock_columns_rlsa(struct matrix *img, struct tuple coord) {
	struct lines *columns = init_lines(coord.length);
	for (int i = 0; i < coord.length; ++i) {
		struct matrix *m = build_matrix(coord.coord[i].y - coord.coord[i].x + 1,
		img->h);
		copy(img, m, coord.coord[i].y, coord.coord[i].x, 0);
		columns->mat[i] = m;
	}
	return columns;
}


struct block* block_rlsa_cut(struct matrix *matrix, struct block* list,
int x, int y) {
	struct block *block_list = malloc(sizeof (struct block));
	struct tuple lines = line_cut(matrix);
	struct lines *stored_lines = stock_lines_rlsa(matrix, lines);
	for (size_t i = 0; i < stored_lines->size; ++i) {
		struct matrix *curr = *(stored_lines->mat + i);
		struct tuple columns = char_cut(curr);
		struct lines *stored_columns = stock_columns_rlsa(curr, columns);
		struct coord c_line = *(lines.coord + i);
		if ((lines.length == 1 || lines.length == 2) && columns.length == 1) {
			block_list->Xori = x;
			block_list->Yori = y;
			block_list->w = matrix->w;
			block_list->h = matrix->h;
			list = append_block(list, block_list);
			return list;
		}
		else {
			for (size_t j = 0; j < stored_columns->size; ++j) {
				struct coord c_column = *(columns.coord + j);
				struct matrix *curr1 = *(stored_columns->mat + j);
				list = block_rlsa_cut(curr1, list, x + c_column.x, y + c_line.x);
			}
		}
	}
	return list;
}
////////////////////////////////////////////
//Delete image
////////////////////////////////////////////

size_t average_heigth(struct block *list, size_t min) {
	size_t aver = 0;
	int i = 0;

	for (; list; list = list->next) {
		if (list->h > min) {
			++i;
			aver = (aver * i + list->h) / (i + 1);
		}
	}
	return aver;
}

void remove_image(struct matrix *img, struct block *b) {
	for (size_t j = b->Xori; j < b->Xori + b->w; ++j) {
		for (size_t i = b->Yori; i < b->Yori + b->h; ++i) {
			img->data[j][i] = 0;
		}
	}
}

void suppress(struct block *list, struct matrix *img, size_t aver) {
	for (; list; list = list->next) {
		if (list->h  > aver * 15) {
			remove_image(img, list);
		}
	}
}

void free_list(struct block *list) {
	if (!list)
		return;
	while (list->next) {
		struct block *temp = list;
		list = list->next;
		free(temp);
	}
	free(list);
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

void rlsa(SDL_Surface *img, struct matrix *mat) {

	/*Generating the matrix*/
	*img = to_black_white(img);
	struct matrix *mat_img = build_matrix(img->w, img->h);
	struct matrix *mat_vec = build_matrix(img->w, img->h);
	struct matrix *mat_hor = build_matrix(img->w, img->h);

	img2mat(img, mat_img);

	/*Horizontal treatment*/
	rlsa_hor(mat_img, mat_hor, img->w / 10);

	/*Vertical treatment*/
	rlsa_vec(mat_img, mat_vec, img->h/5);

	/*Merge of the two matrix + Polish with Horizontal treatment*/
	rlsa_merge(mat_img, mat_vec, mat_hor);
	rlsa_hor(mat_img, mat_img, 4);

	struct matrix *m = build_matrix(img->w, img->h);
	m = mat_img;
	struct block *list = block_rlsa_cut(m, NULL, 0, 0);
	size_t averH = average_heigth(list, m->h / 155);
	suppress(list, mat,  averH);

	free_list(list);
	free_matrix(mat_vec);
	free_matrix(mat_hor);
}

