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
  int canEat;
} typeBlob;

typedef struct{
  int h;
  int w;
  typeBlob **get;
}typeBoard;

int canMove(int player, typeBoard *board);
char* getCommand(typeCommand *command);
int isInside(int x, int y, int w, int h); //Just for the record
int validCommand(int player, typeCommand *command, typeBoard *board);
int move(int player, typeCommand *command, typeBoard *board);
void conquer(int player, typeCommand *command, typeBoard *board, int blobCount[]);
void render(typeBoard *board, const int *blobCount);
void init(typeBoard *board);
void fill(typeBoard *board);
