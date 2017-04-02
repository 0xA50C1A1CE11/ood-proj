#include "map.h"
#include <stdio.h>
int
main(int argc, char *argv[])
{
  Map *m = new Map(40,100);
  m->TEST(atoi(argv[1]),atoi(argv[2]));
  return 0;
}
