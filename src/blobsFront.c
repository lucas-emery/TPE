#include <stdio.h>
#include <stdlib.h>
#include "getnum.h"
#include "blobsBack.h"
#define DIMX 5
#define DIMY 5

void render(typeBoard*board);
void init(typeBoard*board);
void fill(typeBoard*board);
int
main(void){

	typeBoard board;

	printf("La matriz:\n");
	board.h = getint("Ingrese altura:\n");
	board.w = getint("Ingrese ancho:\n");
	init(&board);
	fill(&board);
	render(&board);

}

void fill(typeBoard * board){	//prueba para el switch
 int i,j;
 for(i=0; i < board->h; i++){
 	for(j=0; j < board->w ; j++)
 		board->get[i][j].owner = rand()%3;
 }

}
void render(typeBoard* board){

	int i,j;
	for(i=0;i < board->h;i++){
		for(j=0;j < board->w;j++)
		{
			switch(board->get[i][j].owner){

				case 0:
				printf("| ");
				break;

				case 1:
				printf("|\x1b[31mO\x1b[0m");
				break;

				case 2:
				printf("|\x1b[36mO\x1b[0m");
				break;
			}
		}

		printf("|\n"); //prueba
	}
}

void init(typeBoard * board){ //mover al back
	typeBlob* temp;
	int i;
	board->get = (typeBlob**) malloc(board->h * sizeof(typeBlob*));
	temp = (typeBlob*) malloc(board->h * board->w * sizeof(typeBlob));
	for (i = 0; i < board->h; i++) {
  		board->get[i] = temp + (i * board->w);
	}
}
