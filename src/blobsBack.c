#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "blobsBack.h"

typedef enum {CMD_START, CMD_MOVE, CMD_SAVE, CMD_QUIT, CMD_RESET} getCmdState;
typedef enum {EAT, MOVE} mapType;

int init(typeBoard *board, char *loadedArray) {

  typeBlob *temp;
  int i,j,k,l,minX,minY,maxX,maxY;

  if(loadedArray == NULL) {
    do {
      board->h = getint("\nIngrese la cantidad de filas(Entre 5 y 30): ");
    } while(board->h < 5 || board->h > 30);
    do {
      board->w = getint("\nIngrese la cantidad de columnas(Entre 5 y 30): ");
    } while(board->w < 5 || board->w > 30);
  }

  if((board->get = (typeBlob**) malloc(board->h * sizeof(typeBlob*))) == NULL){
    printf("No hay suficiente espacio en el Heap\n");
    return FALSE;
  }

  if((temp = (typeBlob*) malloc(board->h * board->w * sizeof(typeBlob))) == NULL)
  {
    printf("No hay suficiente espacio en el Heap\n");
    free(board->get);
    return FALSE;
  }

  for (i = 0; i < board->h; i++) {
    board->get[i] = temp + (i * board->w);
  }


  if(loadedArray == NULL) {
    fill(board);
  }
  else {
    for(i = 0 ; i < board->h ; i++) {
      for(j = 0 ; j < board->w ; j++) {
        switch(loadedArray[(i*board->w)+j]) {
          case '0':
            board->get[i][j].owner = 0;
            break;

          case 'A':
            board->get[i][j].owner = 1;
            break;

          case 'Z':
            board->get[i][j].owner = 2;
            break;

          default:
            printf("El archivo cargado esta corrupto, no se puede generar el tablero\n");
            free(loadedArray);
            return FALSE;
            break;
        }
      }
    }
    free(loadedArray);
  }


  for(i = 0 ; i < board->h ; i++){
    for(j = 0 ; j < board->w ; j++){
      {
        minY=i-2;
        minX=j-2;
        maxY=i+2;
        maxX=j+2;

        if( minY < 0 )
          minY = 0;

        if( maxY > board->h-1 )
          maxY = board->h-1;

        if( minX < 0)
          minX = 0;

        if( maxX > board->w-1 )
          maxX = board->w-1;
        /*printf("=i:%d=j:%d=mY:%d=MY:%d=mX:%d=MX:%d=\n",i,j,minY,maxY,minX,maxX);*/
        board->get[i][j].canMove = 0;
        board->get[i][j].canEat = 0;

        for(k = minY; k <= maxY; k++ ){
          for(l = minX; l <= maxX; l++){

            if(board->get[k][l].owner == 0){
              board->get[i][j].canMove++;
            }
            else if((board->get[k][l].owner == 1) && (abs(i-k) < 2 && abs(j-l) < 2) )
              board->get[i][j].canEat++;
          }
        }

        if(board->get[i][j].owner == 0)
        board->get[i][j].canMove--;
        if(board->get[i][j].owner == 1)
        board->get[i][j].canEat--;
      }
    }

  }
  return TRUE;
}

void fill(typeBoard * board){ //prueba para el switch
 int i,j;
 for( i = 0 ; i < board->h; i++){
  for( j = 0 ; j < board->w ; j++){
    if ( j == 0 && (i == 0 || i == board->h-1)){
      board->get[i][j].owner = 1;
    }
    else if ( j == board->w-1 && (i == 0 || i == board->h-1)){
      board->get[i][j].owner = 2;
    }
    else
      board->get[i][j].owner = 0;
    }
  }
}

