#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "utility_SDL.h"
#include "PreTreatment.h"
#include <err.h>
#include "Segmentation.h"

void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

int main(int argc, char *argv[])
{
  if (argc < 2)
      errx(2, "Missing argument");
      
  int quit = 0;
  SDL_Event event;
  init_sdl();
 
  SDL_Window * window = SDL_CreateWindow("SDL2 Displaying Image",
					 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1080, 860, 0);
  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_Surface * image = load_image(argv[1]);
  int h = image -> h;
  int w = image -> w;
  Uint8 binaryArray[h][w];
  if(argc == 2 || strcmp(argv[2],"s") == 0 || strcmp(argv[2],"segmentation") == 0)
    {
      AllTreatment(h, w, binaryArray,image, "");
      InitFile(h,w,binaryArray, image); //Appelle la segmentation
    }
  else
    {
      if (strcmp(argv[2],"b") == 0|| strcmp(argv[2],"binarize") == 0||
	  strcmp(argv[2],"g") == 0|| strcmp(argv[2],"grayscale") == 0)
	AllTreatment(h, w, binaryArray,image, argv[2]);
      else
	errx(3, "Argument : %s doesn't exist", argv[2]);
    }
  display_Array(h,w,binaryArray, image);
  
  
  
  SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);

  while (!quit)
    {
      SDL_WaitEvent(&event);

      switch (event.type)
	{
        case SDL_QUIT:
	  quit = 1;
	  break;
        }
    }

  SDL_DestroyTexture(texture);
  SDL_FreeSurface(image);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
