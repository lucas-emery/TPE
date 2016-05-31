#include <stdio.h>
#include <stdlib.h>
#include "getnum.h"
#define DIMX 5
#define DIMY 5


int
main(void){

	typeBoard board;

	printf("La matriz:\n");
	board.h = getnum("Ingrese altura:\n");
	board.w = getnum("Ingrese ancho:\n");
	render(&board);

}


void render(typeBoard* board){

	int i,j;
	for(i=0;i < board->h;i++){
		for(j=0;j < board->w;j++)
		{
			printf("")
		}
	}
}

void init(typeBoard * board){ //mover al back
	int* temp;

	board->get = malloc(board->h * sizeof(int*));
	temp = malloc(board->h * board->w * sizeof(int));
	for (int i = 0; i < board->h; i++) {
  		board->get[i] = temp + (i * board->w);
	}
}
