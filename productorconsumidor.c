/*
**  PROGRAM: Ejemplo de productor consumidor
**
**  
**   Version original de Tim Mattson, Intel.
*/
#include "omp.h"
#include <malloc.h>
#include <stdio.h>

#define N        10000

/* Constantes para generar numeros aleatorios (numerical recipies) */
#define SEED       2531
#define RAND_MULT  1366
#define RAND_ADD   150889
#define RAND_MOD   714025
int randy = SEED;

/* llenar un array con numeros aleatorios (Productor) */
void fill_rand(int length, double *a)
{
   int i; 
   for (i=0;i<length;i++) {
     randy = (RAND_MULT * randy + RAND_ADD) % RAND_MOD;
     *(a+i) = ((double) randy)/((double) RAND_MOD);
   }   
}

/* Sumar los elementos de un array (Consumidor) */
double Sum_array(int length, double *a)
{
   int i;  double sum = 0.0;
   for (i=0;i<length;i++)  sum += *(a+i);  
   return sum; 
}
  
int main()
{
  double *A, suma, tiempo;
  int flag = 0;

  A = (double *)malloc(N*sizeof(double));

  tiempo = omp_get_wtime();

  fill_rand(N, A);        // Productor

  suma = Sum_array(N, A);  // Consumidor
   
  tiempo = omp_get_wtime() - tiempo;

  printf(" La suma es %f en %f segundos\n",suma, tiempo);
}
 
