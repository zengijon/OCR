#include "Headers/Reseau.h"
#include <stdio.h>
#include <SDL2/SDL.h>

int number = 0;

//Change the form of the char to be always the same (28x28)
char scale_down_to_28(int h, int w,Uint8 binaryArray[h][w],char c)
{
  float ratio_h = ((float) h) / 28;
  float ratio_w = ((float) w) / 28;
  float binaryArray28[28][28];
  for(int i = 0; i < 28; i++)
      for (int j = 0;  j < 28; j++)
        {
          float tmp = (float)binaryArray[(int)(i*ratio_h+0.5)][(int)(j*ratio_w+0.5)];
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
  return Reseau(binaryArray28, c);
}

//Met les charactere en gris afin de ne pas les reprendre en compte
char white_to_vector(int h, int w, int i, int j, Uint8 binaryArray[h][w], char c)
{
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


  return scale_down_to_28(_h, _w,NewArray, c);
}

//FOnction principale qui cherche les caractères
void search_segmented(int h, int w, Uint8 binaryArray[h][w])
{
  int k =0;
  char target[26] = {'a','z','e','r','t','y','u','i','o','p','q','s','d','f','g','h','j','k','l','m','w','x','c','v','b','n'} ;

  Uint8 binary[h][w];
  memcpy(binary, binaryArray, sizeof(Uint8) * h * w);

  FILE* text_res = fopen("Save/resultat.txt", "w+");

  for (int i = 0; i < h; ++i)
  {
    for(int j = 0;j < w; ++j)
      { 
        if (binary[i][j] == 0 || binary[i][j] == 255)
        {
          number++;
          char c = white_to_vector(h, w,i,j, binary, target[k++%26]);
          fputc((int) c, text_res);
        }
      }
    }

    fclose(text_res);
}