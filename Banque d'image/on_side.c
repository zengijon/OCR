#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utility_SDL.h"
#include <stdio.h>
//#include <neuralnetwork.h>


int h;
int w;
SDL_Surface * image;

void GetLetter(Uint8 segmented[h][w],Uint8 binaryArray[h][w],int i,int j)
{
  printf("1");
  binaryArray[i][j]=0;
  segmented[i][j]=255;
  if (i>0 && j>0 && binaryArray[i-1][j-1]==255)
    GetLetter(segmented,binaryArray,i-1,j-1);
  if (i>0 && j<w-1 && binaryArray[i-1][j+1]==255)
    GetLetter(segmented,binaryArray,i-1,j+1);
  if (i<h-1 && j>0 && binaryArray[i+1][j-1]==255)
    GetLetter(segmented,binaryArray,i+1,j-1);
  if (i<h-1 && j<w-1 && binaryArray[i+1][j+1]==255)
    GetLetter(segmented,binaryArray,i+1,j+1);
  if (i>0 && binaryArray[i-1][j]==255)
    GetLetter(segmented,binaryArray,i-1,j);
  if (i<h-1 && binaryArray[i+1][j]==255)
    GetLetter(segmented,binaryArray,i+1,j);
  if (j>0 && binaryArray[i][j-1]==255)
    GetLetter(segmented,binaryArray,i-1,j-1);
  if (j<w-1 && binaryArray[i][j+1]==255)
    GetLetter(segmented,binaryArray,i,j+1);

}

void CleanArray(Uint8 Array[h][w])
{
  printf("é");
    for(int i = 0;i<h;++i)
      for(int j = 0;j<w;++j)
	Array[i][j]=0;
  printf("k");
}

void Segment(Uint8 binaryArray[h][w],Uint8 segmented[h][w])
{
  for(int i = 0;i<h;++i)
    for(int j = 0;j<w;++j)
      {
      	if (binaryArray[i][j]==255)
	  {
	    CleanArray(segmented);
	    GetLetter(segmented,binaryArray,i,j);
	  }	
	// NeuralNetworkTreatment
	//display_Array(h,w,binaryArray, image);
	display_Array(h,w,binaryArray, image);
      }
}

void InitFile(int height,int width, Uint8 binaryArray[height][width],SDL_Surface * image_s)
{

  h = height;
  w = width;
  image = image_s;
  Uint8 segmented[h][w];
  Segment(binaryArray,segmented);
}
