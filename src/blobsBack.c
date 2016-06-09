#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "blobsBack.h"

#define INCREMENT 1
#define DECREMENT -1

typedef enum {GETSOURCE, GETTARGET} AIstate;
typedef enum {EAT, MOVE} mapType;

void init(typeBoard *board){
  typeBlob *temp;
  int i;
  if((board->get = (typeBlob**) malloc(board->h * sizeof(typeBlob*))) == NULL){
    printf("Error de memoria\n");
  }
  temp = (typeBlob*) malloc(board->h * board->w * sizeof(typeBlob));
  if(temp == NULL)
  {
    printf("Error de memoria\n");
  }
  else
  {
    for (i = 0; i < board->h; i++) {
        board->get[i] = temp + (i * board->w);
    }
  }
}

void fill(typeBoard * board){ //prueba para el switch
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

void fillEatAndMove(typeBoard *board) {
  int moveMap[6][6] = {{8,10,13,13,10,8},
                       {10,14,18,18,14,10},
                       {13,18,23,23,18,13},
                       {13,18,23,23,18,13},
                       {10,14,18,18,14,10},
                       {8,10,13,13,10,8}};

  int eatMap[6][6] = {{0,1,0,0,0,0},
                      {1,1,0,0,0,0},
                      {0,0,0,0,0,0},
                      {0,0,0,0,0,0},
                      {1,1,0,0,0,0},
                      {0,1,0,0,0,0}};

  int i, j;
  for (i = 0; i < board->h; i++) {
    for (j = 0; j < board->w; j++) {
      board->get[i][j].canMove = moveMap[i][j];
      board->get[i][j].canEat = eatMap[i][j];
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
  int state;
  int valid = FALSE, i, length;

  while(!valid) {
    output = NULL;
    state = 0;
    i = 0;
    length = 0;
    command->source.x = -1;
    command->source.y = -1;
    command->target.x = -1;
    command->target.y = -1;

    while((input = getchar()) != '\n') {
      switch(state) {
        case 0:
          if(input == '[') {
            state = 1;
            pattern = "[#*,#*][#*,#*]";
          }
          else if(input == 's') {
            state = 2;
            pattern = "save F";
          }
          else if(input == 'q') {
            state = 3;
            pattern = "quit";
          }
          else
            state = 4; //invalid command
          i++;
          break;

        case 1:
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
              state = 4;
          }
          else if(input == pattern[i]) {
            i++;
            if(pattern[i] == '\0')
              valid = TRUE;
          }
          else {
            state = 4;
            valid = FALSE;
          }
          break;

        case 2:
          if(pattern[i] == 'F') {
            if(length < 15) {
              if(output == NULL)
                output = (char*) malloc(16*sizeof(char)); //Extra space for \0
              output[length] = input;
              length++;
              valid = TRUE;
            }
            else {
              valid = FALSE;
              printf("Filename too long, max length is 15 - ");
              state = 4;
            }
          }
          else if(input == pattern[i])
            i++;
          else
            state = 4;
          break;

        case 3:
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
            state = 4;
          }
          break;

        case 4:
          //EMPTY BUFFER
          break;
      }
    }

    if(!valid) {
      printf("Invalid command\n");
      free(output);
    }
  }

  if(output != NULL && *output == EOF) {
    printf("Do you want to save before quitting?(y/n): ");
    valid = FALSE;
    while(!valid) {
      input = getchar();
      if(getchar() == '\n') {
        if(input == 'y') {
          output = (char*) realloc(output, 16*sizeof(char)); //Extra space for \0
          while(!valid) {
            printf("Enter filename(max 15): ");
            length = 0;
            while(length < 15 && (input = getchar()) != '\n') {
              output[length] = input;
              length++;
            }
            if(length <= 15 && input == '\n')
              valid = TRUE;
            else {
              printf("Filename too long!\n");
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
        printf("Invalid answer, enter 'y' for yes or 'n' for no: ");
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
    printf("Invalid command, [%d,%d] doesn't exist!\n", command->source.y+1, command->source.x+1);
    return FALSE;
  }
  else if(!isInside(command->target.x, command->target.y, board->w, board->h)) {
    printf("Invalid command, [%d,%d] doesn't exist!\n", command->target.y+1, command->target.x+1);
    return FALSE;
  }
  else if(board->get[command->source.y][command->source.x].owner != player) {
    printf("Invalid command, [%d,%d] isn't yours!\n", command->source.y+1, command->source.x+1);
    return FALSE;
  }
  else if(abs(command->source.x - command->target.x) > 2 || abs(command->source.y - command->target.y) > 2) {
    printf("Invalid command, [%d,%d] can't move that far!\n", command->source.y+1, command->source.x+1);
    return FALSE;
  }
  else if(board->get[command->target.y][command->target.x].owner != 0) {
    printf("Invalid command, [%d,%d] isn't empty!\n", command->target.y+1, command->target.x+1);
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
  int i = 0, j = 0, minX, maxX, minY, maxY;
  typeCommand bestMove;
  typeCoord newMove;
  int bestScore = -1, isBMmitosis = 0, isMitosis;
  AIstate state = GETSOURCE;

  //bestMove.source.x = -1; //Signal that bestMove is empty

  int searching = TRUE;
  while(searching) {
    switch(state) {
      case GETSOURCE:
      //FOR WITH BREAK LOOP??
        if(i == board->h) {
          searching = FALSE;
        }
        else {
          if(board->get[i][j].owner == AIPLAYER && board->get[i][j].canMove) {
            newMove.x = j;
            newMove.y = i;
            state = GETTARGET;
            printf("GotSource [%d,%d]", i, j);
          }

          j++;
          if(j == board->w) {
            j = 0;
            i++;
          }
        }
        break;

      case GETTARGET:
        minX = newMove.x-2;
        maxX = newMove.x+2;
        minY = newMove.y-2;
        maxY = newMove.y+2;

        if(minX < 0)
          minX = 0;
        else if(maxX >= board->w)
          maxX = board->w - 1;
        if(minY < 0)
          minY = 0;
        else if(maxY >= board->h)
          maxY = board->h - 1;

        int k, l, override, sameScoreMoves;
        for(k = minY; k <= maxY; k++) {
          for(l = minX; l <= maxX; l++) {
            if(board->get[k][l].owner == 0) {
              override = FALSE;
              isMitosis = FALSE;
              if(board->get[k][l].canEat > bestScore) {
                override = TRUE;
                sameScoreMoves = 1;
                if(abs(newMove.x - l) == 1 && abs(newMove.y - k) == 1)
                  isMitosis = TRUE;
              }
              else if(board->get[k][l].canEat == bestScore) {
                if(abs(newMove.x - l) == 1 && abs(newMove.y - k) == 1)
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
                bestMove.source.x = newMove.x;
                bestMove.source.y = newMove.y;
                bestMove.target.x = l;
                bestMove.target.y = k;
                bestScore = board->get[k][l].canEat;
                isBMmitosis = isMitosis;
                printf("GotTarget [%d,%d]", k, l);
              }
            }
          }
        }
        state = GETSOURCE;
        break;
    }
  }
  getchar();
  *command = bestMove;
}
