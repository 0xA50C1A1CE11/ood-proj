struct Route
{
  Point point;
  Route *next;
};

/* states */

#define Normal 0
#define inCombat 1
#define Sleeping 2
#define Chasing 3

class Creature
{
  Point location;
  void Move();
  void Attack();
};
class Player: Creature
{
  void Move(); //to mark visited areas
};
class Monster: Creature
{
  int state;
  Route *route;
  void BuildRoute();
};
/* 
  TODO: 
    movement
    controls for player
    route and pathfiding
    AI
    LOS for monsters
    FOV for player
  */
