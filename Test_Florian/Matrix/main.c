# include <stdlib.h>
# include <err.h>
# include <stdio.h>

# include "cut.h"
# include "rlsa.h"

# include"pixel_operations.h"
# include<SDL/SDL.h>
# include<SDL/SDL_image.h>



int main() {
    char *path = malloc(256);
    printf("Please enter the path to your image: ");
    if(scanf("%256s", path) != 1)
        return -1;
    long ****cutted = cut(path);
	rlsa(path, 4);
    free(path);
    free(cutted);
    return 0;
}

