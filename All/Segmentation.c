#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utility_SDL.h"
#include <stdio.h>
#include "stdbool.h"
#include "vector.h"

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

void scale_down_to_28(int h, int w,int* binaryArray)
{
  float ratio_h = ((float) h) / 28;
  float ratio_w = ((float) w) / 28;
  Uint8 binaryArray28[28][28];
  for(int i = 0; i < 28; i++)
      for (int j = 0;  j < 28; j++)
	  binaryArray28[i][j] = binaryArray[(int)(i*ratio_h+0.5)*w + (int)(j*ratio_w+0.5)];
	    //binaryArray[(int)(i*ratio_h+0.5)][(int)(j*ratio_w+0.5)];
  // call neural network with binaryArray28[28][28]
}

void white_to_vector(int h, int w, int i, int j, Uint8 binaryArray[h][w])
{
  struct vector* v = vector_new();
  for (; i < h && (binaryArray[i][j] == 0 || binaryArray[i][j]) ; v-> height = ++i)
    for(;j < w && (binaryArray[i][j] == 0 || binaryArray[i][j] == 255) ; v-> weight = ++j)
	   vector_push(v,binaryArray[i][j]);
  scale_down_to_28(v -> height,v -> weight, v-> data);
  vector_free(v);
	
}

void search_segmented(int h, int w, Uint8 binaryArray[h][w])
{
  for (int i = 0; i < h; ++i)
      for(int j = 0;j < w; ++j)
	if (binaryArray[i][j] == 0 || binaryArray[i][j] == 255)
	  white_to_vector(h, w,i,j, binaryArray);
}

void InitFile(int h,int w, Uint8 binaryArray[h][w],SDL_Surface * image_s)
{
  image = image_s;
  CheckLine(h, w, binaryArray);
   CheckColumn(h, w, binaryArray);
}


