#ifndef PRETREATMENT_H
#define PRETREATMENT_H

void arrayd(int h, int w, SDL_Surface* image, Uint8 binaryArray[h][w]);
//void AllTreatment(int h, int w, Uint8 Array[h][w], SDL_Surface* image, char* type);
void Convolution(int h, int w, int matrix[3][3],Uint8 binaryArray[h][w]);
int Binarize(int h, int w, Uint8 binaryArray[h][w], SDL_Surface *image, int s);

#endif

