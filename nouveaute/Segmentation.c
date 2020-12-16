#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utility_SDL.h"
#include <stdio.h>
#include "stdbool.h"
#include "vector.h"
#include "Reseau.h"


SDL_Surface * image;

void CheckLine(int h, int w, Uint8 binaryArray[h][w])
{
  int isLine = true;
  for (int i = 0;i < h; ++i)
    {
      for (int j = 0; j < w; ++j)
	{
	  if (binaryArray[i][j] == 0 && isLine)
	    {
	     isLine = false;
	      j = 0;
	    }
	  if (binaryArray[i][j] == 127)
	    {
	      binaryArray[i][j] = 255;
	    }
	  if (isLine)
	  {
	    binaryArray[i][j] = 127;
	  }
	}
      isLine = true;
    }
}

void CheckColumn(int h, int w, Uint8 binaryArray[h][w])
{
  int isColumn = true;
  int mem = 0;
  for (int i = 0; i < h; ++i)
    {
      mem = i;
      for(int j = 0;j < w; ++j)
    	{
	  i = mem;
	  isColumn = true;
	  for (;i < h && binaryArray[i][0]!= 127; ++i)
	    {
	      if (binaryArray[i][j] == 0 && isColumn)
		{
		  isColumn = false;
		  i = mem;
		}
	      if (binaryArray[i][j] == 0 && !isColumn)
		{
		  break;
		}
	      if (binaryArray[i][j] == 128 /*optionel*/&& !isColumn )
		{
		  binaryArray[i][j] = 255;
		}
	      if (isColumn)
		{
		  binaryArray[i][j] = 128;
		} 
	    }
    	}
  
    }
}

void InitFile(int h,int w, Uint8 binaryArray[h][w],SDL_Surface * image_s)
{
  image = image_s;
  CheckLine(h, w, binaryArray);
   CheckColumn(h, w, binaryArray);
}


