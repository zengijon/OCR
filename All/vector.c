#include <err.h>
#include "vector.h"

struct vector *vector_new()
{
  struct vector* vector = NULL;
  int* data = NULL;
  vector = malloc(sizeof(size_t)*2+sizeof(int*));
  data = malloc(sizeof(int));
  if (data == NULL || vector == NULL)
    errx(1,"Not enough memory!");
  vetcor -> height = 0;
  vector -> weight = 0; 
  vector -> capacity = 1;
  vector -> size = 0;
  vector -> data = data;
  return vector;
}

void vector_free(struct vector *v)
{
  free(v -> data);
  free(v);
}

void double_capacity(struct vector *v)
{
  v-> capacity *=2; 
  realloc(v->data,sizeof(int)*(v->capacity));
}

void vector_push(struct vector *v, int x)
{
  if (v-> size == v -> capacity)
    double_capacity(v);
  v -> data[v->size] = x;
  v->size++;
}

int vector_pop(struct vector *v, int *x)
{
  if (!v -> size)
    return 0;
  v->size--;
  *x = v -> data[v->size];
  return 1;
}

int vector_get(struct vector *v, size_t i, int *x)
{
  if (i >= v -> size)
    return 0;
  *x = v -> data[i];
  return 1;
}

void vector_insert(struct vector *v, size_t i, int x)
{
  if (i >= v -> size)
    return;
  vector_push(v,v -> data[v->size-1]);
  for (int j = v->size-2;j>i;--j)
    v -> data[j] = v ->data[j-1];
  v-> data[i]=x;
}

int vector_remove(struct vector *v, size_t i, int *x)
{
    if (i >= v -> size)
    return 0 ;
    *x = v -> data[i];
    for (int j = i;j<v->size-1;--j)
      v -> data[j] = v ->data[j+1];
    v->size--;
    return 1;
}
