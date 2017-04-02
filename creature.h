/* states */

#define Normal 0
#define inCombat 1
#define Sleeping 2
#define Chasing 3

class Creature
{
  public:
    Point location;
    
};
class Player: Creature
{
  public:
    Point location;
    Player(int x,int y)
    {
      this->location.x=x;
      this->location.y=y;
    }
    Point checkMove(int direction)
    {
      switch(direction)
      {
        case 'w': return Point(this->location.x,this->location.y-1);
        case 'a': return Point(this->location.x-1,this->location.y);
        case 's': return Point(this->location.x,this->location.y+1);
        case 'd': return Point(this->location.x+1,this->location.y);
        default: return Point(this->location.x,this->location.y);
      }
    }
    
    void Move_to(Point p)
    {
      this->location.x = p.x;
      this->location.y = p.y;
    }
    int Perform(int action)
    {
      switch(action)
      {
        case 'd':
          break;
        case 'a':
          break;
        case 's':
          break;
        case 'w':
          break;
        default:
          break;
      }
      return action;
    }
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
