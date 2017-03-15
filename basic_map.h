/*
  This is basic map representation based on 'tile' term and
  it's only focused at generating landscape/corridors/doors/rooms,
  handling collision, performing character tile movement and so on.
*/


#include <stdlib.h>
#include <iostream>
#include <vector>


/* 
  Following section containts representation of terrian objects.
  Since map is almost entirely filled with terrian objects and there is
  no need for terrian tile to do anything, it is reasonable to store terrian
  as 2d array of integers representing certain type.
  Objects,items,and beings must have more complicated representation.
*/

#define MIN(a,b) ((a)<(b))? (a) : (b)
#define MAX(a,b) ((a)>(b))? (a) : (b)

inline int from_range(int initial,int min,int max)
{
  if((max-min) == 0) return initial;
  else return initial + (rand() % (max-min));
}

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
    this->left=(this->right=NULL);
  }
  RoomTree(int tLx,int tLy,int bRx, int bRy)
  {
    this->room = Room(tLx,tLy,bRx,bRy);
    this->left = (this->right = NULL);
  }
  ~RoomTree()
  {
    if(this->left != NULL) delete this->left;
    if(this->right != NULL) delete this->right;
  }
};


class Map
{


  public:


    Map(int height,int width)
    {
      this->height = height;
      this->width = width;
      this->roomT = new RoomTree(1,1,width,height);

      this->terrian = new int*[height+1];
      for(int i=0;i<=height+1;i++) 
        this->terrian[i] = new int[width+1];
    }


    void Clear()
    {
      for(int i=0;i<=this->height+1;i++)
        for(int j=0;j<=this->width+1;j++)
          this->terrian[i][j] = Wall;
    }

    
    /* for testing */
    void TEST(int min_box_sz,int min_rm_sz)
    {
      BSPGen(min_box_sz,min_rm_sz);
      for(int i=0;i <= this->height+1; i++)
      {
        for(int j=0;j <= this->width+1; j++)
        {
          std::cout << ((this->terrian[i][j]==Floor)? ".": "#");
        }
        std::cout<<std::endl;
      }
    }


    ~Map()
    {
      for(int i=0;i<height;i++)
        delete this->terrian[i];
      delete this->terrian;
      delete this->roomT;
    }


  private:


    int **terrian,height,width;
    RoomTree *roomT;

    /*
       Simple binary space partitation alghorithm, capable only for 
       generating rooms/corridors/halls, not for open spaces.
       BSsplit builds the binary tree, BSPGen creates rooms and
       connection between them.
    */
    void BSsplit(int min_box_sz,int min_room_sz,struct RoomTree* &tree)
    {
      int tL_x = tree->room.topLeft.x,
          tL_y = tree->room.topLeft.y,
          bR_x = tree->room.botRight.x,
          bR_y = tree->room.botRight.y;
      bool h_split_ok = (bR_y - tL_y) > min_box_sz*2,
           v_split_ok = (bR_x - tL_x) > min_box_sz*2,
           both_splits_ok = h_split_ok and v_split_ok;
      /* if both splits are ok, choose one randomly */
      if(both_splits_ok and rand() % 2)
      {
        int h_split_dot = from_range(tL_y+min_box_sz,tL_y+min_box_sz*2,bR_y);
 
        tree->left = new RoomTree(tL_x,tL_y,bR_x,h_split_dot-1);
        tree->right = new RoomTree(tL_x,h_split_dot+1,bR_x,bR_y);

        BSsplit(min_box_sz,min_room_sz,tree->left);
        BSsplit(min_box_sz,min_room_sz,tree->right);
      }
      else if(v_split_ok)
      {
        int v_split_dot = from_range(tL_x+min_box_sz,tL_x+min_box_sz*2,bR_x);

        tree->left = new RoomTree(tL_x,tL_y,v_split_dot-1,bR_y);
        tree->right = new RoomTree(v_split_dot+1,tL_y,bR_x,bR_y);

        BSsplit(min_box_sz,min_room_sz,tree->left);
        BSsplit(min_box_sz,min_room_sz,tree->right);
      }
      else if(h_split_ok)
      {
        int h_split_dot = from_range(tL_y+min_box_sz,tL_y+min_box_sz*2,bR_y);
        
        tree->left = new RoomTree(tL_x,tL_y,bR_x,h_split_dot-1);
        tree->right = new RoomTree(tL_x,h_split_dot+1,bR_x,bR_y);

        BSsplit(min_box_sz,min_room_sz,tree->left);
        BSsplit(min_box_sz,min_room_sz,tree->right);
      }
      else
      {
        if((bR_y-tL_y-min_room_sz) == 0 or (bR_x-tL_x-min_room_sz)==0)
        if((bR_y - tL_y - min_room_sz) == 0 or (bR_x - tL_x - min_room_sz)==0)
          return;
        /* choose room size and location in box randomly*/
        int room_height = from_range(min_room_sz,tL_y+min_room_sz,bR_y),
            room_width = from_range(min_room_sz,tL_x+min_room_sz,bR_x),
            start_y = from_range(tL_y,tL_y+room_height,bR_y),
            start_x = from_range(tL_x,tL_x+room_width,bR_x);
            tree->room = Room(start_x,start_y,
                          start_x+room_width,start_y+room_height);
      }
    }

