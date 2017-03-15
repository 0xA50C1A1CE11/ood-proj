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

#define DBG_VAR(var) std::cout<<(#var)<<" is "<< (var)<<std::endl;
inline int range_val(int initial,int min,int max)
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
    int leafs_c;
    //objects,beings,items list not yet implemented


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
        int h_split_dot = range_val(tL_y+min_box_sz,tL_y+min_box_sz*2,bR_y);
 
        tree->left = new RoomTree(tL_x,tL_y,bR_x,h_split_dot-1);
        tree->right = new RoomTree(tL_x,h_split_dot+1,bR_x,bR_y);

        BSsplit(min_box_sz,min_room_sz,tree->left);
        BSsplit(min_box_sz,min_room_sz,tree->right);
      }
      else if(v_split_ok)
      {
        int v_split_dot = range_val(tL_x+min_box_sz,tL_x+min_box_sz*2,bR_x);

        tree->left = new RoomTree(tL_x,tL_y,v_split_dot-1,bR_y);
        tree->right = new RoomTree(v_split_dot+1,tL_y,bR_x,bR_y);

        BSsplit(min_box_sz,min_room_sz,tree->left);
        BSsplit(min_box_sz,min_room_sz,tree->right);
      }
      else if(h_split_ok)
      {
        int h_split_dot = range_val(tL_y+min_box_sz,tL_y+min_box_sz*2,bR_y);
        
        tree->left = new RoomTree(tL_x,tL_y,bR_x,h_split_dot-1);
        tree->right = new RoomTree(tL_x,h_split_dot+1,bR_x,bR_y);

        BSsplit(min_box_sz,min_room_sz,tree->left);
        BSsplit(min_box_sz,min_room_sz,tree->right);
      }
      else
      {
        this->leafs_c++;return;
        if((bR_y - tL_y - min_room_sz) == 0 or (bR_x - tL_x - min_room_sz)==0)
          return;
        /* choose room size and location in box randomly*/
        int room_height = range_val(min_room_sz,tL_y+min_room_sz,bR_y),
            room_width = range_val(min_room_sz,tL_x+min_room_sz,bR_x),
            start_y = range_val(tL_y,tL_y+room_height,bR_y),
            start_x = range_val(tL_x,tL_x+room_width,bR_x);
        tree->room = Room(start_x,start_y,
                          start_x+room_width,start_y+room_height);
      }
    }


    void DrawRoom(RoomTree* tree)
    { 
      for(int i = tree->room.topLeft.y; i <= tree->room.botRight.y; i++)
      {
        for(int j = tree->room.topLeft.x; j <= tree->room.botRight.x; j++)
        {
          this->terrian[i][j] = Floor;
        }
      }
    }
    void LeafsCollect(RoomTree *tree,std::vector <RoomTree*> &vect)
    {
      if(tree->left == nullptr and tree->right == nullptr)
      {
        vect.push_back(tree);
        return;
      }
      if(tree->left != nullptr)
        LeafsCollect(tree->left,vect);
      if(tree->right != nullptr)
        LeafsCollect(tree->right,vect);
    }
    void BSPGen(int min_box_sz,int min_room_sz)
    {
      this->BSsplit(min_box_sz,min_room_sz,this->roomT);
      std::vector <RoomTree*> leafs;
      LeafsCollect(this->roomT,leafs);
      this->Clear();
      for(int i=0;i<leafs.size();i++)
        DrawRoom(leafs[i]);
    }


  public:


    Map(int height,int width)
    {
      this->height = height;
      this->width = width;
      this->leafs_c = 0;
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
    }
};
