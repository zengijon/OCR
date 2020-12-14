#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float activation[3][3] = { { 0, 1, 0 }, { 0,0,0 }, {1, 0/*z*/,0} };
float poids[3][3] = { { 0 /*x/h*/,0 /*bh*/,0 /*y/h*/}, { 0/*xz*/,0/*hz*/,0/*yz*/ }, {0/*bzh*/,0,0} };
float exemple[2][2] = { {0,1}, {1,0} };

float sigm(float x)
{
    return (1 / (1 + expf(-x)));
}
void test(float x, float y)
{
    activation[0][0] = x;
    activation[0][2] = y;
    activation[1][1] = sigm(x * poids[0][0] + activation[0][1]*poids[0][1] + y*poids[0][2]);
    activation[2][1] = sigm(x * poids[1][0] + activation[1][1] * poids[1][1] 
        + y * poids[1][2] + activation[2][0]*poids[2][0]);
}
float error_res()
{
    int x = activation[0][0];
    int y = activation[0][2];
    return (exemple[x][y] - activation[2][1])*activation[2][1]*(1-activation[2][1]);
}
void update_res()
{
    float pas = 0.5;
    float error = error_res();
    poids[1][0] += pas * error * activation[0][0];
    poids[1][1] += pas * error * activation[1][1];
    poids[1][2] += pas * error * activation[0][2];
    poids[2][0] += pas * error * activation[2][0];
}
float error_h()
{
    float h = activation[1][1];
    return h * (1 - h) * error_res() * poids[1][1];
}
void update_h()
{
    float pas = 0.5;
    float error = error_h();
    poids[0][0] += pas * error * activation[0][0];
    poids[0][1] += pas * error * activation[0][1];
    poids[0][2] += pas * error * activation[0][2];
}
void save()
{
    FILE* sav = fopen("save.txt", "w+");
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            char point[20];
            gcvt(poids[i][j], 6, point);
            fputs(point,sav);
            fputc('\n', sav);
        }
    }
    fclose(sav);
}
void load()
{
    FILE* sav = fopen("save.txt", "r");
    if (sav == NULL)
        return;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            char point[20];
            fgets(point, 15, sav);
            float res = atof(point);
            poids[i][j] = res;
        }
    }
    fclose(sav);
}
void randomizer()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            poids[i][j] = (float)rand() / (float)RAND_MAX;
        }
    }
}

int main()
{
    randomizer();

    load();

    float x = 0, y = 1;

    for (int i = 0; i < 100000; i++)
    {
        x = rand() % 2;
        y = rand() % 2;

        test(x, y);


        update_res();
        update_h();
    }
    
    save();
    return 0;
}
