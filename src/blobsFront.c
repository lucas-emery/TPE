#include <stdio.h>
#include <stdlib.h>
#include "getnum.h"
#include "blobsBack.h"
#define DIMX 5
#define DIMY 5
#ifdef _WIN32
	#define PLAYER1 "|A"
	#define PLAYER2 "|Z"
	#define CLEAR "cls"
#else
	#define PLAYER1 "|\x1b[36;1mO\x1b[0m" //Blue
	#define PLAYER2 "|\x1b[31;1mO\x1b[0m" //Red
	#define CLEAR "clear"
#endif

int main(int argc, char **argv) {

	int blobCount[3] = {0,2,2};
	typeBoard board;
	printf("La matriz:\n");
	board.h = getint("Ingrese altura:\n");
	board.w = getint("Ingrese ancho:\n");
	init(&board);
	fill(&board);

	gameState state = GAME;
	typeCommand command;
	char *filename, *retValue;
	int player = rand()%2 + 1; /* Generate a number between 1 & 2 (0 is not included) */

	while(state != QUIT) {
		switch(state) {
			case MENU:
				printf("\t\t\tBienvenido al juego Guerra de Manchas (Blob Wars)\n"); //SPACEHOLDER
				printf("1. Juego de dos jugadores\n");
        printf("2. Juego contra computadora\n");
        printf("3. Recuperar un juego guardado\n");
        printf("4. Terminar\n");
        printf("Elegir opción: \n");
				while(getchar() != '\n'); /* getnum or getchar? */
				break;

			case GAME:
				//Generate board if not loaded (wip)
				render(&board, blobCount);
				if(canMove(player, &board)) {
					//IMPRIMIR TURNO Y COMANDOS DISPONIBLES
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
					if(state == GAME) {
						blobCount[player] += move(player, &command, &board);
						conquer(player, &command, &board, blobCount);
						if(player == 1)
							player = 2;
						else
							player = 1;
					}
				}
				else
					state = END;//Someone won
				break;

			case SAVE:
				printf("Game %s saved!", filename); //SPACEHOLDER
				while(getchar() != '\n');
				state = GAME;
				break;

			case LOAD:
				break;

			case END:
				if(blobCount[1] > blobCount[2])
					player = 1;
				else
					player = 2;
				printf("Player %d won!", player);
				while(getchar() != '\n');
				state = MENU;
				break;

			//Other cases
		}
	}
	return 0;
}


void render(typeBoard* board, const int blobCount[]){

  system(CLEAR);

  int i,j;
  printf("P1:%d\tP1:%d\n", blobCount[1], blobCount[2]);
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

int save(/*...WIP...*/) /* ERROR HANDLING: DEVUELVE SI HUBO UN PROBLEMA AL GUARDAR */
;
int load(/*...WIP...*/)  /* ERROR HANDLING: DEVUELVE SI HUBO UN PROBLEMA AL CARGAR, EJ. CORRUPTO O NO EXISTE */
;
