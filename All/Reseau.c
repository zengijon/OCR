#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*=============================================
            --Outils mathematique--  
===============================================*/

float sigmoid(float x){
  return 1/(1+exp(-x));
}

float OutputError(float o, float t){
  return (t-o)*o*(1-o);
}

float WeightUpdate(float d, float o){
  /*---   V = 0.1   ---*/
  return d * 0.1 *o;
}

/*============================================*/



void InitGraph(float image[28][28], float weight[830][830],float neurone[830])
{
  for(int i = 0;i<784;i+=2)
    {
      neurone[i]=image[i%28][i/28];
      neurone[i+1]=1;
    }
  
  for(int i = 784;i<830;++i)
    neurone[i] = 1;
 
  for(int i = 1;i<784;i++)
    for(int j = 784;j<814;++j)
      weight[i][j]=0.5;
  
  for(int i = 784;i<814;++i)
    for(int j = 814;j<830;++j)
      weight[i][j]=0.5;
}

void RunReseau(float weight[830][830],float neurone[830]){
  for(int j = 784;j<814;j+=2)
    {
      float s = 0;
      for(int i = 0;i<784;i++)
	  s += weight[i][j]*neurone[i];
      neurone[j]=sigmoid(s+weight[j+1][j]);
    }
  for(int j = 814;j<830;j+=2)
    {
      float s = 0;
      for(int i = 784;i<814;i+=2)
	s += weight[i][j]*neurone[i];
      neurone[j]=sigmoid(s + weight[j+1][j]);
    }
}

void BackProp(float weight[830][830],float neurone[830],float target[8])
{
  for(int i = 814; i<830; i+=2)
    {
      float d = OutputError(neurone[i],target[i-814]);
      for(int j = 784; j<814;j+=2)
	weight[j][i] += WeightUpdate(d,neurone[i]);
      weight[i+1][i] += WeightUpdate(d,neurone[i]);
    }

  for(int i = 784; i<814;i+=2)
    {
      float s =0;
      for(int j = 814; j<830;j+=2)
	{
	  s +=OutputError(neurone[j],target[j-814])*weight[i][j];
	}
      float d = 0;
      for(int k = 0; k<784;++k)
	{
	  d += s*weight[k][i];
	}
      for(int k = 0; k<784;++k)
	{
	  weight[k][i] +=  WeightUpdate(d,neurone[k]);
	}
      weight[i+1][i] +=  WeightUpdate(d,neurone[i+1]);
    }    
}

/*=============================================
                --Save & Load--  
===============================================*/

/*void save(float weight[830][830])
{
    FILE* sav = fopen("save.txt", "w+");
    for (int i = 0; i < 784; i++)
    {
        for (int j = 784; j < 814; j+=2)
        {
            char point[20];
            gcvt(weight[i][j], 6, point);
            fputs(point,sav);
            fputc('\n', sav);
       } 
    }
    for (int i = 784; i < 814; i+=2)
    {
        for (int j = 814; j < 830; j+=2)
        {
            char point[20];
            gcvt(weight[i][j], 6, point);
            fputs(point,sav);
            fputc('\n', sav);
       } 
    }
    
    fclose(sav);
}

void load(float weight[830][830])
{
    FILE* sav = fopen("save.txt", "r");
    if (sav == NULL)
        return;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            char point[20];
            fgets(weight, 15, sav);
            float res = atof(point);
            weight[i][j] = res;
        }
    }
    fclose(sav);
}
*/
/*=============================================
        --A few fuction to rules them All--  
===============================================*/

int binArray_to_int(float neurone[830]){
  int res = 0;
  int power = 1;
  for (int i = 814; i<830; i+=2)
    {
      if (neurone[i] > 0.8)
	res+= power;
      power *=2;
    }
  return res;
}

int Reseau(float image[28][28])
{
  float weight[830][830];
  float neurone[830];
  InitGraph(image, weight, neurone);
  //BackProp(weight, neurone,target);
  RunReseau(weight, neurone);
  return 0;
}

/*int main(){

  return 0;
}
*/
