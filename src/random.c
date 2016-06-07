#include <time.h>

#include "random.h"

void randomize()
{
  srand(time(0));
}

double randNormalize()
{
  return rand()/((double)RAND_MAX + 1);
}

int randInt( int izq, int der)
{
  return (int)(randNormalize()*(der - izq + 1 ) + izq);
}

double randReal( double izq, double der)
{
  return randNormalize()*(der-izq) + izq;
}