int canMove(int player, typeBoard *board) {
  int i, j;
  for(i = 0; i < board->h; i++) {
    for(j = 0; j < board->w; j++) {
      if(board->get[i][j].owner == player && board->get[i][j].canMove) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

char* getCommand(typeCommand *command) {
  char input;
  const char *pattern;
  char *output;
  getCmdState state;
  int valid = FALSE, i, length;

  while(!valid) {
    output = NULL;
    state = CMD_START;
    i = 0;
    length = 0;

    printf("\nIngrese un comando: ");

    while((input = getchar()) != '\n') {
      switch(state) {
        case CMD_START:
          if(input == '[') {
            state = CMD_MOVE;
            pattern = "[#*,#*][#*,#*]";
          }
          else if(input == 's') {
            state = CMD_SAVE;
            pattern = "save F";
          }
          else if(input == 'q') {
            state = CMD_QUIT;
            pattern = "quit";
          }
          else
            state = CMD_RESET; //invalid command
          i++;
          break;

        case CMD_MOVE:
          if(pattern[i] == '*') {
            i++;
            if(input >= '0' && input <= '9') {
              if(i == 3) command->source.y = command->source.y*10 + (input-'0');
              else if(i == 6) command->source.x = command->source.x*10 + (input-'0');
              else if(i == 10) command->target.y = command->target.y*10 + (input-'0');
              else if(i == 13) command->target.x = command->target.x*10 + (input-'0');
              break; //TODO
            }
          }
          if(pattern[i] == '#') {
            if(input >= '0' && input <= '9') {
              if(i == 1) command->source.y = input-'0';
              else if(i == 4) command->source.x = input-'0';
              else if(i == 8) command->target.y = input-'0';
              else if(i == 11) command->target.x = input-'0';
              i++;
            }
            else
              state = CMD_RESET;
          }
          else if(input == pattern[i]) {
            i++;
            if(pattern[i] == '\0')
              valid = TRUE;
          }
          else {
            state = CMD_RESET;
            valid = FALSE;
          }
          break;

        case CMD_SAVE:
          if(pattern[i] == 'F') {
            if(length < 15) {
              if(output == NULL) {
                output = (char*) malloc(17*sizeof(char)); //Extra space for \0 && not quit command
                output[0] = 'F'; //False
                output++;
              }
              output[length] = input;
              length++;
              valid = TRUE;
            }
            else {
              valid = FALSE;
              printf("La máxima longitud para el nombre del archivo es 15 - ");
              state = CMD_RESET;
            }
          }
          else if(input == pattern[i])
            i++;
          else
            state = CMD_RESET;
          break;

        case CMD_QUIT:
          if(input == pattern[i]) {
            i++;
            if(pattern[i] == '\0') {
              output = (char*) malloc(sizeof(char));
              *output = EOF;
              valid = TRUE;
            }
          }
          else {
            valid = FALSE;
            state = CMD_RESET;
          }
          break;

        case CMD_RESET:
          //EMPTIES BUFFER
          break;
      }
    }

    if(!valid) {
      printf("Comando Inválido\n");
      free(output);
    }
  }

  if(output != NULL && *output == EOF) {
    printf("¿Desea guardar antes de salir?(s/n): ");
    valid = FALSE;
    while(!valid) {
      input = getchar();
      if(getchar() == '\n') {
        if(input == 's') {
          output = (char*) realloc(output, 17*sizeof(char)); //Extra space for \0 && quit command
          output[0] = 'T'; //True
          output++;
          while(!valid) {
            printf("Ingrese un nombre para el archivo (máximo 15 caracteres): ");
            length = 0;
            while((input = getchar()) != '\n' && length < 15) {
              output[length] = input;
              length++;
            }
            if(input == '\n')
              valid = TRUE;
            else {
              printf("¡El nombre es demasiado largo!\n");
              while(getchar() != '\n'); //EMPTY BUFFER
            }
          }
        }
        else if(input == 'n')
          valid = TRUE;
      }
      else
        while(getchar() != '\n');//EMPTY BUFFER

      if(!valid)
        printf("Respuesta inválida, ingrese s para sí o n para no: ");
    }
  }

  command->source.x--; //Human to computer correction
  command->source.y--;
  command->target.x--;
  command->target.y--;

  if(output != NULL && *output != EOF)
    output[length] = '\0';

  return output;
}

int isInside(int x, int y, int w, int h) {
  if(x >= 0 && x < w && y >= 0 && y < h)
    return TRUE;
  else
    return FALSE;
}

int validCommand(int player, typeCommand *command, typeBoard *board) {
  if(!isInside(command->source.x, command->source.y, board->w, board->h)) {
    printf("Comando inválido, [%d,%d] no existe!\n", command->source.y+1, command->source.x+1);
    return FALSE;
  }
  else if(!isInside(command->target.x, command->target.y, board->w, board->h)) {
    printf("Comando inválido, [%d,%d] no existe!\n", command->target.y+1, command->target.x+1);
    return FALSE;
  }
  else if(board->get[command->source.y][command->source.x].owner != player) {
    printf("Comando inválido, [%d,%d] no es tuyo!\n", command->source.y+1, command->source.x+1);
    return FALSE;
  }
  else if(abs(command->source.x - command->target.x) > 2 || abs(command->source.y - command->target.y) > 2) {
    printf("Comando inválido, [%d,%d] no se puede mover tan lejos!\n", command->source.y+1, command->source.x+1);
    return FALSE;
  }
  else if(board->get[command->target.y][command->target.x].owner != 0) {
    printf("Comando inválido, [%d,%d] no está vacio!\n", command->target.y+1, command->target.x+1);
    return FALSE;
  }
  else
    return TRUE;
}

//Action must be INCREMENT or DECREMENT
void updateMap(typeCoord center, typeBoard *board, mapType type, int action) {
  int minX, maxX, minY, maxY, radius;
  size_t offset;

  if(type == EAT) {
    radius = 1;
    offset = offsetof(typeBlob, canEat);
  }
  else {
    radius = 2;
    offset = offsetof(typeBlob, canMove);
  }

  minX = center.x - radius;
  maxX = center.x + radius;
  minY = center.y - radius;
  maxY = center.y + radius;

  if(minX < 0)
    minX = 0;
  else if(maxX >= board->w)
    maxX = board->w - 1;
  if(minY < 0)
    minY = 0;
  else if(maxY >= board->h)
    maxY = board->h - 1;

  int i, j;
  for(i = minY; i <= maxY; i++) {
    for(j = minX; j <= maxX; j++) {
      *((int*)(((size_t)&board->get[i][j])+offset)) += action; //Equals typeBlob.canEat or typeBlob.canMove
    }
  }

  //Correct outside loop to halve comparisons
  *((int*)(((size_t)&board->get[center.y][center.x])+offset)) -= action; //Equals typeBlob.canEat or typeBlob.canMove
}

int move(int player, typeCommand *command, typeBoard *board) {
  board->get[command->target.y][command->target.x].owner = player;
  updateMap(command->target, board, MOVE, DECREMENT);
  if(player != AIPLAYER)
    updateMap(command->target, board, EAT, INCREMENT);
  if(abs(command->source.x - command->target.x) == 2 || abs(command->source.y - command->target.y) == 2) {
    board->get[command->source.y][command->source.x].owner = 0;
    updateMap(command->source, board, MOVE, INCREMENT);
    if(player != AIPLAYER)
      updateMap(command->source, board, EAT, DECREMENT);
    return FALSE;
  }
  return TRUE;
}

void conquer(int player, typeCommand *command, typeBoard *board, int blobCount[]) {
  int otherPlayer, minX, maxX, minY, maxY;
  typeCoord coord;
  if(player == 1)
    otherPlayer = 2;
  else
    otherPlayer = 1;

  minX = command->target.x-1;
  maxX = command->target.x+1;
  minY = command->target.y-1;
  maxY = command->target.y+1;

  if(minX < 0)
    minX = 0;
  else if(maxX >= board->w)
    maxX = board->w - 1;
  if(minY < 0)
    minY = 0;
  else if(maxY >= board->h)
    maxY = board->h - 1;

  int i, j, action;
  for(i = minY; i <= maxY; i++) {
    for(j = minX; j <= maxX; j++) {
      if(board->get[i][j].owner == otherPlayer) {
        board->get[i][j].owner = player;
        blobCount[player]++;
        blobCount[otherPlayer]--;

        if(player == AIPLAYER)
          action = DECREMENT;
        else
          action = INCREMENT;

        coord.y = i;
        coord.x = j;
        updateMap(coord, board, EAT, action);
      }
    }
  }
}

void getAImove(typeCommand *command, typeBoard *board) {
  int i, j, k, l, minX, maxX, minY, maxY, override, sameScoreMoves;
  typeCommand bestMove;
  int bestScore = -1, isBMmitosis = 0, isMitosis;

  for(i = 0; i < board->h; i++) {
    for(j = 0; j < board->w; j++) {
      if(board->get[i][j].owner == AIPLAYER && board->get[i][j].canMove) {
        minY = i - 2;
        maxY = i + 2;
        minX = j - 2;
        maxX = j + 2;

        if(minY < 0)
          minY = 0;
        else if(maxY >= board->h)
          maxY = board->h - 1;
        if(minX < 0)
          minX = 0;
        else if(maxX >= board->w)
          maxX = board->w - 1;

        for(k = minY; k <= maxY; k++) {
          for(l = minX; l <= maxX; l++) {
            if(board->get[k][l].owner == 0) {
              override = FALSE;
              isMitosis = FALSE;
              if(board->get[k][l].canEat > bestScore) {
                override = TRUE;
                sameScoreMoves = 1;
                if(abs(i - k) == 1 && abs(j - l) == 1)
                  isMitosis = TRUE;
              }
              else if(board->get[k][l].canEat == bestScore) {
                if(abs(i - k) == 1 && abs(j - l) == 1)
                  isMitosis = TRUE;
                if(!isBMmitosis && isMitosis) {
                  override = TRUE;
                  sameScoreMoves = 1;
                }
                else if(isMitosis == isBMmitosis) {
                  sameScoreMoves++;
                  if(rand()%sameScoreMoves == 0)
                    override = TRUE;
                }
              }

              if(override) {
                bestMove.source.y = i;
                bestMove.source.x = j;
                bestMove.target.y = k;
                bestMove.target.x = l;
                bestScore = board->get[k][l].canEat;
                isBMmitosis = isMitosis;
              }
            }
          }
        }
      }
    }
  }
  *command = bestMove;
}

int endGame(typeBoard *board, int blobCount[]) {
  int winner;
  if(blobCount[1] > blobCount[2])
    winner = 1;
  else
    winner = 2;

  int i, j;
  for(i = 0; i < board->h; i++) {
    for(j = 0; j < board->w; j++) {
      if(board->get[i][j].owner == 0) {
        board->get[i][j].owner = winner;
        blobCount[winner]++;
      }
    }
  }
  return winner;
}

char* getFilename() {
  char *filename = malloc(16 * sizeof(char));
  int length, valid = FALSE;
  char input;

  while(!valid) {
    printf("Ingrese el nombre del archivo (máximo 15 caracteres): ");
    length = 0;
    while((input = getchar()) != '\n' && length < 15) {
      filename[length] = input;
      length++;
    }
    if(input == '\n')
      valid = TRUE;
    else {
      printf("¡El nombre es demasiado largo!\n\n");
      while(getchar() != '\n'); //EMPTY BUFFER
    }
  }
  filename[length] = '\0';

  return filename;
}

int save(char *filename, int vsAI, int player, int blobCount[], typeBoard *board){
  int i, j, result = FALSE;
	FILE *file;
  char *boardBuffer;
  int dataBuffer[6] = {vsAI, player, board->h, board->w, blobCount[1], blobCount[2]};

  if((boardBuffer = (char*) malloc(board->w * board->h * sizeof(char))) == NULL) {
    printf("No hay suficiente espacio en el heap para realizar esta operación\n");
  }
  else {
    if ((file = fopen(filename,"wb")) == NULL) {
  		perror("Error al intentar crear el archivo");
  	}
    else {
  	  if(fwrite(dataBuffer, sizeof(int), 6, file) < 6) {
        printf("Error al intentar guardar los datos\n");
      }
      else {
        for(i = 0; i < board->h; i++) {
          for(j = 0; j < board->w; j++) {
            switch(board->get[i][j].owner) {
              case 0:
                boardBuffer[(i*board->w)+j] = '0';
                break;
              case 1:
                boardBuffer[(i*board->w)+j] = 'A';
                break;
              case 2:
                boardBuffer[(i*board->w)+j] = 'Z';
                break;
            }
          }
        }
        if(fwrite(boardBuffer, sizeof(char), board->w * board->h, file) < (board->w * board->h)) {
          printf("Error al intentar guardar el tablero\n");
        }
        else {
          result = TRUE;
        }
      }
      if(fclose(file) == EOF)
  			printf("Error al intentar cerrar el archivo\n");
    }
    free(boardBuffer);
  }
	return result;
}

int load(char *filename, int *vsAI, int *player, int blobCount[], typeBoard *board, char **loadedArray) {
	int result = FALSE;
	FILE *file;

	if((file = fopen(filename, "rb")) == NULL) {
		perror("Error al intentar abrir el archivo");
	}
	else {
		if(fread(vsAI, sizeof(int), 1, file) == 0)
			printf("Error al intentar cargar el modo de juego\n");
		else {
			if(fread(player, sizeof(int), 1, file) == 0)
				printf("Error al intentar cargar el turno\n");
			else {
				if(fread(&board->h, sizeof(int), 1, file) == 0)
					printf("Error al intentar cargar el alto del tablero\n");
				else {
					if(fread(&board->w, sizeof(int), 1, file) == 0)
						printf("Error al intentar cargar el ancho del tablero\n");
					else {
						if(fread(&blobCount[1], sizeof(int), 2, file) < 2)
							printf("Error al intentar cargar los puntajes\n");
						else {
							if((*loadedArray = (char*) malloc(board->h * board->w * sizeof(char))) == NULL)
						    printf("No hay suficiente espacio en el Heap para cargar el tablero\n");
							else {
									if(fread(*loadedArray, sizeof(char), board->h * board->w, file) < (board->h*board->w))
										printf("Error al intentar cargar el tablero\n");
									else
										result = TRUE;
							}
						}
					}
				}
			}
		}
		if(fclose(file) == EOF)
			printf("Error al intentar cerrar el archivo\n");
	}
  free(filename);
	return result;
}

int getint(char *message) {
  char input, output, valid = FALSE;
  while(!valid) {
    output = 0;
    printf(message);
    while((input = getchar()) != '\n') {
      if(input >= '0' && input <= '9') {
          output = (10*output) + (input - '0');
          valid = TRUE;
      }
      else {
        valid = FALSE;
        while(getchar() != '\n'); //EMPTY BUFFER
        break;
      }
    }
  }
  return output;
}
