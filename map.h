/*
  This is basic map representation based on 'tile' term and
  it's only focused at generating landscape/corridors/doors/rooms,
  handling collision, performing character tile movement and so on.
*/


#include <stdlib.h>
#include <iostream>
#include <vector>
#include <stdio.h>


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
#define Tunnel 2


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


struct Route
{
  Point point;
  Route *next;
  Route(int y,int x)
  {
    this->point = Point(x,y);
    this->next = NULL;
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


    void drawPlayer(Point location)
    {
      system("clear");
      char ** shot = new char*[this->height+1];
      for(int i=0;i<=this->height+1;i++)
      {
        shot[i] = new char[width+1];
        for(int j=0;j<=this->width+1;j++)
          shot[i][j] = drawRule(this->terrian[i][j]);
      }
      shot[location.y][location.x] = '@';
      for(int i=0;i<=this->height+1;i++)
      {
        for(int j=0;j<=this->width+1;j++)
        {
          std::cout<<shot[i][j];
        }
        std::cout<<std::endl;
      }
    }


    Point randomPoint()
    {
      Room rand_room = this->roomV[rand() % this->roomV.size()]->room;
      int x_max = rand_room.botRight.x,
          x_min = rand_room.topLeft.x,
          y_max = rand_room.botRight.y,
          y_min = rand_room.topLeft.y;
      int x = from_range(x_min,x_min,x_max),
          y = from_range(y_min,y_min,y_max);
      return Point(x,y);
    }


    void Clear()
    {
      for(int i=0;i<=this->height+1;i++)
        for(int j=0;j<=this->width+1;j++)
          this->terrian[i][j] = Wall;
    }

    bool legalMove(int x, int y)
    {
      return this->terrian[y][x]==Tunnel or
             this->terrian[y][x]==Floor;
    }
    void makeMap(int a,int b)
    {
      this->BSPGen(a,b);
    }
    bool checkMove(Point p)
    {
      return this->terrian[p.y][p.x]==Floor or this->terrian[p.y][p.x]==Tunnel;
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
    std::vector<RoomTree *> roomV;
    //Creature **monsters;
    //Creature *player;

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
          this->terrian[h_break_dot][i] = Tunnel;

            low = MIN(a_rand_y,h_break_dot),
            up  = MAX(a_rand_y,h_break_dot);

        for(int i=low;i<=up;i++)
          this->terrian[i][a_rand_x] = Tunnel;

            low = MIN(b_rand_y,h_break_dot),
            up  = MAX(b_rand_y,h_break_dot);

        for(int i=low;i<=up;i++)
          this->terrian[i][b_rand_x] = Tunnel;
      }
      else
      {
        int lower = MIN(a_rand_x,b_rand_x),
            upper = MAX(a_rand_x,b_rand_x),
            v_break_dot = from_range(lower,lower,upper);

        int low = MIN(a_rand_y,b_rand_y),
            up  = MAX(a_rand_y,b_rand_y);

        for(int i=low;i<=up;i++)
          this->terrian[i][v_break_dot] = Tunnel;

            low = MIN(a_rand_x,v_break_dot),
            up  = MAX(a_rand_x,v_break_dot);

        for(int i=low;i<=up;i++)
          this->terrian[a_rand_y][i] = Tunnel;

            low = MIN(b_rand_x,v_break_dot),
            up  = MAX(b_rand_x,v_break_dot);

        for(int i=low;i<=up;i++)
          this->terrian[b_rand_y][i] = Tunnel;
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
      /* no error if invalid */
      if(min_box_sz < min_room_sz)
      {
        int tmp = min_box_sz;
        min_box_sz = min_room_sz;
        min_room_sz = min_box_sz;
      }

      /* expand room tree */
      this->BSsplit(min_box_sz,min_room_sz,this->roomT);

      /* collect leafs aka single rooms */
      LeafsCollect(this->roomT,this->roomV);

      /* clear treeian */
      this->Clear();

      /* draw connection between rooms */
      int sz = this->roomV.size()-1;
      
      while(sz)
      {
        ConnectRooms(this->roomV[--sz],this->roomV[sz]);
      }
      
      /* then draw rooms themselves */
      for(int i=0;i<this->roomV.size();i++)
        DrawRoom(this->roomV[i]);
    }
    
    
    void RouteCollector(int **m,int y,int x,Route *r)
    {
      r->next = new Route(y,x);
      if(m[y][x]==0) return;
      int dy = (m[y-1][x] == m[y][x]-1)? -1 :(m[y+1][x] == m[y][x]-1)? 1: 0,
          dx = (dy != 0)? 0 :(m[y+1][x] == m[y][x]-1)? -1: +1;
      RouteCollector(m,y+dy,x+dx,r->next);
    }


    void BuildRoute(int end_y, int end_x, int y,int x)
    {
      int **matr = new int*[this->height+1];
      for(int i=0;i <= this->height+1;i++)
        matr[i] = new int[this->width+1];
      for(int i=0;i<=this->height+1;i++)
        for(int j=0;j<=this->width+1;j++)
          matr[i][j] = (terrian[i][j]==0 or terrian[i][j]==2)? -2: -1;
      matr[y][x] = 0;
      int dx[] = {1,0,-1,0},
          dy[] = {0,1,0,-1};
      int d=0;
      bool proceed = true;
      while(matr[end_y][end_x]==-2 and proceed)
      {
        proceed = false;
        for(int i=0;i<this->height;++i)
        {
          for(int j=0;j<this->width;++j)
          {
            if(matr[i][j]==d)
            {
              for(int k=0;k<4;k++)
              {
                int iy = i + dy[k],
                    ix = j + dx[k];
                if ( iy >= 0 and iy < this->height and
                     ix >= 0 and ix < this->width and
                    matr[iy][ix] == -2)
                {
                  proceed = true;
                  matr[iy][ix] = d + 1;
                }
              }
            }
          }
        }
        d++;
      }

      Route* r = new Route(end_y,end_x);
      this->RouteCollector(matr,end_y,end_x,r);
      for(Route *q = r; q; q=q->next)
        matr[q->point.y][q->point.x] = 0;

      for(int i=0;i <= this->height+1; i++)
      {
        for(int j=0;j <= this->width+1; j++)
        {
          std::cout << ((matr[i][j]==-1)? '#': (matr[i][j]==0)? '.': ' ');
        }
        std::cout<<std::endl;
      }
    };
    
    inline bool isIn(int dot_x,int dot_y,RoomTree* node)
    {
      return dot_x <= node->room.botRight.x and
             dot_x >= node->room.topLeft.x and
             dot_y <= node->room.botRight.x and
             dot_y >= node->room.topLeft.y;
    }
    /* returns rectangle where certain creature may see objects */
    Room ReturnLOS(int obj_x, int obj_y)
    {
      if(this->terrian[obj_y][obj_x]==Tunnel)
      {
        return Room(obj_x-1,obj_y-1,obj_x+1,obj_y+1);
      }
      else
      {
        int i=0;
        /*throws exception object insnt belong to any room nor any tunnel
          by exceeding vectors size*/
        while(not (isIn(obj_x,obj_y,this->roomV[i++])));
        return this->roomV[i]->room;
      }
    }
    char drawRule(int num)
    {
      switch(num)
      {
        case Tunnel: return '.';
        case Floor: return '.';
        case Wall : return '#';
      }
    }
};
