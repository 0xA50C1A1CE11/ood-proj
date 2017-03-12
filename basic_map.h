/*
  This is basic map representation based on 'tile' term and
  it's only focused at generating landscape/corridors/doors/rooms,
  handling collision, performing character tile movement and so on.
*/


//includes and defines goes here


/* 
  Following section containts representation of terrian objects.
  Since map is almost entirely filled with terrian objects and there is
  no need for terrian tile to do anything, it is reasonable to store terrian
  as 2d array of integers representing certain type.
  Objects,items,and beings must have more complicated representation.
*/


#define Floor 0
#define Wall 1


typedef struct 
{
  int x;
  int y;
} Point;


typedef struct
{
  Point topLeft;
  point botRight;
}Room;


#define MAKE_ROOM(a,b,c,d) {{(a),(b)},{(c),(d)}}


typedef struct
{
  Room rect;
  Room *left,*right;
}RoomTree;


class Map
{
  private:

    int **terrian,height,width,deep;//the deeper you go, the more dangerous
                                      //shall apear
    RoomTree *rectT;
    //objects,beings,items list not yet implemented

  public:


    Map(int height,int width)
    {
      self.height = height;
      self.width = width;
      rectT = new RoomTree;
      self.rectT->rect = MAKE_ROOM(0,0,height,width);

      self.terrian = new int*[height];
      for(int i=0;i<height;i++) 
        self.terrian[i] = new int[width];
    }


    void Clear()
    {
      for(int i=0;i<self.height;i++)
        for(int j=0;j<self.width;j++)
          self.terrian[i][j] = Floor;
    }


    /*
       Simple binary space partitation alghorithm, capable only for 
       generating rooms/corridors/halls, not for open spaces.
       BSsplit builds the binary tree, BSPGen creates rooms and
       connection between them.
    */
    void BSsplit(int min_room_size,RoomTree *tree = self->rectT)
    {
      int xDiff = tree->rect.botRight.x - tree->rect.topLeft.x,
          yDiff = tree->rect.botRight.y - tree->rect.topLeft.y;
      int tL_x = tree->rect.topLeft.x,
          tL_y = tree->rect.topLeft.y,
          bR_x = tree->rect.botRight.x,
          bR_y = tree->rect.botRight.y;
      bool v_split_OK = xDiff > min_room_size*2,
           h_split_OK = yDiff > min_room_size*2,
           any_split_OK = xDiff and yDiff;
      /* if both splits if OK, choose one randomly */
      if (any_split_OK and rand() % 2)
      {
        //maybe wrong \(o_o)/ , not yet tested
        int h_split_dot = min_room_size + (rand() % (yDiff - min_room_size*2));
        tree->left = new RoomTree;
        tree->right = new RoomTree;
        tree->left->rect = MAKE_ROOM(tL_x, tL_y, bR_x, h_splt_dot-1);
        tree->right->rect = MAKE_ROOM(tL_x, h_split_dot+1 ,bR_x, bRy);
        self.BSsplit(min_room_size,tree->left);
        self.BSsplit(min_room_size,tree->right);
      }
      /* if only one is ok, use it */
      else if (v_split_OK)
      {
        int v_split_dot = min_room_size + (rand() % (yDiff - min_room_size*2));
        tree->left = new RoomTree;
        tree->right = new RoomTree;
        tree->left->rect = MAKE_ROOM(tL_x, tL_y, v_split_dot-1, bRy);
        tree->right->rect = MAKE_ROOM(v_split_dot+1, tLy ,bR_x, bRy);
        self.BSsplit(min_room_size,tree->left);
        self.BSsplit(min_room_size,tree->right);
      }
      else if (h_split_OK)
      {
        int h_split_dot = min_room_size + (rand() % (yDiff - min_room_size*2));
        tree->left = new RoomTree;
        tree->right = new RoomTree;
        tree->left->rect = MAKE_ROOM(tL_x, tL_y, bR_x, h_splt_dot-1);
        tree->right->rect = MAKE_ROOM(tL_x, h_split_dot+1 ,bR_x, bRy);
        self.BSsplit(min_room_size,tree->left);
        self.BSsplit(min_room_size,tree->right);
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


    /* ObjectsGen spawns objects in dungeon (enemies/items/door/etc) */
    void ObjectsGen()
    {
      //not yet implemented
    }


    ~Map()
    {
      for(int i=0;i<height;i++) 
        delete self.terrian[i];
      delete self.terrian;
    }
}
