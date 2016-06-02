typedef enum {MENU, GAME, SAVE, LOAD, END, QUIT} gameState;

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
} typeBlob;

typedef struct{
  int h;
  int w;
  typeBlob** get;
}typeBoard;

int canMove(int player, typeBoard* board);
char* getCommand(typeCommand* command);
int isInside(int x, int y, int w, int h); //Just for the record
int validCommand(typeCommand* command, typeBoard* board, int player);
