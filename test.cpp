#include <ncurses.h>
#include "map.h"
#include <stdio.h>
int
main(int argc, char *argv[])
{
<<<<<<< HEAD
  Map *m = new Map(40,100);
  m->TEST(atoi(argv[1]),atoi(argv[2]));
  return 0;
=======
	int startx, starty, width, height;
	int ch;
	initscr();			/* Start curses mode 		*/
	cbreak();			/* Line buffering disabled, Pass on
					 * everty thing to me 		*/
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/
  	height = LINES-1;
	width = COLS-1;
	starty = (LINES - height) / 2;	/* Calculating for a center placement */
	startx = (COLS - width) / 2;	/* of the window		*/
	refresh();
  	//m->TEST(atoi(argv[1]),atoi(argv[2]));
  	//m->TEST(4,5);
  	Map *m = new Map(20,30);
  	m->TEST(atoi(argv[1]),atoi(argv[2]),height,width,starty,startx);
  	getch();
  	endwin();
  	/*
  	int a,b,c,d;
  	std::cin>>a>>b>>c>>d;
  	m->ROUTETEST(a,b,c,d);*/
  	return 0;
>>>>>>> 4c2b53fec6ff242700e04d53d6b73716f6333697
}
