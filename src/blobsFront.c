#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include "getnum.h"
#include "blobsBack.h"
#ifdef _WIN32
	#define BLOB1 "A"
	#define BLOB2 "Z"
	#define POINTS1 "J1"
	#define POINTS2 "J2"
	#define PLAYER1 "Jugador1"
	#define PLAYER2 "Jugador2"
	#define CLEAR_SCREEN system("cls")
#else
	#define BLOB1 "\x1b[36;1mO\x1b[0m" //Blue
	#define BLOB2 "\x1b[31;1mO\x1b[0m" //Red
	#define POINTS1 "\x1b[36;1mJ1\x1b[0m"
	#define POINTS2 "\x1b[31;1mJ2\x1b[0m"
	#define PLAYER1 "\x1b[36;1mJugador1\x1b[0m"
	#define PLAYER2 "\x1b[31;1mJugador2\x1b[0m"
	#define CLEAR_SCREEN system("clear")
#endif

int main(int argc, char **argv) {

	int blobCount[3] = {0,2,2};
	typeBoard board;
	board.get = NULL;
	char** loadedArray;
	int dimx = 0, dimy = 0;

	gameState state = MENU;
	typeCommand command;
	int vsAI, input, winner; //winner quedo aca porque no puede estar al principio de un case
	char *filename, *retValue;
	srand(time(NULL));
	int player = rand()%2 + 1; /* Generate a number between 1 & 2 (0 is not included) */

	while(state != QUIT) {
		switch(state) {
			case MENU:

				if(board.get != NULL) {
					free(*board.get);
					free(board.get);
					board.get = NULL;
				}
				vsAI=FALSE;
				input = 0;

				CLEAR_SCREEN;
				printf("=====Bienvenido al juego Guerra de Manchas (Blob Wars)=====\n"
						"\t1. Juego de dos jugadores\n"
						"\t2. Juego contra computadora\n"
        				"\t3. Recuperar un juego guardado\n"
           				"\t4. Terminar\n");
				do {
					input = getint("\nElegir opción: ");

					if(input == 1)
						state = NEWGAME;
					else if(input == 2){
						state = NEWGAME;
						vsAI = TRUE;
					}
					else if(input == 3)
						state = LOAD;
					else if(input == 4)
						state = QUIT;
					else
						printf("Opción invalida\n");

					} while(!(input >= 1 && input <= 4));

				break;

			case NEWGAME:
				loadedArray = NULL;
				init(&board, loadedArray, dimx, dimy);
				state = GAME;
				break;

			case GAME:
				render(&board, blobCount, player);
				if(argc > 1 && argv[1][0] == 'd')
					renderMaps(&board);
				if(canMove(player, &board)) {
					if(vsAI && player == AIPLAYER) {
						getAImove(&command, &board);
						printf("gotAImove\n");
					}
					else {
						printf("Para moverse:        [f1,c1][f2,c2]\n"
									 "Para guardar:        save nombreDelArchivo\n"
									 "Para volver al menu: quit\n");
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
				save(filename, vsAI, player, &board);
				printf("\n¡Juego guardado exitosamente en %s!", filename); //SPACEHOLDER
				while(getchar() != '\n');
				if(filename[-1] == 'T')
					state = MENU;
				else
					state = GAME;
				break;

			case LOAD:
				//load()
				//init()
				printf("loaded\n");
				getchar();
				//state = GAME;
				break;

			case END:
				winner = endGame(&board, blobCount);
				render(&board, blobCount, 0);
				printf("Ganó el jugador %d!\n\nPresione enter para volver al menu\n", winner);
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
          printf("|"BLOB1);
          break;

        case 2:
          printf("|"BLOB2);
          break;
      }
    }
    printf("|\n");
  }

	putchar('\n');
	space = (scrWidth - 15)/2;
	for(i = 0; i < space; i++)
		putchar(' ');

  if(player==1){
  	printf("Turno: "PLAYER1"\n\n");
  }
  else if(player == 2){
  	printf("Turno: "PLAYER2"\n\n");
  }

}

int save(char *filename, int mode, int player, typeBoard *board){

	errno = 0;

	FILE *ptr;

    typeSave rec = {mode, player, board -> h, board -> w}; /* Falta recorrer los 2 blobs y guardarlas fichas que tiene cada uno y por último las A y Z, el load es al revés, ya lo aprendé :) */

	ptr = fopen(filename,"wb");

	if (ptr == NULL)
	{
		printf("Error al abrir el archivo.\n");
		printf("Error %d \n", errno);
		return 1;
	}

	fwrite(&rec, sizeof(typeSave), 1, ptr);

	fclose(ptr);

	return 0;
}

int load(/*...WIP...*/)  /* ERROR HANDLING: DEVUELVE SI HUBO UN PROBLEMA AL CARGAR, EJ. CORRUPTO O NO EXISTE */
;

void renderMaps(typeBoard *board) {
	int i, j;
	for (i = 0; i < board->h; i++) {
    for (j = 0; j < board->w; j++) {
			printf("|%d",board->get[i][j].canMove);
		}
		printf("|\n");
	}
	for (i = 0; i < board->h; i++) {
    for (j = 0; j < board->w; j++) {
			printf("|%d",board->get[i][j].canEat);
		}
		printf("|\n");
	}
}
