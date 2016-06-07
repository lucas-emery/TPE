#include <stdio.h>
#include <stdlib.h>
#include "blobsBack.h"

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

int move(int player, typeCommand *command, typeBoard *board) {
  board->get[command->target.y][command->target.x].owner = player;
  if(abs(command->source.x - command->target.x) == 2 || abs(command->source.y - command->target.y) == 2) {
    board->get[command->source.y][command->source.x].owner = 0;
    return FALSE;
  }
  return TRUE;
}

void conquer(int player, typeCommand *command, typeBoard *board, int blobCount[]) {
  int otherPlayer, minX, maxX, minY, maxY;
  if(player == 1)
    otherPlayer = 2;
  else
    otherPlayer = 1;

  minX = command->target.x-1;
  maxX = command->target.x+1;
  minY = command->target.y-1;
  maxY = command->target.y+1;

  if(command->target.x == 0)
    minX++;
  else if(command->target.x == (board->w - 1))
    maxX--;
  if(command->target.y == 0)
    minY++;
  else if(command->target.y == (board->h - 1))
    maxY--;

  int i, j;
  for(i = minY; i <= maxY; i++) {
    for(j = minX; j <= maxX; j++) {
      if(board->get[i][j].owner == otherPlayer) {
        board->get[i][j].owner = player;
        blobCount[player]++;
        blobCount[otherPlayer]--;
      }
    }
  }
}
