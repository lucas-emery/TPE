#include <stdio.h>
#include <stdlib.h>
#include "getnum.h"
#define DIMX 5
#define DIMY 5

typedef struct{

int h;
int w;
char **get;

}typeBoard;(


int
main(void){
     
	typeBoard tableroCabeza;

	printf("La matriz:\n");
	tableroCabeza.h = getnum("Ingrese altura:\n");
	tableroCabeza.w = getnum("Ingrese ancho:\n");
	render(&tableroCabeza);

}


void render(typeBoard* board){
	int i,j;


}