    /* build connection between rooms */
    void ConnectRooms(RoomTree *a,RoomTree * b)
    {
      int a_rand_y = from_range(a->room.topLeft.y,
                                a->room.topLeft.y,
                                a->room.botRight.y),

          a_rand_x = from_range(a->room.topLeft.x,
                                a->room.topLeft.x,
                                a->room.botRight.x),

          b_rand_y = from_range(b->room.topLeft.y,
                                b->room.topLeft.y,
                                b->room.botRight.y),
    
          b_rand_x = from_range(b->room.topLeft.x,
                                b->room.topLeft.x,
                                b->room.botRight.x);


          a_rand_x = from_range(a->room.topLeft.x,
                               a->room.topLeft.x, 
                               a->room.botRight.x),

          b_rand_y = from_range(b->room.topLeft.y,
                               b->room.topLeft.y,
                               b->room.botRight.y),

          b_rand_x = from_range(b->room.topLeft.x,
                               b->room.topLeft.x,
                               b->room.botRight.x);
      

      /* 50% horizontal angle */
      if(rand() % 2)
      {
        int lower = MIN(a_rand_y,b_rand_y),
            upper = MAX(a_rand_y,b_rand_y),
            h_break_dot = from_range(lower,lower,upper);

        int low = MIN(a_rand_x,b_rand_x),
            up  = MAX(a_rand_x,b_rand_x);

        for(int i=low;i<=up;i++)
          this->terrian[h_break_dot][i] = Floor;

            low = MIN(a_rand_y,h_break_dot),
            up  = MAX(a_rand_y,h_break_dot);

        for(int i=low;i<=up;i++)
          this->terrian[i][a_rand_x] = Floor;

            low = MIN(b_rand_y,h_break_dot),
            up  = MAX(b_rand_y,h_break_dot);

        for(int i=low;i<=up;i++)
          this->terrian[i][b_rand_x] = Floor;
      }
      else
      {
        int lower = MIN(a_rand_x,b_rand_x),
            upper = MAX(a_rand_x,b_rand_x),
            v_break_dot = from_range(lower,lower,upper);

        int low = MIN(a_rand_y,b_rand_y),
            up  = MAX(a_rand_y,b_rand_y);

        for(int i=low;i<=up;i++)
          this->terrian[i][v_break_dot] = Floor;

            low = MIN(a_rand_x,v_break_dot),
            up  = MAX(a_rand_x,v_break_dot);

        for(int i=low;i<=up;i++)
          this->terrian[a_rand_y][i] = Floor;

            low = MIN(b_rand_x,v_break_dot),
            up  = MAX(b_rand_x,v_break_dot);

        for(int i=low;i<=up;i++)
          this->terrian[b_rand_y][i] = Floor;
      }
    }


    void DrawRoom(RoomTree* tree)
    { 
      for(int i = tree->room.topLeft.y; i <= tree->room.botRight.y; i++)
        for(int j = tree->room.topLeft.x; j <= tree->room.botRight.x; j++)
          this->terrian[i][j] = Floor;
    }
    void LeafsCollect(RoomTree *tree,std::vector <RoomTree*> &vect)
    {
      if(tree->left == NULL and tree->right == NULL)
      {
        vect.push_back(tree);
        return;
      }
      if(tree->left != NULL)
        LeafsCollect(tree->left,vect);
      if(tree->right != NULL)
        LeafsCollect(tree->right,vect);
    }
    void BSPGen(int min_box_sz,int min_room_sz)
    {
      /* expand room tree */
      this->BSsplit(min_box_sz,min_room_sz,this->roomT);

      /* collect leafs aka single rooms */
      std::vector <RoomTree*> leafs;
      LeafsCollect(this->roomT,leafs);

      /* clear treeian */
      this->Clear();

      /* draw rooms... */
      for(int i=0;i<leafs.size();i++)
        DrawRoom(leafs[i]);

      /* ... and connection between them*/
      int sz = 0;

      while((sz = leafs.size())-1)
      {
        ConnectRooms(leafs[sz-1],leafs[sz-2]);
        leafs.pop_back();
      }
    }
};
