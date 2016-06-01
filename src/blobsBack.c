#include <stdio.h>
#include <stdlib.h>
#include "blobsBack.h"


int canMove(int player, typeBoard* board) {
  int i, j;
  for(i = 0; i < board->h; i++) {
    for(j = 0; j < board->w; j++) {
      if(board->get[i][j].owner == player && board->get[i][j].canMove) {
        return 1;
      }
    }
  }
  return 0;
}

char* getCommand(typeCommand* command) {
  char input;
  int state = 0;
  int i = 0, j = 0;

  while((input = getchar()) != '\n') {
    switch(state) {
      case 0:
        if(input == '[') state = 1;
        else if(input == 's') state = 2;
        else if(input == 'q') state = 3;
        else state = 4;
        i++;
        break;

      case 1:
        if(input >= '0' && input <= '9');
    }
  }
}

int isInside(int x, int y, int w, int h) {
  if(x >= 0 && x < w && y >= 0 && y < h)
    return 1;
  else
    return 0;
}

int validCommand(typeCommand* command, typeBoard* board, int player) {
  if(!isInside(command->source.x, command->source.y, board->w, board->h))
    return 0;
  else if(!isInside(command->target.x, command->target.y, board->w, board->h))
    return 0;
  else if(abs(command->source.x - command->target.x) > 2 || abs(command->source.y - command->target.y) > 2)
    return 0;
  else if(board->get[command->source.y][command->source.x].owner != player)
    return 0;
  else if(board->get[command->target.y][command->target.x].owner != 0)
    return 0;
  else
    return 1;
}
