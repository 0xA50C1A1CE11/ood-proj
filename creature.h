/* states */

#define Normal 0
#define inCombat 1
#define Sleeping 2
#define Chasing 3

class Creature
{
  public:
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
  public:
    int state;
    Route *route;
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
