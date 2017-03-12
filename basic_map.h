/*
  This is basic map representation based on 'tile' term and
  it's only focused at generating landscape/corridors/doors/rooms,
  handling collision, performing character tile movement and so on.
*/


#include <stdlib.h>


/* 
  Following section containts representation of terrian objects.
  Since map is almost entirely filled with terrian objects and there is
  no need for terrian tile to do anything, it is reasonable to store terrian
  as 2d array of integers representing certain type.
  Objects,items,and beings must have more complicated representation.
*/


#define Floor 0
#define Wall 1


struct Point
{
  int x;
  int y;

  Point()
  {
    this->x = (this->x =0);
  }

  Point(int x,int y)
  {
    this->x = x; 
    this->y = y;
  }
};


struct Room
{
  Point topLeft;
  Point botRight;
 
  Room()
  {
    this->topLeft = (this->botRight = Point(0,0));
  } 

  Room(int tLx,int tLy,int bRx, int bRy)
  {
    this->topLeft = Point(tLx,tLy);
    this->botRight = Point(bRx,bRy);
  }
};


struct RoomTree
{
  Room room;
  RoomTree *left,*right;
  RoomTree(int tLx,int tLy,int bRx, int bRy)
  {
    this->room = Room(tLx,tLy,bRx,bRy);
    this->left = (this->right = nullptr);
  }
};


class Map
{
  private:

    int **terrian,height,width,deep;//the deeper you go, the more dangerous
                                    //enemies shall apear
    RoomTree *roomT;
    //objects,beings,items list not yet implemented


    /*
       Simple binary space partitation alghorithm, capable only for 
       generating rooms/corridors/halls, not for open spaces.
       BSsplit builds the binary tree, BSPGen creates rooms and
       connection between them.
    */
    void BSsplit(int min_room_size,struct RoomTree *tree)
    {

      int xDiff = tree->room.botRight.x - tree->room.topLeft.x,
          yDiff = tree->room.botRight.y - tree->room.topLeft.y;

      int tL_x = tree->room.topLeft.x,
          tL_y = tree->room.topLeft.y,
          bR_x = tree->room.botRight.x,
          bR_y = tree->room.botRight.y;

      bool v_split_OK = xDiff > min_room_size*2,
           h_split_OK = yDiff > min_room_size*2,
           any_split_OK = xDiff and yDiff;

      /* if both splits if OK, choose one randomly */
      if (any_split_OK and rand() % 2)
      {
        //maybe wrong \(o_o)/ , not yet tested
        int h_split_dot = min_room_size + (rand() % (yDiff - min_room_size*2));
        this->BSsplit(min_room_size,tree->left = new RoomTree(tL_x,
                                                             tL_y,
                                                             bR_x,
                                                             h_split_dot-1));
        this->BSsplit(min_room_size,tree->right = new RoomTree(tL_x,
                                                              h_split_dot+1,
                                                              bR_x,
                                                              bR_y));
      }
      /* if only one is ok, use it */
      else if (v_split_OK)
      {
        int v_split_dot = min_room_size + (rand() % (yDiff - min_room_size*2));
        this->BSsplit(min_room_size,tree->left = new RoomTree(tL_x,
                                                             tL_y,
                                                             v_split_dot-1,
                                                             bR_y));
        this->BSsplit(min_room_size,tree->right = new RoomTree(v_split_dot+1,
                                                              tL_y,
                                                              bR_x,
                                                              bR_y));
      }
      else if (h_split_OK)
      {
        int h_split_dot = min_room_size + (rand() % (yDiff - min_room_size*2));
        this->BSsplit(min_room_size,tree->left = new RoomTree(tL_x,
                                                             tL_y,
                                                             bR_x,
                                                             h_split_dot-1));
        this->BSsplit(min_room_size,tree->right = new RoomTree(tL_x,
                                                              h_split_dot+1,
                                                              bR_x,
                                                              bR_y));
      }
      /* othewise stop */
      else
      {
        return;
      }
    }
    void BSPGen(int min_room_size)
    {
      //not yet implemented
    }


  public:


    Map(int height,int width)
    {
      this->height = height;
      this->width = width;
      this->roomT = new RoomTree(0,0,height,width);

      this->terrian = new int*[height];
      for(int i=0;i<height;i++) 
        this->terrian[i] = new int[width];
    }


    void Clear()
    {
      for(int i=0;i<this->height;i++)
        for(int j=0;j<this->width;j++)
          this->terrian[i][j] = Floor;
    }


    /* ObjectsGen spawns objects in dungeon (enemies/items/door/etc) */
    void ObjectsGen()
    {
      //not yet implemented
    }


    ~Map()
    {
      for(int i=0;i<height;i++)
        delete this->terrian[i];
      delete this->terrian;
    }
};
