#ifndef _BLOBSBACK_H_
  #define _BLOBSBACK_H_

  #define INCREMENT 1
  #define DECREMENT -1
  #define TRUE 1
  #define FALSE 0
  #define AIPLAYER 2

  typedef enum {MENU, NEWGAME, GAME, SAVE, LOAD, END, QUIT} gameState;

  typedef struct {
    int x;
    int y;
  } typeCoord;

  typedef struct {
    typeCoord source;
    typeCoord target;
  } typeCommand;

  typedef struct {
    int owner;
    int canMove;
    int canEat;
  } typeBlob;

  typedef struct {
    int h;
    int w;
    typeBlob **get;
  } typeBoard;

  int canMove(int player, typeBoard *board);
  void conquer(int player, typeCommand *command, typeBoard *board, int blobCount[]);
  int endGame(typeBoard *board, int blobCount[]);
  void fill(typeBoard *board);
  void getAImove(typeCommand *command, typeBoard *board);
  int getCommand(typeCommand *command, char **output);
  char* getFilename();
  int getint(char *message);
  int init(typeBoard *board, char *loadedArray);
  int isInside(int x, int y, int w, int h); //Just for the record
  int load(char *filename, int *vsAI, int *player, int blobCount[], typeBoard *board, char **loadedArray);
  int move(int player, typeCommand *command, typeBoard *board);
  void render(typeBoard *board, const int *blobCount,int player);
  int save(char *filename, int vsAI, int player, int blobCount[], typeBoard *board);
  int validCommand(int player, typeCommand *command, typeBoard *board);
#endif
