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

/*int is_button(int mouse_x, int mouse_y, int src_x, int src_y, int size)
{
	if (mouse_y >= src_y && mouse_y <= src_y+size)
	{
		if (mouse_x >= src_x && mouse_x <= src_x + size)
		{
			return true;
		}
	}
	return false;
}

int mouse(int mouse_x, int mouse_y, int texW)
{

	for (int i = 0; i < count; ++i)
	{
		if (is_button(mouse_x, event.button.y, texW/2, 0, 200) && display_button)
		{
			int h = text -> h;
			int w = text -> w;
			Uint8 binaryArray[h][w];
			AllTreatment(h, w, binaryArray, text, "g");
			display_Array(h, w, binaryArray, text);
			t = SDL_CreateTextureFromSurface(renderer, text);
		}
	}
}*/

int main()
{
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
	char txt[300];

	TTF_Font *police = TTF_OpenFont("arial.ttf", 50);
	SDL_Surface *text = TTF_RenderText_Blended(police, "", vert);
	SDL_Texture *blit = SDL_CreateTextureFromSurface(renderer,load_image("grayscale.jpg"));
	SDL_Surface *ecran = load_image("rectangle-noir.jpg");
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, ecran);
	SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, text);
	SDL_Rect all = {0, 0, 200, 200};
	SDL_Texture *res = SDL_CreateTextureFromSurface(renderer, load_image("rectangle-blanc.jpg"));
	SDL_Surface *tmp = load_image("rectangle-noir.jpg");
	SDL_Texture *binarize = SDL_CreateTextureFromSurface(renderer, load_image("binarize.jpg"));
	SDL_Texture *segmentation = SDL_CreateTextureFromSurface(renderer, load_image("segmentation.jpg"));

	int display_button = false;
	int nb_char = 0;
	int first_change = true;

	int texW = 0;
	int texH = 0;
	while(!done)
	{
		SDL_WaitEvent(&event);
        switch(event.type)
        {
           case SDL_QUIT:
            done = true;
            break;
           case SDL_TEXTINPUT:
           if (!display_button)
           {
           	nb_char++;
           	strcat(txt, event.text.text);
           	text = TTF_RenderText_Blended(police, txt, vert);
           	t = SDL_CreateTextureFromSurface(renderer, text);
           }
        	break;
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
           			int h = text->h;
           			int w = text->w;
           			Uint8 binaryArray[h][w];
           			AllTreatment(h, w, binaryArray, text, "");
           			InitFile(h,w,binaryArray, text);
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
           		int h = text->h;
				int w = text->w;
				Uint8 binaryArray[h][w];
				AllTreatment(h, w, binaryArray, text, s);
				display_Array(h, w, binaryArray, tmp);
				res = SDL_CreateTextureFromSurface(renderer, tmp);

           	}
            break;
           case SDL_KEYDOWN:
           	if (event.key.keysym.sym == SDLK_RETURN && !display_button)
            {
            	text = load_image(txt);
            	tmp = load_image(txt);
            	t = SDL_CreateTextureFromSurface(renderer, text);
            	display_button = true;
            	break;
            }
            if (event.key.keysym.sym == SDLK_BACKSPACE && !display_button)
            {
            	if (nb_char > 0)
            	{
            		txt[nb_char-1] = 0;
            		nb_char--;
            		text = TTF_RenderText_Blended(police, txt, vert);
           			t = SDL_CreateTextureFromSurface(renderer, text);
            	}
            }
        }
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
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderCopy(renderer, t, NULL, &dstrect);
		if (display_button)
		{
			SDL_RenderCopy(renderer, blit, NULL, &all);
			all.y = 200;
			SDL_RenderCopy(renderer, binarize, NULL, &all);
			all.y = 400;
			SDL_RenderCopy(renderer, segmentation, NULL, &all);
			SDL_RenderCopy(renderer, res, NULL, &dstres);
		}
        SDL_RenderPresent(renderer);
	}

	//FREE SURFACE

	TTF_CloseFont(police);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(text);
	SDL_DestroyTexture(blit);
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