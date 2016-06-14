#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "blobsBack.h"

typedef enum {CMD_START, CMD_MOVE, CMD_SAVE, CMD_QUIT, CMD_RESET} getCmdState;
typedef enum {EAT, MOVE} mapType;


void generate(typeBoard * board){              // Genera el tablero estándar
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

void initMaps(typeBoard *board){                               // initMaps crea el mapa canMove y el mapa canEat para despues analizar 
                                                               // la cantidad de movimientos restantes y los mejores movimientos para
  int i, j, k, l, minX, minY, maxX, maxY;                      // la computadora
                                                               // minX, minY, maxX, maxY van a limitarse a los bordes del tablero
  for(i = 0 ; i < board->h ; i++){                             
    for(j = 0 ; j < board->w ; j++){                           
      
        minY=i-2;                   // Se definen los limites en un radio de 2 celdas al rededor del centro
        minX=j-2;
        maxY=i+2;
        maxX=j+2;

        if( minY < 0 )             // Si estas variables se encuentran afuera de las dimensiones del tablero, se ajustan acorde a lo necesario
          minY = 0;

        if( maxY > board->h-1 )
          maxY = board->h-1;

        if( minX < 0)
          minX = 0;

        if( maxX > board->w-1 )
          maxX = board->w-1;

        board->get[i][j].canMove = 0;   
        board->get[i][j].canEat = 0;    

        for(k = minY; k <= maxY; k++ ){  
          for(l = minX; l <= maxX; l++){

            if(board->get[k][l].owner == 0){                                             // Se calcula canMove en un radio de 2 celdas
              board->get[i][j].canMove++;
            }
            else if((board->get[k][l].owner == 1) && (abs(i-k) < 2 && abs(j-l) < 2) )   // Se calcula canEat en un radio de 1 celda
              board->get[i][j].canEat++;                                              
          }
        }

        if(board->get[i][j].owner == 0)  // Se le resta 1 al valor de canMove porque se cuenta 1 de más
        board->get[i][j].canMove--;
        if(board->get[i][j].owner == 1)  // Se le resta 1 al valor de canEat porque se cuenta 1 de más
        board->get[i][j].canEat--;
      }
    }

  
  //   Mapa            //   Mapa canMove           //   Mapa canEat     //
  //                   //                          //                   //
  //  |A| | | | | |Z|  //  | 8|10|13|14|13|10| 8|  //  |0|1|0|0|0|0|0|  //
  //  | | | | | | | |  //  |10|14|18|19|18|14|10|  //  |1|1|0|0|0|0|0|  //
  //  | | | | | | | |  //  |12|17|22|24|22|17|12|  //  |0|0|0|0|0|0|0|  //
  //  | | | | | | | |  //  |10|14|18|19|18|14|10|  //  |1|1|0|0|0|0|0|  //
  //  |A| | | | | |Z|  //  | 8|10|13|14|13|10| 8|  //  |0|1|0|0|0|0|0|  //
  //                   //                          //                   //
 
}

int init(typeBoard *board, char *loadedArray) {                                 // Retorna falso si se produce un error

  typeBlob *temp;
  int i, j;

  if(loadedArray == NULL) {
    do {
      board->h = getInt("\nIngrese la cantidad de filas(Entre 5 y 30): ");     
    } while(board->h < 5 || board->h > 30);
    do {
      board->w = getInt("\nIngrese la cantidad de columnas(Entre 5 y 30): ");
    } while(board->w < 5 || board->w > 30);
  }

  if((board->get = (typeBlob**) malloc(board->h * sizeof(typeBlob*))) == NULL){  // Crea un vector dinamico con la dimension de la altura
    printf("No hay suficiente espacio en el Heap\n");
    return FALSE;
  }

  if((temp = (typeBlob*) malloc(board->h * board->w * sizeof(typeBlob))) == NULL) // Crea un bloque en el heap del tamaño del tablero
  {
    printf("No hay suficiente espacio en el Heap\n");
    free(board->get);
    return FALSE;
  }

  for (i = 0; i < board->h; i++) {    // Divide el bloque en filas y guarda su dirección en la posición correspondiente del primer vector
      board->get[i] = temp + (i * board->w);
    }


  if(loadedArray == NULL) {           // En caso de que no se haya cargado ningun tablero, genera el tablero estándar
    generate(board);
  }
  else {                              // El otro caso es que se cargó un tablero de tipo char y hay que guardarlo al tipo typeBoard
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
  
  initMaps(board);                   //  Calcula el mapa de movimientos y el mapa canEat

  return TRUE;
}

int canMove(int player, typeBoard *board) { //Busca si alguno de los blobs del jugador se puede mover
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

int getCommand(typeCommand *command, char **output) { //Retorna verdadero si no hubo ningun error de memoria
  char input;                                         //Si el jugador introdujo un movimiento, lo retorna por command
  const char *pattern;                                //Si decidio guardar devuelve el nombre del archivo por output
  getCmdState state;                                  //y un caracter 'F' escondido antes del nombre del archivo que
  int valid = FALSE, i, length;                       //indica que debe continuar la partida
                                                      //Si decidio guardar y salir devuelve el nombre del archivo por
  while(!valid) {                                     //output y 'T' escondido antes del nombre del archivo
    *output = NULL;
    state = CMD_START;
    i = 0;
    length = 0;

    printf("\nIngrese un comando: ");

    while((input = getchar()) != '\n') {
      switch(state) {
        case CMD_START:
          if(input == '[') {
            state = CMD_MOVE;
            pattern = "[#*,#*][#*,#*]";		//En este patron, # es un numero obligatorio y * un nro opcional
          }
          else if(input == 's') {
            state = CMD_SAVE;
            pattern = "save F";				//En este patron, F indica que lo que sigue es el filename(nombre del archivo)
          }
          else if(input == 'q') {
            state = CMD_QUIT;
            pattern = "quit";
          }
          else
            state = CMD_RESET;		//Indica que el comando ingresado es invalido
          i++;
          break;

        case CMD_MOVE:
          if(pattern[i] == '*') {	//Significa que este caracter puede ser un nro o debe ser evaluado contra el patron
            i++;
            if(input >= '0' && input <= '9') {
              if(i == 3) command->source.y = command->source.y*10 + (input-'0');
              else if(i == 6) command->source.x = command->source.x*10 + (input-'0');
              else if(i == 10) command->target.y = command->target.y*10 + (input-'0');
              else if(i == 13) command->target.x = command->target.x*10 + (input-'0');
              break;	//Pasa al siguiente caracter porque lo que se ingreso es un nro de dos digitos
            }
          }
          if(pattern[i] == '#') { //Significa que este caracter tiene que ser un nro
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
          else if(input == pattern[i]) { //Si el comando es mas largo que el patron, chequea contra NULL y por ende es invalido
            i++;
            if(pattern[i] == '\0') //Si se llegó al final del patron, el comando es valido
              valid = TRUE;
          }
          else {
            state = CMD_RESET;
            valid = FALSE;
          }
          break;

        case CMD_SAVE:
          if(pattern[i] == 'F') {
            if(length < 100) { //Largo máximo arbitrario
              if(*output == NULL) {
                if((*output = (char*) malloc(102*sizeof(char))) == NULL) { //2 espacios extra, uno para el final del string y
				  printf("No hay suficiente espacio en el Heap\n");      //otro para el caracter escondido
				  return FALSE;
				}
                **output = 'F'; //Flag para NO salir despues de guardar
                (*output)++;
              }
              (*output)[length] = input;
              length++;
              valid = TRUE;
            }
            else {
              valid = FALSE;
              printf("La máxima longitud para el nombre del archivo es 100 - ");
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
            if(pattern[i] == '\0') { //Cuando llego al final del patron, el comando es valido
              if((*output = (char*) malloc(sizeof(char))) == NULL) {
				printf("No hay suficiente espacio en el Heap\n");
				return FALSE;
			  }
              **output = EOF;		//EOF se utiliza para señalizar que el usuario quiere salir
              valid = TRUE;
            }
          }
          else {
            valid = FALSE;
            state = CMD_RESET;
          }
          break;

        case CMD_RESET:
          //Vacia el buffer
          break;
      }
    }

    if(!valid) {
      printf("Comando Inválido\n");
	  if(*output != NULL) {
		if(**output == EOF)
		  free(*output);
		else
		  free((*output)-1); //Si estaba tratando de guardar output tiene el caracter escondido
	  }
    }
  }

  if(*output != NULL && **output == EOF) { //Si el usuario quiere salir
    printf("¿Desea guardar antes de salir?(s/n): ");
    valid = FALSE;
    while(!valid) {
      input = getchar();
      if(getchar() == '\n') { //si ingresó solo un caracter
        if(input == 's') {
          char *aux = (char*) realloc(*output, 102*sizeof(char)); //2 espacios extra, uno para el final del string y
		  if(aux == NULL) {                                       //otro para el caracter escondido
			printf("No hay suficiente espacio en el Heap\n");
			free(*output);
			return FALSE;
		  }
		  else
			*output = aux;
          **output = 'T'; //Flag para salir despues de guardar
          (*output)++;
          while(!valid) {
            printf("Ingrese un nombre para el archivo (máximo 100 caracteres): ");
            length = 0;
            while((input = getchar()) != '\n' && length < 100) {
              (*output)[length] = input;
              length++;
            }
            if(input == '\n')	//Si el input quedo en \n significa que entraron 100 o menos caracteres
              valid = TRUE;
            else {
              printf("¡El nombre es demasiado largo!\n");
              while(getchar() != '\n'); //Vacia el buffer
            }
          }
        }
        else if(input == 'n')
          valid = TRUE;
      }
      else
        while(getchar() != '\n');//Vacia el buffer

      if(!valid)
        printf("Respuesta inválida, ingrese s para sí o n para no: ");
    }
  }

  if(*output != NULL && **output != EOF)
    (*output)[length] = '\0';	//Cierra el string

  return TRUE;
}

int isInside(int x, int y, int w, int h) {
  if(x >= 0 && x < w && y >= 0 && y < h)
    return TRUE;
  else
    return FALSE;
}

int validCommand(int player, typeCommand *command, typeBoard *board) {
  if(!isInside(command->source.x, command->source.y, board->w, board->h)) {
    printf("Comando inválido, [%d,%d] no existe!\n", command->source.y, command->source.x);
    return FALSE;
  }
  else if(!isInside(command->target.x, command->target.y, board->w, board->h)) {
    printf("Comando inválido, [%d,%d] no existe!\n", command->target.y, command->target.x);
    return FALSE;
  }
  else if(board->get[command->source.y][command->source.x].owner != player) {
    printf("Comando inválido, [%d,%d] no es tuyo!\n", command->source.y, command->source.x);
    return FALSE;
  }
  else if(abs(command->source.x - command->target.x) > 2 || abs(command->source.y - command->target.y) > 2) {
    printf("Comando inválido, [%d,%d] no se puede mover tan lejos!\n", command->source.y, command->source.x);
    return FALSE;
  }
  else if(board->get[command->target.y][command->target.x].owner != 0) {
    printf("Comando inválido, [%d,%d] no está vacio!\n", command->target.y, command->target.x);
    return FALSE;
  }
  else
    return TRUE;
}

void updateMap(typeCoord center, typeBoard *board, mapType type, int action) {	//Realiza la accion especificada a todos los blobs
  int minX, maxX, minY, maxY, radius;                                           //alrededor del centro en el radio correspondiente
  size_t offset;                                                                //al mapa que se esta actualizando

  if(type == EAT) {                                                             //Estos mapas reducen significativamente el tiempo de 
    radius = 1;                                                                 //cálculo de la IA
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
      *((int*)(((size_t)&board->get[i][j])+offset)) += action; //Equivale a board->get[i][j].canEat o board->get[i][j].canMove
    }
  }

  *((int*)(((size_t)&board->get[center.y][center.x])+offset)) -= action; //Se corrige afuera del loop para reducir las comparaciones
}

int move(int player, typeCommand *command, typeBoard *board) {		//Retorna verdadero si el blob se dividio
  board->get[command->target.y][command->target.x].owner = player;	//el espacio al que se mueve ahora es del jugador
  updateMap(command->target, board, MOVE, DECREMENT);		//actualiza el mapa de movimiento, indicando que este espacio ya no esta disponible
  if(player != AIPLAYER)
    updateMap(command->target, board, EAT, INCREMENT);		//Si es enemigo de la IA, indica a los espacios adyacentes que pueden comer uno más
  if(abs(command->source.x - command->target.x) == 2 || abs(command->source.y - command->target.y) == 2) {
    board->get[command->source.y][command->source.x].owner = 0;		//Si el movimiento es un salto, libera la celda inicial
    updateMap(command->source, board, MOVE, INCREMENT);				//Y avisa que esta disponible
    if(player != AIPLAYER)
      updateMap(command->source, board, EAT, DECREMENT);		//Si es enemigo de la IA, avisa que ya no esta ahi en el eatMap
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
      if(board->get[i][j].owner == otherPlayer) {	//Si las piezas que estan alrededor son del otro jugador
        board->get[i][j].owner = player;			//Las convierte
        blobCount[player]++;						//Y corrige los puntajes de los jugadores
        blobCount[otherPlayer]--;

        if(player == AIPLAYER)
          action = DECREMENT;
        else
          action = INCREMENT;

        coord.y = i;
        coord.x = j;
        updateMap(coord, board, EAT, action);		//Corrige el eatMap dependiendo de si comió la IA o el jugador
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
      if(board->get[i][j].owner == AIPLAYER && board->get[i][j].canMove) {		//Recorre el tablero buscando blobs de la IA que se puedan mover
        minY = i - 2;
        maxY = i + 2;
        minX = j - 2;
        maxX = j + 2;

        if(minY < 0)			//Ajusta los min y los max para que no salgan del tablero
          minY = 0;
        else if(maxY >= board->h)
          maxY = board->h - 1;
        if(minX < 0)
          minX = 0;
        else if(maxX >= board->w)
          maxX = board->w - 1;

        for(k = minY; k <= maxY; k++) {
          for(l = minX; l <= maxX; l++) {
            if(board->get[k][l].owner == 0) {		//Busca posiciones vacias a las que puede moverse en un radio de 2
              override = FALSE;
              isMitosis = FALSE;
              if(board->get[k][l].canEat > bestScore) {		//Si come mas en la nueva posicion que en la mejor que tiene guardada, las cambia
                override = TRUE;
                sameScoreMoves = 1;
                if(abs(i - k) == 1 && abs(j - l) == 1)
                  isMitosis = TRUE;							//Chequea si es un movimiento de division, estos se consideran mejores que
              }                                             //los saltos porque ademas de lo que comen dejan un blob extra
              else if(board->get[k][l].canEat == bestScore) {
                if(abs(i - k) == 1 && abs(j - l) == 1)		//Si el nuevo movimiento come lo mismo que le mejor guardado y
                  isMitosis = TRUE;							//el nuevo es mitosis pero el guardado no, el nuevo es mejor entonces se reemplaza
                if(!isBMmitosis && isMitosis) {				//el guardado.
                  override = TRUE;
                  sameScoreMoves = 1;
                }
                else if(isMitosis == isBMmitosis) {			//Si los dos son del mismo tipo, se elige aleatoriamente cual se guarda
                  sameScoreMoves++;							//A medida que se van encontrando mas movimientos con el mismo puntaje
                  if(rand()%sameScoreMoves == 0)			//Va disminuyendo la probabilidad de que reemplacen el guardado.
                    override = TRUE;						//Esto es para que las primeras tengan las mismas chances que las ultimas
                }
              }

              if(override) {
                bestMove.source.y = i;						//Aqui se reemplaza el movimiento guardado por el nuevo
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

int endGame(typeBoard *board, int blobCount[], int player) {
  int i, j, otherPlayer, winner;
  if(player == 1)			//Player es el jugador que no se puede mover
    otherPlayer = 2;
  else
    otherPlayer = 1;

  for(i = 0; i < board->h; i++) {
    for(j = 0; j < board->w; j++) {
      if(board->get[i][j].owner == 0) {		//Se le asignan todos los espacios vacios al otro jugador
        board->get[i][j].owner = otherPlayer;
        blobCount[otherPlayer]++;
      }
    }
  }
  
  if(blobCount[1] > blobCount[2])		//Se calcula el ganador
    winner = 1;
  else
    winner = 2;

  return winner;
}

int getFilename(char **filename) {
  if((*filename = malloc(101 * sizeof(char))) == NULL) {
	printf("No hay suficiente espacio en el Heap\n");
	return FALSE;
  }
  int length, valid = FALSE;
  char input;

  while(!valid) {
    printf("Ingrese el nombre del archivo (máximo 100 caracteres): ");
    length = 0;
    while((input = getchar()) != '\n' && length < 100) {
      (*filename)[length] = input;
      length++;
    }
    if(input == '\n')	//Si el input quedo en \n significa que entraron 100 o menos caracteres
      valid = TRUE;
    else {
      printf("¡El nombre es demasiado largo!\n\n");
      while(getchar() != '\n'); //Vacía el buffer
    }
  }
  (*filename)[length] = '\0'; //Cierra el string

  return TRUE;
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
            switch(board->get[i][j].owner) {		//Se convierte el tablero a chars y se lo guarda en un buffer,
              case 0:								//Lo que facilita y agiliza la operacion de guardado
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

int getInt(char *message) {
  char input, output, valid = FALSE;
  while(!valid) {
    output = 0;
    printf(message);
    while((input = getchar()) != '\n') {
      if(input >= '0' && input <= '9') {
	    output = (10*output) + (input - '0');  //Dezplaza los digitos de output un lugar a la izquierda e inserta input
	    valid = TRUE;
      }
      else {
        valid = FALSE;
        while(getchar() != '\n'); //Vacía el buffer
        break;
      }
    }
  }
  return output;
}
