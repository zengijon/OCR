#ifndef UTILITY_SDL_H
#define UTILITY_SDL_H

Uint32 getpixel(SDL_Surface *surface, int x, int y);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void display_Array(int h, int w, Uint8 binaryArray[h][w], SDL_Surface* image);
void wait_for_keypressed();
#endif
