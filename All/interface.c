#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

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
	int done = false;
	SDL_Event event;
	init_sdl();
	TTF_Init();
	int w = 0;
	int h = 0;
	SDL_Window * window = SDL_CreateWindow("OCR",
					 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
					 640, 480, SDL_WINDOW_RESIZABLE);
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Color noire = {255, 255, 255, 0};
	char txt[300];

	TTF_Font *police = TTF_OpenFont("arial.ttf", 25);
	SDL_Surface *text = TTF_RenderText_Blended(police, "", noire);
	SDL_Texture *blit = SDL_CreateTextureFromSurface(renderer,load_image("test.jpg"));
	SDL_Surface *ecran = load_image("rectangle-noir.jpg");
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, ecran);
	SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, text);
	SDL_Rect all = {0, 0, 200, 200};

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
            strcat(txt, event.text.text);
        	text = TTF_RenderText_Blended(police, txt, noire);
			t = SDL_CreateTextureFromSurface(renderer, text);
        	break;
           case SDL_MOUSEBUTTONUP:
            printf("x : %d, y : %d\n", event.button.x, event.button.y);
            break;
           case SDL_KEYDOWN:
           	if (event.key.keysym.sym == SDLK_RETURN)
            {
            	text = load_image(txt);
            	t = SDL_CreateTextureFromSurface(renderer, text);
            	break;
            }
        }
        SDL_QueryTexture(t, NULL, NULL, &texW, &texH);
        SDL_GetWindowSize(window, &w, &h);
        if(texH > h)
        	texH = h;

        if(texW > w-200)
        {
        	texW = w-200;
        }

		SDL_Rect dstrect = {0, 0, texW, texH};
		all.x = texW;
		//SDL_Rect real = {0, 0, texW+200, texH+200};
		//SDL_BlitSurface(blit, NULL, text, &all);
		//texture = SDL_CreateTextureFromSurface(renderer, ecran);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderCopy(renderer, t, NULL, &dstrect);
		SDL_RenderCopy(renderer, blit, NULL, &all);
        SDL_RenderPresent(renderer);
	}

	//FREE SURFACE

	TTF_CloseFont(police);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(text);
	SDL_DestroyTexture(blit);
	SDL_FreeSurface(ecran);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	TTF_Quit();

	printf("OK\n");
	return 0;
}