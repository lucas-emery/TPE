#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "getnum.h"
#include "blobsBack.h"
#include "random.h"
#ifdef _WIN32
	#define PLAYER1 "A"
	#define PLAYER2 "Z"
	#define POINTS1 "P1"
	#define POINTS2 "P2"
	#define CLEAR_SCREEN system("cls")
#else
	#define POINTS1 "\x1b[36;1mP1\x1b[0m"
	#define POINTS2 "\x1b[31;1mP2\x1b[0m"
	#define PLAYER1 "\x1b[36;1mO\x1b[0m" //Blue
	#define PLAYER2 "\x1b[31;1mO\x1b[0m" //Red
	#define CLEAR_SCREEN system("clear")
#endif

int main(int argc, char **argv) {

	int blobCount[3] = {0,2,2};
	typeBoard board;
	printf("La matriz:\n");
	board.h = getint("Ingrese altura:\n");
	board.w = getint("Ingrese ancho:\n");
	init(&board);
	fill(&board);

	gameState state = MENU;
	typeCommand command;
	int vsAI, winner; //winner quedo aca porque no puede estar al principio de un case
	char *filename, *retValue;
	srand(time(NULL));
	int player = rand()%2 + 1; /* Generate a number between 1 & 2 (0 is not included) */

	while(state != QUIT) {
		switch(state) {
			case MENU:

				vsAI=FALSE;
				int aux=0;
				CLEAR_SCREEN;
				printf("=====Bienvenido al juego Guerra de Manchas (Blob Wars)=====\n"
						"\t1. Juego de dos jugadores\n"
						"\t2. Juego contra computadora\n"
        				"\t3. Recuperar un juego guardado\n"
           				"\t4. Terminar\n\n"
        				"Elegir opción: ");
				do {
				aux=getint("");
				CLEAR_SCREEN;
				} while(!(aux >= 1 && aux <= 4));

				if(aux == 1)
					state = GAME;
				else if(aux == 2){
					state = GAME;
					vsAI = TRUE;
				}
				else if(aux == 3)
					state = LOAD;
				else if(aux == 4)
					state = QUIT;
				else
					printf("Opción invalida\n");

				printf("STATE == %d\n",state);

				break;

			case GAME:
				//Generate board if not loaded (wip) or ¿loaded ended?
				render(&board, blobCount, player);
				if(canMove(player, &board)) {
					if(vsAI && player == AIPLAYER) {
						getAImove(&command, &board);
						printf("gotAImove\n");
					}
					else {
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
					}
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
				printf("load\n");
				getchar();
				break;

			case END:
				winner = endGame(&board, blobCount);
				render(&board, blobCount, 0);
				printf("Player %d won!\nPress enter to go back to menu\n", winner);
				while(getchar() != '\n');
				state = MENU;
				break;

		}
	}
	return 0;
}

void render(typeBoard* board, const int blobCount[],int player){
	int i, j, scrWidth, space;

  CLEAR_SCREEN;

	scrWidth = board->w*2 + 7;

	printf(POINTS1":%d", blobCount[1]);

	space = scrWidth - 8;
	if(blobCount[1] > 10)
		space--;
	if(blobCount[2] > 10)
		space--;

	for(i = 0; i < space; i++)
		putchar(' ');

  printf(POINTS2":%d\n\n", blobCount[2]);

  for(i = 0; i < board->h; i++){
		printf("   ");
    for(j = 0; j < board->w; j++)
    {
      switch(board->get[i][j].owner){

        case 0:
          printf("| ");
          break;

        case 1:
          printf("|"PLAYER1);
          break;

        case 2:
          printf("|"PLAYER2);
          break;
      }
    }
    printf("|\n");
  }

	putchar('\n');
	space = (scrWidth - 8)/2;
	for(i = 0; i < space; i++)
		putchar(' ');

  if(player==1){
  	printf("turno: "PLAYER1"\n\n");
  }
  else if(player == 2){
  	printf("turno: "PLAYER2"\n\n");
  }

}

int save(/*...WIP...*/) /* ERROR HANDLING: DEVUELVE SI HUBO UN PROBLEMA AL GUARDAR */
;
int load(/*...WIP...*/)  /* ERROR HANDLING: DEVUELVE SI HUBO UN PROBLEMA AL CARGAR, EJ. CORRUPTO O NO EXISTE */
;
