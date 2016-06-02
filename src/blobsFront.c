#include <stdio.h>
#include <stdlib.h>
#include "getnum.h"
#include "blobsBack.h"
#define DIMX 5
#define DIMY 5

void render(typeBoard*board);
void init(typeBoard*board);
void fill(typeBoard*board);

int main(int argc, char **argv){
	typeBoard board;

	if(argc > 1 && *argv[1] == 'd') {
		printf("La matriz:\n");
		board.h = getint("Ingrese altura:\n");
		board.w = getint("Ingrese ancho:\n");
		init(&board);
		fill(&board);

		render(&board);
	}
	else {
		//MAIN POSTA
		gameState state = MENU;
		//typeBoard board; DECLARED BEFORE IF STATEMENT
		char* filename;
		int player;
		switch(state) {
			case MENU:
				break;

			case GAME:
				player = rand()%2; //Just for testing
				//Generate board if not loaded (wip)
				char* retValue;
				typeCommand command;
				while(state == GAME) {
					if(canMove(player, &board)) {
						do {
							retValue = getCommand(&command);
							if(retValue != NULL) {
								if(*retValue == EOF) //PLAYER QUIT
									state = MENU;
								else {
									state = SAVE;
									filename = retValue;
								}
							}
						} while(!validCommand(&command, &board, player));
					}
					else
						state = END;//Someone won
				}
				break;

			case SAVE:
				break;

			case LOAD:
				break;

			case END:
				break;

			//Other cases
		}
	}

	return 0;
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
