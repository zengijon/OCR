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
  //return t-o;
}

float WeightUpdate(float d, float o){
  /*---   V = 0.01   ---*/
  return d * 0.1 *o;
}

/*=============================================
                --Save & Load--  
  ===============================================*/

void save(float weight[914][914])
{
  FILE* sav = fopen("Save/save.txt", "w+");
  for (int i = 0; i < 784; i++)
  {
    for (int j = 784; j < 862; j+=2)
    {
     char point[15];
     sprintf(point,"%f",weight[i][j]);
     fputs(point,sav);
     fputc('\n', sav);
   } 
 }
 //printf("lolilol\n");

 for (int i = 784; i < 862; i+=2)
 {
  for (int j = 862; j < 914; j+=2)
  {
   char point[15];
   sprintf(point,"%f",weight[i][j]);
   fputs(point,sav);
   fputc('\n', sav);
 } 
}


fclose(sav);
}

void load(float weight[914][914])
{
  FILE* sav = fopen("Save/save.txt", "r");
  if (sav == NULL)
    {
      for (int i =0; i < 914;++i)
	       for (int j = 0;j< 914;++j)
            weight[i][j] = (float)rand() / (float)RAND_MAX;
      return;
    }
  for (int i = 0; i < 3; i++)
    {
      for (int j = 0; j < 3; j++)
        {
	  char point[20];
	  fgets(point, 15, sav);
	  float res = atof(point);
	  weight[i][j] = res;
        }
    }
  fclose(sav);
}

/*=============================================*/


void Init_Reseau(float image[28][28],float target[26],float neurone[914],float weight[914][914],char c)
{
  for (int i =0; i< 26;++i)
    target[i]=0;
  target[((int) c) - 97] = 1;
  
  for(int i = 0;i<784;i++)
    {
      neurone[i]=image[i%28][i/28];
    }
  
  for(int i = 784;i<914;++i)
    neurone[i] = 1;

  load(weight);
  //printf("%f\n", weight[0][784]);
}

void RunReseau(float weight[914][914],float neurone[914]){

  
  for(int j = 784;j<862;j+=2)
    {
      float s = 0;
      for(int i = 0;i<784;i++)
      {
        s += weight[i][j]*neurone[i];
      }
      neurone[j] = sigmoid(s+weight[j+1][j]);
      //printf("%f\n", neurone[j]);
    }
  
  for(int j = 862;j<914;j+=2)
    {
      float s = 0;
      for(int i = 784;i<862;i+=2)
	      s += weight[i][j]*neurone[i];
      neurone[j]=sigmoid(s + weight[j+1][j]);
    }
}

/*void BackProp(float d_error[26])
{
  float weight[914][914];

  load(weight);

  printf("start back\n");
  fflush(stdout);
  for(int i = 862; i<914; i+=2)
    {
      //printf("neurone : %f target : %f\n", neurone[i], target[(i-862)/2]);
      for(int j = 784; j<862;j+=2)
        weight[j][i] += d_error[(i-862)/2];

      weight[i+1][i] += d_error[(i-862)/2];
      //printf("d start : %f\n", d);
    }
  for(int i = 784; i<862;i+=2)
    {
      float d = 0;
      for(int j = 862; j<914;j+=2)
      {
       d += d_error[(j-862)/2]*weight[i][j];
      }
     d *=(1-d)*d;
      for(int k = 0; k<784;++k)
     {
       weight[k][i] +=  d;
     }
      weight[i+1][i] +=  d;
    } 

    save(weight);   
    printf("end back\n");
}*/


void Error(float d_array[26], float neurone[914], float target[26])
{
  for(int i = 862; i<914; i+=2)
  {
    //printf("%d\n", (i-862)/2);
      //printf("neurone : %f target : %f\n", neurone[i], target[(i-862)/2]);
    d_array[(i-862)/2] = OutputError(neurone[i],target[(i-862)/2]);
      //printf("d start : %f\n", d);
  }
}

void BackProp(float weight[914][914],float neurone[914],float target[26])
{
  float d;
  for(int i = 862; i<914; i+=2)
    {
      //printf("neurone : %f target : %f\n", neurone[i], target[(i-862)/2]);
      d = OutputError(neurone[i],target[(i-862)/2]);
      for(int j = 784; j<862;j+=2)
	       weight[j][i] += WeightUpdate(d,neurone[i]);
      
      weight[i+1][i] += WeightUpdate(d,neurone[i]);
      //printf("d start : %f\n", d);
    }

  for(int i = 784; i<862;i+=2)
    {
      d =0;

      for(int j = 862; j<914;j+=2)
      {
	     d +=OutputError(neurone[j],target[(j-862)/2])*weight[i][j];
	    }

	    d *=(1-d)*d;
      for(int k = 0; k<784;++k)
	    {
	     weight[k][i] +=  WeightUpdate(d,neurone[k]);
	    }

      weight[i+1][i] +=  WeightUpdate(d,neurone[i+1]);
    }    
    //printf("d end : %f\n", d);
}

char binArray_to_letter(float neurone[914])
{
  char alphabet[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
  float max = neurone[862];
  int max_index= 862;
  for (int i = 862; i<914; i+=2)
    {
      if (neurone[i]>max)
	   {
	     max = neurone[i];
       max_index = i;
	   }
    }
  //printf("index : %d, max : %f\n", max_index-862, max);
  return alphabet[(max_index-862)/2];
}

void Print_return_Array(float neurone[914])
{
  printf("[");
  for (int i = 862; i<914; i+=2)
    printf("%f, ",neurone[i]);
  printf("]\n");
}

char Reseau(float image[28][28], char c)
{
  float neurone[914];
  float weight[914][914];
  float target[26];
  Init_Reseau(image,target,neurone,weight,c);

  RunReseau(weight, neurone);
  BackProp(weight, neurone, target);

  save(weight);
  char res = binArray_to_letter(neurone);

  printf("res :%c expect : %c\n",res, c);
  //fflush(stdout);
  //Print_return_Array(neurone);

  return res;
}
