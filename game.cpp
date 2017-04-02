#include "map.h"
#include "creature.h"
#include <stdio.h>
#include <iostream>
class GameMaster
{
  private:
    Player *player;
    //Creatures list
    Map *map;
  public:
    GameMaster()
    {
      this->player = new Player(0,0);
      this->map = new Map(30,30);
    }
    void CreateMap()
    {
      map->makeMap(10,4);
      this->player->Move_to(this->map->randomPoint());
    }
    void Loop()
    {
      for(;;)
      {
        int c = getc(stdin);
        Point end;
        if(map->checkMove(end = player->checkMove(c)))
        {
          player->Move_to(end);
        }
        map->drawPlayer(this->player->location);
        //all creatures perform their move
      }
    }
};
int
main()
{
  auto g = GameMaster();
  g.CreateMap();
  g.Loop();
  return 0;
}
