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

typedef struct{
  int h;
  int w;
  typeBlob **get;
} typeBoard;

typedef struct
	{
		int mode;
		int nextTurn;
		int h;
		int w;
		int blobsPlayer1;
		int blobsPlayer2;
		char blobs[1000];

} typeSave;

int canMove(int player, typeBoard *board);
char* getCommand(typeCommand *command);
int isInside(int x, int y, int w, int h); //Just for the record
int validCommand(int player, typeCommand *command, typeBoard *board);
int move(int player, typeCommand *command, typeBoard *board);
void conquer(int player, typeCommand *command, typeBoard *board, int blobCount[]);
void render(typeBoard *board, const int *blobCount,int player);
void init(typeBoard *board, char **loadedArray, int dimx, int dimy);
void fill(typeBoard *board);
void fillEatAndMove(typeBoard *board);
void renderMaps(typeBoard *board);
void getAImove(typeCommand *command, typeBoard *board);
int endGame(typeBoard *board, int blobCount[]);
int save(char *filename, int mode, int player, typeBoard *board);
