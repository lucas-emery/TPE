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
  const char* pattern;
  char* output;
  int state = 0;
  int  valid = 0, i = 0, length = 0;

  while(!valid) {
    output = NULL;
    *command = {{-1,-1},{-1,-1}}; //WIP INIT
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
            state = 4; //invalid
          i++;
          break;

        case 1:
          if(pattern[i] == '#') {
            if(input >= '0' && input <= '9') { //XY o YX ???
              if(command->source.x == -1) command->source.x = input-'0';
              else if(command->source.y == -1) command->source.y = input-'0';
              else if(command->target.x == -1) command->target.x = input-'0';
              else if(command->target.y == -1) command->target.y = input-'0';
            }
            else
              state = 4;
          }
          else if(pattern[i] == '*') {
            if(input >= '0' && input <= '9') {
              if(command->source.x < 10) command->source.x = command->source.x*10 + (input-'0');
              else if(command->source.y < 10) command->source.y = command->source.y*10 + (input-'0');
              else if(command->target.x < 10) command->target.x = command->target.x*10 + (input-'0');
              else if(command->target.y < 10) command->target.y = command->target.y*10 + (input-'0');
            }
            else
              i++;
          }
          else if(input == pattern[i]) {
            i++;
            if(pattern[i] == '\0')
              valid = 1;
          }
          else {
            state = 4;
            valid = 0;
          }
          break;

        case 2:
          if(pattern[i] == 'F') {
            if(length < 15) {
              if(output == NULL)
                output = (char*) malloc(16*sizeof(char)); //Extra space for \0
              output[length] = input;
              length++;
              valid = 1;
            }
            else {
              valid = 0;
              printf("Filename too long, max length is 15");
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
            if(pattern[i] == '\0')
              output = (char*) malloc(sizeof(char));
              *output = EOF;
              valid = 1;
          }
          else {
            valid = 0;
            state = 4;
          }
          break;

        case 4:
          //EMPTY BUFFER
          break;
      }
    }
  }
  return output;
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
