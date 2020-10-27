#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utility_SDL.h"
#include <stdio.h>
#include "stdbool.h"
//#include <neuralnetwork.h>


int h;
int w;
SDL_Surface * image;

void CheckLine(Uint8 binaryArray[h][w])
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
	 // binaryArray[i][j] = 127;
	}
      isLine = true;
    }
}

void CheckColumn(Uint8 binaryArray[h][w])
{
  int isColumn = true;
  int mem = 0;
  int test = 0;
  for (int i = 0; i < h; ++i)
    {
    mem = i;
    for(int j = 0; /*binaryArray[i][0]!= 127 &&*/ j < w; ++j)
    	{
	  i = mem;
	  isColumn = true;
	  for (/*int k = mem*/; binaryArray[i][0]!= 127 && i < h; ++i/*++k*/)
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

void Checktest(Uint8 binaryArray[h][w])
{
  int isColumn = true;
  //int mem = 0;
  //int test = 0;
  for(int i = 0; i < h; ++i)
    {
      if (binaryArray[i][0] != 127)
	for(int j = 0; j < w; ++j)
	  {
	    int k = i;
	    while (k < h)
	      {
		if(isColumn == false && binaryArray[k][j] == 128)
		  binaryArray[k][j] = 255;
		
		if(binaryArray[k][j] == 0 && isColumn)
		  {
		    isColumn = false;
		    k = i;
		  }
		if(!isColumn && binaryArray[k][j] == 0)
		  break;
		if(isColumn && binaryArray[k][j] == 255)
		  binaryArray[k][j] = 128;
		++k;
	      }
	    isColumn = true;
	  }
    }
}

void test(Uint8 binaryArray[h][w])
{
  for(int i = 0; i<120; ++i)
    for(int j = 0; j<100; ++j)
      binaryArray[i][j] = 200;
}


void InitFile(int height,int width, Uint8 binaryArray[height][width],SDL_Surface * image_s)
{
  h = height;
  w = width;
  image = image_s;
  CheckLine(binaryArray);
   CheckColumn(binaryArray);
  //Uint8 segmented[h][w];
  //Segment(binaryArray,segmented);
}


