#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "Headers/utility_SDL.h"
#include <math.h>

float nb_pixels;
int histo[256];

int blur[3][3]={{1,1,1},{1,1,1},{1,1,1}};
int median[3][3]={{1,4,5},{3,9,6},{8,10,2}};
int smouth[3][3]={{1,2,1},{2,4,2},{1,2,1}};

/*=========================================================
             Calcul du seuil pour la binarisation
  =========================================================*/

//Fonction annexe (somme histo[i]/pi)
float sum_pi(int i)
{
  float sum = 0;
  for(int j = 0; j <= i; ++j)
    sum += histo[j]/nb_pixels;
  return sum;
}

//Fonction annexe (somme histo[i]/pi * i)
float m(int i)
{
  float sum = 0;
  for(int j = 0; j <= i; ++j)
    sum += (histo[j]/nb_pixels)*j;
  return sum;
}

//Calcul final du seuil minimisant la variance
int max_s()
{
  int j = 0;
  float s_max = 0;
  float wi;
  float tp;
  float x;
  for (int i = 0; i < 256; ++i)
    {
      wi = sum_pi(i);
      tp = m(256)*wi - m(i);
      x = wi*(1-wi)*tp*tp;
      if(s_max < x)
	{
	  s_max = x;
	  j = i;
	}
    }
  return j;
}

/*=========================================================
  =========================================================*/

/*
- Transforme l'image en niveau de gris et la met dans une matrice
- Enregistre la matrice dans binaryArray
- Crée l'histogramme des valeurs des pixels en Grayscale
*/
void GreyScaleArray (int h, int w, SDL_Surface* image, Uint8 binaryArray[h][w])
{
  Uint8 r = 0;
  Uint8 g = 0;
  Uint8 b = 0;
  Uint8 a = 0;
  Uint8 moyenne = 0;
  Uint32 ip;
  
  for(int i = 0; i<256; ++i)
    histo[i] = 0;
  
  for(int i = 0; i < h; i++)
    {
      for (int j = 0;  j < w; j++)
	{
	  ip = getpixel(image, j, i);
	  SDL_GetRGBA(ip,image->format, &r,&g,&b,&a);
	  //Formule pour obtenir le niveau de gris à partir des couleurs
	  moyenne = 0.3*r + 0.59*g + 0.11*b;
	  ++histo[moyenne]; 
	  binaryArray[i][j]=moyenne;
        }
    }
}

void arrayd(int h, int w, SDL_Surface* image, Uint8 binaryArray[h][w])
{
  Uint8 r = 0;
  Uint8 g = 0;
  Uint8 b = 0;
  Uint8 a = 0;
  Uint8 moyenne = 0;
  Uint32 ip;

  for(int i = 0; i < h; i++)
    {
      for (int j = 0;  j < w; j++)
        {
          ip = getpixel(image, j, i);
          SDL_GetRGBA(ip,image->format, &r,&g,&b,&a);
          moyenne = 0.3*r + 0.59*g + 0.11*b;
          binaryArray[i][j] = moyenne;
        }
    }
}

//Applique matrice de convolution sur l'image
void Convolution(int h, int w, int matrix[3][3],Uint8 binaryArray[h][w])
{
  Uint8 mem_tampon[h][w];
  int tally;
  for(int i = 1; i < h-1; i++)
    {
      for (int j = 1;  j < w-1; j++)
	{
	  tally =binaryArray[i][j]*matrix[1][1];
	  tally+=binaryArray[i-1][j-1]*matrix[0][0];
	  tally+=binaryArray[i-1][j+1]*matrix[0][2];
	  tally+=binaryArray[i+1][j-1]*matrix[2][0];	  
	  tally+=binaryArray[i+1][j+1]*matrix[2][2];
	  tally+=binaryArray[i-1][j]*matrix[0][1];
	  tally+=binaryArray[i+1][j]*matrix[2][1];
	  tally+=binaryArray[i][j-1]*matrix[1][0];
	  tally+=binaryArray[i][j+1]*matrix[1][2];

	  mem_tampon[i][j] = (Uint8) tally ;
	}
    }
 
  for(int i = 1; i < h-1; i++)
    {
      for (int j = 1;  j < w-1; j++)
	{
	  binaryArray[i][j] = mem_tampon[i][j];
	}
    }
}

//Binarisation de l'image grace au seuil
int Binarize(int h, int w, Uint8 binaryArray[h][w], SDL_Surface *image, int s)
{
  nb_pixels = h*w;
  GreyScaleArray(h, w, image, binaryArray);
  int seuil;
  if (s > 1)
    seuil = s;
  else
    seuil = max_s();
  for(int i = 0; i < h; i++)
    {
      for (int j = 0;  j < w; j++)
	{
	  if (binaryArray[i][j] <= seuil-20)
	    binaryArray[i][j] = 0;
	  else
	    binaryArray[i][j] = 255;
	}
    }
  return seuil;
}

//Fonction principale du fichier rassemblant toutes les fonctions précédante
/*int AllTreatment(int h, int w, Uint8 binaryArray[h][w],SDL_Surface* image, char* t)
{
  nb_pixels = h*w; 
  GreyScaleArray(h, w, image, binaryArray);
  if (strcmp(t, "b") == 0 || strcmp(t, "binarize") == 0 || strcmp(t, "") == 0)
  	return Binarize(h, w, binaryArray, 0);
  return 0;
}*/
