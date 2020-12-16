#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*=============================================
  --Outils mathematique--  
  ===============================================*/

double sigmoid(double x){
  return 1/(1+exp(-x));
}

double OutputError(double o, double t){
  return (t-o)*o*(1-o);
}

double WeightUpdate(double d, double o){
  /*---   V = 0.1   ---*/
  return d * 0.1 *o;
}

/*=============================================
  --Save & Load--  
  ===============================================*/

void save(double weight[866][866])
{
  FILE* sav = fopen("save.txt", "w+");
  for (int i = 0; i < 784; i++)
    {
      for (int j = 784; j < 814; j+=2)
        {
	  char point[15];
	  sprintf(point,"%f",weight[i][j]);
	  fputs(point,sav);
	  fputc('\n', sav);
	} 
    }
  
  for (int i = 784; i < 814; i+=2)
    {
      for (int j = 814; j < 866; j+=2)
        {
	  char point[15];
	  sprintf(point,"%f",weight[i][j]);
	  fputs(point,sav);
	  fputc('\n', sav);
	} 
    }
    
  fclose(sav);
}

void load(double weight[866][866])
{
  FILE* sav = fopen("save.txt", "r");
  if (sav == NULL)
    {
      for (int i =0; i < 866;++i)
	for (int j = 0;j< 866;++j)
	  weight[i][j] = (double)rand() / (double)RAND_MAX;
      return;
    }
  for (int i = 0; i < 3; i++)
    {
      for (int j = 0; j < 3; j++)
        {
	  char point[20];
	  fgets(point, 15, sav);
	  double res = atof(point);
	  weight[i][j] = res;
        }
    }
  fclose(sav);
}

/*=============================================*/


void Init_Reseau(double image[28][28],double target[26],double neurone[866],double weight[866][866],char c)
{
  for (int i =0; i< 26;++i)
    target[i]=1;
  target[((int) c) - 97] = 1;
  
  for(int i = 0;i<784;i+=2)
    {
      neurone[i]=image[i%28][i/28];
    }
  
  for(int i = 784;i<866;++i)
    neurone[i] = 1;


  load(weight);
}

void RunReseau(double weight[866][866],double neurone[866]){

  
  for(int j = 784;j<814;j+=2)
    {
      double s = 0;
      for(int i = 0;i<784;i++)
	{
	  //printf("%f\n",weight[i][j]);
	  printf("i = %d, j = %d\n",i,j);
	  //fflush(stdout);
	  s += weight[i][j]*neurone[i];
	  //printf("test2\n");
	  //fflush(stdout);
	}
      neurone[j]=sigmoid(s+weight[j+1][j]);
    }
  
  for(int j = 814;j<866;j+=2)
    {
      double s = 0;
      for(int i = 784;i<814;i+=2)
	s += weight[i][j]*neurone[i];
      neurone[j]=sigmoid(s + weight[j+1][j]);
    }
}

void BackProp(double weight[866][866],double neurone[866],double target[26])
{
  for(int i = 814; i<866; i+=2)
    {
      double d = OutputError(neurone[i],target[i-814]);
      for(int j = 784; j<814;j+=2)
	weight[j][i] += WeightUpdate(d,neurone[i]);
      weight[i+1][i] += WeightUpdate(d,neurone[i]);
    }

  for(int i = 784; i<814;i+=2)
    {
      double s =0;
      for(int j = 814; j<866;j+=2)
	{
	  s +=OutputError(neurone[j],target[j-814])*weight[i][j];
	}
      double d = 0;
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

char binArray_to_letter(double neurone[866])
{
  char alphabet[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
  int max = neurone[814];
  int max_index= 814;
  for (int i = 816; i<866; i+=2)
    {
      if (neurone[i]>max)
	{
	  max = neurone[i];max_index = i;
	}
    }
  return alphabet[(max_index-(814))/2];
}

void Print_return_Array(double neurone[866])
{
  printf("[");
  for (int i = 816; i<866; i+=2)
    printf("%f, ",(double) neurone[i]);
  printf("]\n");
}

char Reseau(double image[28][28], char c)
{
  double neurone[866];
  double weight[866][866];
  double target[26];
  
  Init_Reseau(image,target,neurone,weight,c);
  
  RunReseau(weight, neurone);

 

  
  BackProp(weight, neurone,target);

  //printf("test\n");
  save(weight);

  
  char res = binArray_to_letter(neurone);
  //printf("%c\n",res);
  Print_return_Array(neurone);
  return res;
}
