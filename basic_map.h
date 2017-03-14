/*
  This is basic map representation based on 'tile' term and
  it's only focused at generating landscape/corridors/doors/rooms,
  handling collision, performing character tile movement and so on.
*/


#include <stdlib.h>
#include <iostream>


/* 
  Following section containts representation of terrian objects.
  Since map is almost entirely filled with terrian objects and there is
  no need for terrian tile to do anything, it is reasonable to store terrian
  as 2d array of integers representing certain type.
  Objects,items,and beings must have more complicated representation.
*/

#define DBG_VAR(var) std::cout<<(#var)<<" is "<< (var)<<std::endl;
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
  RoomTree()
  {
    this->room = Room();
    this->left=(this->right=nullptr);
  }
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
    
    //objects,beings,items list not yet implemented


    /*
       Simple binary space partitation alghorithm, capable only for 
       generating rooms/corridors/halls, not for open spaces.
       BSsplit builds the binary tree, BSPGen creates rooms and
       connection between them.
    */
    void BSsplit(int min_room_size,struct RoomTree* tree)
    {
      int tL_x = tree->room.topLeft.x,
          tL_y = tree->room.topLeft.y,
          bR_x = tree->room.botRight.x,
          bR_y = tree->room.botRight.y;
      bool h_split_ok = (bR_y - tL_y) > min_room_size*2,
           v_split_ok = (bR_x - tL_x) > min_room_size*2,
           both_splits_ok = h_split_ok and v_split_ok;
      /* if both splits is ok, choose one randomly */
      if(both_splits_ok and rand() % 2)
      {
        int h_split_dot = tL_y + min_room_size +
                          (rand() % (bR_y - tL_y - min_room_size*2));
        tree->left = new RoomTree(tL_x,tL_y,bR_x,h_split_dot-1);
        tree->right = new RoomTree(tL_x,h_split_dot+1,bR_x,bR_y);
        BSsplit(min_room_size,tree->left);
        BSsplit(min_room_size,tree->right);
      }
      else if(v_split_ok)
      {
        int h_split_dot = tL_x + min_room_size +
                          (rand() % (bR_x - tL_x - min_room_size*2));
        tree->left = new RoomTree(tL_x,tL_y,h_split_dot-1,bR_y);
        tree->right = new RoomTree(h_split_dot+1,tL_y,bR_x,bR_y);
        BSsplit(min_room_size,tree->left);
        BSsplit(min_room_size,tree->right);
      }
      else if(h_split_ok)
      {
        int h_split_dot = tL_y + min_room_size +
                          (rand() % (bR_y - tL_y - min_room_size*2));
        tree->left = new RoomTree(tL_x,tL_y,bR_x,h_split_dot-1);
        tree->right = new RoomTree(tL_x,h_split_dot+1,bR_x,bR_y);
        BSsplit(min_room_size,tree->left);
        BSsplit(min_room_size,tree->right);
      }
      else
      {
        this->DrawRoom(tree);
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
    void DrawRoom(RoomTree* &tree)
    { 
      for(int i = tree->room.topLeft.x; i < tree->room.botRight.x; i++)
        for(int j = tree->room.topLeft.y; j < tree->room.botRight.y; j++)
          this->terrian[i][j] = Floor;
    }
    void TESTDRAW(RoomTree *tree)
    {
      if(tree == NULL) return;
      DrawRoom(tree);
      TESTDRAW(tree->left);
      TESTDRAW(tree->right);
    }
    void BSPGen(int min_room_size)
    {
      
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
          this->terrian[i][j] = Wall;
    }

    
    /* for testing */
    void TEST(int max_rm_sz)
    {
      this->Clear();
      this->BSsplit(max_rm_sz,this->roomT);
      for(int i=0;i < this->height; i++)
      {
        for(int j=0;j < this->width; j++)
        {
          std::cout << ((this->terrian[i][j]==Floor)? ".": "#");
        }
        std::cout<<std::endl;
      }
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
