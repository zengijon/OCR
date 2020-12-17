#include "Reseau.h"
#include <stdio.h>
#include <SDL2/SDL.h>

int number = 0;

char scale_down_to_28(int h, int w,Uint8 binaryArray[h][w],char c)
{
  double ratio_h = ((double) h) / 28;
  double ratio_w = ((double) w) / 28;
  double binaryArray28[28][28];
  for(int i = 0; i < 28; i++)
      for (int j = 0;  j < 28; j++)
        {
          double tmp = (double)binaryArray[(int)(i*ratio_h+0.5)][(int)(j*ratio_w+0.5)];
          if (tmp == 0)
          {
            binaryArray28[i][j] = 0;
          }
          else if (tmp == 255)
          {
            binaryArray28[i][j] = 1;
          }
          else 
            binaryArray28[i][j] = tmp/255;
          
        }
  //printf("Scale_Down %d\n", number);
  return Reseau(binaryArray28, c);
}

char white_to_vector(int h, int w, int i, int j, Uint8 binaryArray[h][w], char c)
{
  //printf("White Vector begin %d\n", number);
  int _h =0;
  int _w =0;
  for (; _h + i < h && (binaryArray[_h+i][j] == 0 || binaryArray[_h+i][j] == 255) ; _h++);
  for(; _w+j < w && (binaryArray[i][_w+j] == 0 || binaryArray[i][_w+j] == 255) ; _w++);
  Uint8 NewArray[_h][_w];
  
  for (int y = _h-1;y>-1;y--)
    for(int k = _w-1;k>-1;k--)
      {
        NewArray[y][k] = binaryArray[i+y][j+k];
        binaryArray[i+y][j+k] = 129;
      }

  //printf("White Vector end %d\n", number);

  return scale_down_to_28(_h, _w,NewArray, c);
  /*for (int i = 0; i < count; ++i)
  {
    for (int i = 0; i < count; ++i)
    {

    }
  }*/
}

void search_segmented(int h, int w, Uint8 binaryArray[h][w])
{
  int k =0;
  char target[26] = {'a','z','e','r','t','y','u','i','o','p','q','s','d','f','g','h','j','k','l','m','w','x','c','v','b','n'} ;
for (int i = 0; i < h; ++i)
  {
    for(int j = 0;j < w; ++j)
      { 
        //printf("ifstart\n");
        if (binaryArray[i][j] == 0 || binaryArray[i][j] == 255)
        {
          number++;
          //printf("Segmented : %d\n", number);
          white_to_vector(h, w,i,j, binaryArray, target[k++%26]);
        }
        //printf("forj\n");
      }
      //printf("%d\n", i);
    }
}