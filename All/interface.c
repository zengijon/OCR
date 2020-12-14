#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "PreTreatment.h"
#include "utility_SDL.h"
#include "Segmentation.h"

#define true 1
#define false 0


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

int main()
{
	//Define all the variables

	int done = false;
	SDL_Event event;
	init_sdl();
	TTF_Init();
	int w = 0;
	int h = 0;
	SDL_Window * window = SDL_CreateWindow("OCR",
					 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
					 1350, 750, SDL_WINDOW_RESIZABLE);
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Color vert = {127, 255, 0, 0};
	SDL_Rect all = {0, 0, 200, 200};
	char txt[300];
	TTF_Font *police = TTF_OpenFont("Data/arial.ttf", 50);
	SDL_Surface *surface_text = TTF_RenderText_Blended(police, "", vert);
	SDL_Surface *ecran = load_image("Data/rectangle-noir.jpg");
	SDL_Surface *tmp = load_image("Data/rectangle-noir.jpg");
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, ecran);
	SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, surface_text);
	SDL_Texture *gray = SDL_CreateTextureFromSurface(renderer,load_image("Data/grayscale.jpg"));
	SDL_Texture *res = SDL_CreateTextureFromSurface(renderer, load_image("Data/rectangle-blanc.jpg"));
	SDL_Texture *binarize = SDL_CreateTextureFromSurface(renderer, load_image("Data/binarize.jpg"));
	SDL_Texture *segmentation = SDL_CreateTextureFromSurface(renderer, load_image("Data/segmentation.jpg"));

	int display_button = false;
	int nb_char = 0;
	int first_change = true;

	int texW = 0;
	int texH = 0;

	/*=========================================================
  =========================================================*/
	//loop until the window is closed
	while(!done)
	{
		SDL_WaitEvent(&event);
        switch(event.type)
        {
        	//Stop the loop
           case SDL_QUIT:
            done = true;
            break;
            //Add the caracter to the path
           case SDL_TEXTINPUT:
           if (!display_button)
           {
           	nb_char++;
           	strcat(txt, event.text.text);
           	surface_text = TTF_RenderText_Blended(police, txt, vert);
           	t = SDL_CreateTextureFromSurface(renderer, surface_text);
           }
        	break;
        	//if we click on a button do what he's supposed to
        	//do
           case SDL_MOUSEBUTTONUP:
           	if (event.button.x > texW/2 && event.button.x < texW/2+200)
           	{
           		int y = event.button.y;
           		char *s;
           		if (y < 800)
           			s = "0";
           		if (y < 600 && y > 400)
           		{
	           		if (!first_change)
	           			t = res;
	           		first_change = false;
           			int h = surface_text->h;
           			int w = surface_text->w;
           			Uint8 binaryArray[h][w];
           			AllTreatment(h, w, binaryArray, surface_text, "");
           			InitFile(h,w,binaryArray, surface_text);
           			display_Array(h, w, binaryArray, tmp);
					res = SDL_CreateTextureFromSurface(renderer, tmp);break;
           		}
           		if (y < 400)
           			s = "b";
           		if (y < 200)
           			s = "g";
           		if (!first_change)
           			t = res;
           		first_change = false;
           		int h = surface_text->h;
				int w = surface_text->w;
				Uint8 binaryArray[h][w];
				AllTreatment(h, w, binaryArray, surface_text, s);
				display_Array(h, w, binaryArray, tmp);
				res = SDL_CreateTextureFromSurface(renderer, tmp);

           	}
            break;
           case SDL_KEYDOWN:
           //Open the image at path
           	if (event.key.keysym.sym == SDLK_RETURN && !display_button)
            {
            	surface_text = load_image(txt);
            	tmp = load_image(txt);
            	t = SDL_CreateTextureFromSurface(renderer, surface_text);
            	display_button = true;
            	break;
            }
            //Delete a character of the path when backspace
            if (event.key.keysym.sym == SDLK_BACKSPACE && !display_button)
            {
            	if (nb_char > 0)
            	{
            		txt[nb_char-1] = 0;
            		nb_char--;
            		surface_text = TTF_RenderText_Blended(police, txt, vert);
           			t = SDL_CreateTextureFromSurface(renderer, surface_text);
            	}
            }
        }
        //Find the right size for the images
        SDL_QueryTexture(t, NULL, NULL, &texW, &texH);
        SDL_GetWindowSize(window, &w, &h);
        if(texH > h)
        	texH = h;
        if(texH < h && display_button)
        	texH = h;

        if(texW > w-200)
        {
        	texW = w-200;
        }
        if (texW < w && display_button)
        {
        	texW = w-200;
        }

		SDL_Rect dstrect = {0, 0, texW/2, texH};
		SDL_Rect dstres = {(texW/2+200), 0, texW/2, texH};
		all.y = 0;
		all.x = texW/2;

		//Put all the images together
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderCopy(renderer, t, NULL, &dstrect);
		if (display_button)
		{
			SDL_RenderCopy(renderer, gray, NULL, &all);
			all.y = 200;
			SDL_RenderCopy(renderer, binarize, NULL, &all);
			all.y = 400;
			SDL_RenderCopy(renderer, segmentation, NULL, &all);
			SDL_RenderCopy(renderer, res, NULL, &dstres);
		}
        SDL_RenderPresent(renderer);
	}

	//FREE MEMORY

	TTF_CloseFont(police);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface_text);
	SDL_FreeSurface(tmp);
	SDL_DestroyTexture(segmentation);
	SDL_DestroyTexture(gray);
	SDL_DestroyTexture(t);
	SDL_DestroyTexture(res);
	SDL_DestroyTexture(binarize);
	SDL_FreeSurface(ecran);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	TTF_Quit();

	printf("OK\n");
	return 0;
}