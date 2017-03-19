#include "map.h"
#include <stdio.h>
int
main(int argc, char *argv[])
{
  Map *m = new Map(20,20);
  //m->TEST(atoi(argv[1]),atoi(argv[2]));
  m->TEST(6,5);
  m->ROUTETEST(2,2,5,15);
  return 0;
}
