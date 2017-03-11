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
}Rectangle;


#define rectang(a,b,c,d) {{(a),(b)},{(c),(d)}}


typedef struct
{
  Rectangle rect;
  Rectangle *left,*right;
}RectangleTree;


class Map
{
  private:

    int **terrian,height,width,deep;//the deeper you go, the more dangerous
                                      //shall apear
    RectanlgeTree *rectT;
    //objects,beings,items list not yet implemented

  public:


    Map(int height,int width)
    {
      self.height = height;
      self.width = width;
      rectT = new RectangleTree;
      self.rectT->rect = rectang(0,0,height,width);

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
    void BSsplit(int min_room_sizem,RectangleTree *tree = self->rectT)
    {
      int xDiff = tree->rect.botRight.x - tree->rect.topLeft.x,
          yDiff = tree->rect.botRight.y - tree->rect.topLeft.y;
      /* if current room too small to split, stop*/
      if(xDiff < min_room_size*2 or yDiff < min_room_size*2)
        return;
      /* otherwise choose split type and  random point in suitable range */
      if(rand() % 2)
      {
        /* horizontal split */
        int h_split = min_room_size*2 + (rand() % (yDiff - min_room_size*2));
        //not yet implemented
      }
      else
      {
        /* vertical split*/
        int v_split = min_room_size*2 + (rand() % (xDiff - min_room_size*2));
      }
    }
    void BSPGen(int min_room_size)
    {
      //not yet implemented
      
    }


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
