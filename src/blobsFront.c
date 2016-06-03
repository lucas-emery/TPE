#include <stdio.h>
#include <stdlib.h>
#include "getnum.h"
#include "blobsBack.h"
#define DIMX 5
#define DIMY 5
#ifdef _WIN32
	#define PLAYER1 "|A"
	#define PLAYER2 "|Z"
#else
	#define PLAYER1 "|\x1b[36mO\x1b[0m" //Blue
	#define PLAYER2 "|\x1b[31mO\x1b[0m" //Red
#endif

<<<<<<< HEAD
void render(typeBoard*board, const int points[]);
void init(typeBoard*board);
void fill(typeBoard*board);

int main(int argc, char **argv){
	int points[3] = {0,2,2};
	typeBoard board;
	//if(argc > 1 && *argv[1] == 'd') {
		printf("La matriz:\n");
		board.h = getint("Ingrese altura:\n");
		board.w = getint("Ingrese ancho:\n");
		init(&board);
		fill(&board);

		render(&board,points);
	//}
	//else {
		//MAIN POSTA
		gameState state = GAME;
		//typeBoard board; DECLARED BEFORE IF STATEMENT
		typeCommand command;
		char *filename, *retValue;
		int player = 1; //FOR TESTING, should be random or loaded
		while(state != QUIT) {
			switch(state) {
				case MENU:
					printf("This is a menu"); //SPACEHOLDER
					getchar();
					break;
=======
void render(typeBoard *board);
void init(typeBoard *board);
void fill(typeBoard *board);

int main(int argc, char **argv) {
>>>>>>> master

	typeBoard board;
	printf("La matriz:\n");
	board.h = getint("Ingrese altura:\n");
	board.w = getint("Ingrese ancho:\n");
	init(&board);
	fill(&board);

	gameState state = GAME;
	typeCommand command;
	char *filename, *retValue;
	int player = 1; //FOR TESTING, should be random or loaded

	while(state != QUIT) {
		switch(state) {
			case MENU:
				printf("This is a menu"); //SPACEHOLDER
				getchar();
				break;

			case GAME:
				//Generate board if not loaded (wip)
				render(&board);
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
					} while(state == GAME && !validCommand(player, &command, &board));
					blobCount[player] += move(player, &command, &board);
					conquer(player, &command, &board, blobCount);

				}
				else
					state = END;//Someone won
				break;

			case SAVE:
				printf("Game %s saved!", filename); //SPACEHOLDER
				getchar();
				state = MENU;
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
 	for(j=0; j < board->w ; j++){
 		if ( j == 0 && (i == 0 || i == board->h-1)){
 			board->get[i][j].owner = 1;
 			board->get[i][j].canMove = 8;
 		}
 		else if ( j == board->w-1 && (i == 0 || i == board->h-1)){
 			board->get[i][j].owner = 2;
 			board->get[i][j].canMove = 8;
 		}
 		else
 			board->get[i][j].owner = 0;
	 	}
	}
}
<<<<<<< HEAD
void render(typeBoard* board, const int points[]){
=======

void render(typeBoard* board){
>>>>>>> master

	int i,j;
	printf("P1:%d\tP1:%d\n",points[1],points[2]);
	for(i=0;i < board->h;i++){
		for(j=0;j < board->w;j++)
		{
			switch(board->get[i][j].owner){

				case 0:
					printf("| ");
					break;

				case 1:
					printf(PLAYER1);
					break;

				case 2:
					printf(PLAYER2);
					break;
			}
		}

		printf("|\n"); //prueba
	}
}

void init(typeBoard *board){ //mover al back
	typeBlob *temp;
	int i;
	board->get = (typeBlob**) malloc(board->h * sizeof(typeBlob*));
	temp = (typeBlob*) malloc(board->h * board->w * sizeof(typeBlob));
	for (i = 0; i < board->h; i++) {
  		board->get[i] = temp + (i * board->w);
	}
}
