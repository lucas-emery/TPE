#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "blobsBack.h"
#ifdef _WIN32
	#define BLOB1 "A"
	#define BLOB2 "Z"
	#define POINTS1 "J1"
	#define POINTS2 "J2"
	#define PLAYER1 "Jugador1"
	#define PLAYER2 "Jugador2"
#else
	#define BLOB1 "\x1b[36;1mO\x1b[0m" //Blue
	#define BLOB2 "\x1b[31;1mO\x1b[0m" //Red
	#define POINTS1 "\x1b[36;1mJ1\x1b[0m"
	#define POINTS2 "\x1b[31;1mJ2\x1b[0m"
	#define PLAYER1 "\x1b[36;1mJugador1\x1b[0m"
	#define PLAYER2 "\x1b[31;1mJugador2\x1b[0m"
#endif

int main() {

	int blobCount[3];
	typeBoard board;
	board.get = NULL;

	gameState state = MENU;
	typeCommand command;
	int vsAI, input, player, winner;
	char *filename, *retValue, *loadedArray = NULL;
	srand(time(NULL));

	while(state != QUIT) {
		switch(state) {
			case MENU:

				if(board.get != NULL) {    // Cada vez que entra a MENU, limpia el tablero.
					free(*board.get);
					free(board.get);
					board.get = NULL;
				}
				vsAI = FALSE;              // Y las variables
				input = 0;

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
				player = rand()%2 + 1;
				blobCount[1] = 2;           // blobCount contiene los puntajes de los jugadores
				blobCount[2] = 2;
				if(init(&board, NULL))      // Si logra inicializar el tablero, comienza el juego
					state = GAME;           
				else {						// De no ser asi, vuelve a MENU (donde se limpia el tablero)
					printf("\nPresione enter para volver al menu\n");
					while(getchar() != '\n');
					state = MENU;
				}
				break;

			case GAME:
				render(&board, blobCount, player);
				if(canMove(player, &board)) {				// Analiza si hay jugadas disponibles para el jugador. Si no las hay cambia de estado a END
					if(vsAI && player == AIPLAYER) {  		   
						getAImove(&command, &board);		
					}
					else {
						printf("Para moverse:        [f1,c1][f2,c2]\n"
									 "Para guardar:        save nombreDelArchivo\n"
									 "Para volver al menu: quit\n");
						do {
							if(getCommand(&command, &retValue)) {  // 
								if(retValue != NULL) {   // retValue vale NULL cuando el usuario ingresa un comando
									if(*retValue == EOF) // El usuario ingreso quit
										state = MENU;
									else {				// El usuario ingreso save
										state = SAVE;
										filename = retValue;
									}
								}
							}
							else {
								printf("\nPresione enter para volver al menu\n");
								while(getchar() != '\n');
								state = MENU;
								break;
							}
						} while(state == GAME && !validCommand(player, &command, &board));
					}
					if(state == GAME) {
						blobCount[player] += move(player, &command, &board); //move() devuelve 1 si se divide o 0 si salta
						conquer(player, &command, &board, blobCount);		 //Captura las manchas enemigas
						if(player == 1)
							player = 2;
						else
							player = 1;
					}
				}
				else
					state = END;							// Termino el juego
				break;

			case SAVE:
				if(save(filename, vsAI, player, blobCount, &board))
					printf("\n¡Juego guardado exitosamente en %s!\n", filename);
				printf("\nPresione enter para continuar\n");
				while(getchar() != '\n');
				if(filename[-1] == 'T')
					state = MENU;
				else
					state = GAME;
				free(filename - 1);
				break;

			case LOAD:
				if(getFilename(&filename) && load(filename, &vsAI, &player, blobCount, &board, &loadedArray) && init(&board, loadedArray))
					state = GAME;
				else {
					printf("\nPresione enter para volver al menu\n");
					while(getchar() != '\n');
					state = MENU;
				}
				break;

			case END:
				winner = endGame(&board, blobCount); // Completa el tablero con los movimientos disponibles y devuelve el ganador
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
														//	 ┌> blobCount[1]    ┌> blobCount[2]		
	scrWidth = board->w*2 + 7;                          //J1:2|<---space--->|J2:2//
														// 						 //
	printf(POINTS1":%d", blobCount[1]);					//	  |A| | | | | |Z|	 //
														//	  | | | | | | | |	 //
	space = scrWidth - 8;								//	  | | | | | | | |	 //
	if(blobCount[1] > 10)								//	  | | | | | | | |	 //
		space--;										//	  |A| | | | | |Z|	 //
	if(blobCount[2] > 10)								//						 //
		space--;										//|<┬>|Turno:Jugador 1	 //
														//  └> space			 // 
	for(i = 0; i < space; i++)							//						 //
		putchar(' ');									//|<------scrWidth----->|//


  printf(POINTS2":%d\n\n", blobCount[2]);

  for(i = 0; i < board->h; i++){
		printf("   ");
    for(j = 0; j < board->w; j++)
    {
      switch(board->get[i][j].owner){  //owner == 	0:Vacio 1:Mancha Jugador1 2:Mancha Jugador2

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
	space = (scrWidth - 15)/2; //se redefine space
	for(i = 0; i < space; i++)
		putchar(' ');

  if(player==1){
  	printf("Turno: "PLAYER1"\n\n");
  }
  else if(player == 2){
  	printf("Turno: "PLAYER2"\n\n");
  }

}
