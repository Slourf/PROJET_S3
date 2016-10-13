# define _XOPEN_SOURCE 500

# include <err.h>
# include <stdio.h>
# include <stdlib.h>

# include"pixel_operations.h"
# include<SDL/SDL.h>
# include<SDL/SDL_image.h>


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

SDL_Surface* BlackandWhite(SDL_Surface *imggrey){
  Uint8 mid = 127;
  Uint8 blanc = 255;
  Uint8 noir = 0;
  for (int x = 0; x < imggrey->w; ++x) {
        for (int y = 0; y < imggrey->h; ++y) {
                Uint8 r, g, b;
                Uint32 pixel = getpixel(imggrey, x, y);
                SDL_GetRGB(pixel, imggrey->format, &r, &g, &b);
		if(r>mid){
			 pixel = SDL_MapRGB(imggrey->format, blanc, blanc,blanc);

		}
		else{
			pixel = SDL_MapRGB(imggrey->format, noir,noir, noir);
		}
                putpixel(imggrey, x, y, pixel); 
        }       
    }
  return imggrey;
}

int main() {
    init_sdl();
    char *path = malloc(256);
    printf("Veuillez entrer le chemin de votre image : ");
    scanf("%256s", path);
    SDL_Surface *img = load_image(path);
    free(path);
    display_image(img);	
    for (int x = 0; x < img->w; ++x) {
	for (int y = 0; y < img->h; ++y) {
		Uint8 r, g, b;
		Uint32 pixel = getpixel(img, x, y);
		SDL_GetRGB(pixel, img->format, &r, &g, &b);
		r *= 0.3f;
		g *= 0.59f;
		b *= 0.11f;
		float lumi = (r + g + b);
		pixel = SDL_MapRGB(img->format, lumi, lumi, lumi);
		putpixel(img, x, y, pixel);
	}
    }
    display_image(img);
    img= BlackandWhite(img);
    display_image(img);
    free(img);
    return 0;
}
