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
