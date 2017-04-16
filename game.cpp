#include "map.h"
#include "creature.h"
#include <stdio.h>
#include <iostream>
#include <vector>
class GameMaster
{
  private:
    Player *player;
    std::vector <Monster*> monsters;
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
      for(int i = 0;i<8;i++)
      {
        Point spawn = this->map->randomPoint(),
              reach = this->map->randomPoint();
        this->monsters.push_back(new Monster(spawn.y,spawn.x));
        this->map->BuildRoute(spawn.y,spawn.x,reach.y,reach.x,this->monsters.back()->route);
        this->map->Chain(this->monsters.back()->route);
      }
    }
    void Loop()
    {
      this->CreateMap();
      for(;;)
      {
        int c = getc(stdin);
        Point end;
        if(map->checkMove(end = player->checkMove(c)))
        {
          player->Move_to(end);
        }
        for(int i=0;i<this->monsters.size();i++)
          this->monsters[i]->step();
        Point * mnsters = new Point[8];
        for(int i=0;i<this->monsters.size();i++)
          mnsters[i] = this->monsters[i]->location;
        map->drawPlayer(this->player->location,mnsters,8);
      }
    }
};
int
main()
{
  auto g = GameMaster();
  g.Loop();
  return 0;
}
