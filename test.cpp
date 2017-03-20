#include "map.h"
#include <stdio.h>
int
main(int argc, char *argv[])
{
  Map *m = new Map(20,30);
  //m->TEST(atoi(argv[1]),atoi(argv[2]));
  m->TEST(4,5);
  int a,b,c,d;
  std::cin>>a>>b>>c>>d;
  m->ROUTETEST(a,b,c,d);
  return 0;
}
