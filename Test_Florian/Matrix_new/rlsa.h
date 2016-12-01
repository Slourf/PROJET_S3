//rlsa.h

# ifndef RLSA_H_
# define RLSA_H_

# include <err.h>
# include <stdio.h>
# include <stdlib.h>

# include "pixel_operations.h"
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>

# include "main.h"
/////////////////////////////////////////                                       
/*Struct declaration*/                                                          
/////////////////////////////////////////                                       
                                                                                
struct block {                                                                  
        int black_pixel;                                                        
        int ori_black_pixel;                                                    
        size_t Xmin, Ymin, delX, delY;                                          
        int wb_trans;                                                           
        struct block *next;                                                     
};                                                                              
                                                                                
struct matrix {                                                                 
        long **mat;                                                             
        size_t h;                                                               
        size_t w;                                                               
};                                                                              
                                                                                
                                                                                
struct lines {                                                                  
        struct matrix **mat;                                                    
        int *Xori, *Yori;                                                       
        size_t size;                                                            
};                                                                              
struct matrix* init_matrix(size_t w, size_t h);
struct lines* init_lines(size_t size);  
void rlsa_vec(long **ori, long **mat, int h, int w, int c);
void rlsa_hor(long **ori, long **mat, int h, int w, int c);
void rlsa_merge(long **mat_img, long **mat_vec, long **mat_hor, size_t h, size_t w);
struct block* append_block(struct block *block, struct block *new);
void stock_lines_rlsa(struct lines *lines, struct matrix *img, struct tuple coord) ;
struct block* block_rlsa_cut(struct matrix *matrix, struct block* list);
void display(long **mat, size_t h, size_t w);
    
struct matrix* rlsa(SDL_Surface *img, int c);

# endif
