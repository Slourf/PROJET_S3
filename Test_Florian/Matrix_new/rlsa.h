//rlsa.h

# ifndef RLSA_H_
# define RLSA_H_

# include <err.h>
# include <stdio.h>
# include <stdlib.h>

# include "pixel_operations.h"
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>

# include "resize.h"


struct matrix* rlsa(SDL_Surface *img, int c);

# endif
