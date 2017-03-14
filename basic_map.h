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
    this->left = (this->right = NULL);
  }
};


class Map
{
  private:

    int **terrian,height,width,deep;//the deeper you go, the more dangerous
                                    //enemies shall apear
    RoomTree *roomT;
    int rooms_count;
    //objects,beings,items list not yet implemented


    /*
       Simple binary space partitation alghorithm, capable only for 
       generating rooms/corridors/halls, not for open spaces.
       BSsplit builds the binary tree, BSPGen creates rooms and
       connection between them.
    */
    void BSsplit(int min_room_size,struct RoomTree *tree)
    {
      
      this->rooms_count++;
      
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
        int v_split_dot = min_room_size + (rand() % (xDiff - min_room_size*2));
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
      /* othewise make a room and stop */
      else
      {
        double room_scale = (6 + rand() % 4)*0.1;

        int room_width = room_scale * (bR_x - tL_x),
            room_height = room_scale * (bR_y - tL_y),
            begin_y_range = (bR_x - tL_x) - room_height,
            begin_x_range = (bR_y - tL_y) - room_width,
            room_start_y = tL_y + rand() % begin_y_range,
            room_start_x = tL_x + rand() % begin_x_range;
        tree->room = Room(room_start_x,
                          room_start_y,
                          room_start_x + room_width,
                          room_start_y + room_height);
        this->rooms_count--;
      }
    }


    /*
       The alghorithm of building room connection searchs for neighboring
       rooms, makes connection between them, and then removes them from a tree.
       Connected room is assigned to their lua. Proceeds until only one
       node left.
    */
    inline bool isLeaf(RoomTree *node)
    {
        return node->right == NULL and node->left == NULL;
    }
   

    #define RANDOM_POINT_IN(room)\
      Point((room.topLeft.x + rand() % (room.botRight.x - room.topLeft.x)),\
            (room.topLeft.y + rand() % (room.botRight.y - room.topLeft.y)))
    #define MIN(a,b) ((a)<(b)) ? (a) : (b)
    #define MAX(a,b) ((a)>(b)) ? (a) : (b)
    /* code here might look a bit creepe at first glance */
    void ConnectNieghbors(RoomTree *tree)
    {
      
      /* select two random points, each resides inside certain room */
      Point a = RANDOM_POINT_IN(tree->left->room),
            b = RANDOM_POINT_IN(tree->right->room);
      /* make a range between them */
      int min_x = MIN(a.x,b.x),
          min_y = MIN(a.y,b.y),
          max_x = MAX(a.x,b.x),
          max_y = MAX(a.y,a.y);
      /* horizontal fracture  */
      if(rand() % 2)
      {
        int fracture_dot = min_y + rand() % (max_y - min_y),
            direction = (a.y < fracture_dot) ? 1 : -1;
        for(int i=min_x;i<=max_x;i++)
          this->terrian[i][fracture_dot] = Floor;
        for(int i=a.y;i!=fracture_dot;i+direction)
          this->terrian[a.x][i] = Floor;
        for(int i=b.y;i!=fracture_dot;i-direction)
          this->terrian[b.x][i] = Floor;
      }
      /* vertiacal fracture */
      else
      {
        int fracture_dot = min_x + rand() % (max_x - min_x),
            direction = (a.x < fracture_dot) ? 1 : -1;
        for(int i=min_y;i<=max_y;i++)
          this->terrian[fracture_dot][i] = Floor;
        for(int i=a.x;i!=fracture_dot;i+direction)
          this->terrian[i][a.y] = Floor;
        for(int i=b.x;i!=fracture_dot;i-direction)
          this->terrian[i][b.y] = Floor;
      }
    }
    void DrawRoom(RoomTree *tree)
    {
      for(int i = tree->room.botRight.x; i <= tree->room.topLeft.x; i++)
        for(int j = tree->room.botRight.y; i <= tree->room.topLeft.y; j++)
          this->terrian[i][j] = Floor;
    }
    void BSPGen(int min_room_size)
    {
      
    }


  public:


    Map(int height,int width)
    {
      this->rooms_count = 1;
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
          this->terrian[i][j] = Wall;
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
