#include <stdlib.h>
#include <stdio.h>
#include "omp.h"

#define NMAX 200
#define DEBUG 1

struct nodo {
   int inputdata;
   int outdata;
   struct nodo* next;
};

int fibo(int n) {
   int x, y;
   if (n < 2) {
      return (n);
   } else {
      x = fibo(n - 1);
      y = fibo(n - 2);
      return (x + y);
   }
}

void updatenodo(struct nodo* p) 
{
   p->outdata = fibo(p->inputdata);
}

struct nodo* init_list(struct nodo* p, int size, int firstnum) {
    int i;
    struct nodo* head = NULL;
    struct nodo* temp = NULL;
    
    
    head = malloc(sizeof(struct nodo));
    p = head;
    p->inputdata = firstnum;
    p->outdata = 0;
    for (i=0; i< (size-1); i++) {
       temp  = malloc(sizeof(struct nodo));
       p->next = temp;
       p = temp;
       p->inputdata = firstnum + i + 1;
       p->outdata = i+1;
    }
    p->next = NULL;
    return head;
}

int main(int argc, char *argv[]) {
    double tinicio, tfin;
    struct nodo *puntero=NULL;
    struct nodo *temp=NULL;
    struct nodo *iniciolista=NULL;
    struct nodo *arraypunteros[NMAX];
    struct nodo *array[8];
    
    int contador,i;
    
    int tamanyolista, numinicial;
    
    if(argc ==3){
        tamanyolista=atoi(argv[1]);
        numinicial=atoi(argv[2]);
    } else if(argc ==2){
        tamanyolista=atoi(argv[1]);
        numinicial=1;
    } else {
        tamanyolista=40;
        numinicial=1;
    }
    
    printf("La funcion init_list inicializa una lista enlazada de tamaño %d (el tamaño debe ser menor que NMAX=%d)\n",tamanyolista,NMAX);
    printf("La funcion updatenodo procesa un nodo de la lista\n");
    printf("Cada nodo almacenara como salida la sucession de fibonacci, empezando en %d\n", numinicial);      
 
    puntero = init_list(puntero,tamanyolista,numinicial);
    iniciolista = puntero;
#pragma parallel 
    {

    tinicio = omp_get_wtime();
#pragma omp single nowait
    {
    int z=0;
      while (puntero!= NULL) {
	if(z==8){

	  tratararray(array,8);
	  z=0;
	}
        array[z]=puntero;
	z++;
        puntero = puntero->next;
      }

      tratararray(array,z);
    }  
    }
    tfin = omp_get_wtime();
    
    printf("Tiempo procesamiento serie lista: %lf segundos\n", tfin - tinicio);
    
    
    
    
 
    puntero=iniciolista;
    while (puntero != NULL) {
        if(DEBUG) printf("%d : %d\n",puntero->inputdata, puntero->outdata);
        temp = puntero->next;
        free (puntero);
        puntero = temp;
    }  

    

     return 0;
}
// Esta implementación no es tan rápida como rellenar el array de una vez y lanzarlo
// Las secciones solo las ejecuta un thread
void tratararray(struct nodo *array[],int i){
  int j;
  printf("llega al update, %d",i);
#pragma parallel for schedule(dynamic,1) nowait
  for(j=0;j<8;j++){
    if(j<i) updatenodo(array[j]);
  }
}
