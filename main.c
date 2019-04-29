/* PORT NUMBER 8012 8013*/
#include <iostream>
#include <curses.h>
#include "lib/getChar.hpp"

void startup   (void);
void terminate (void);

int main(void) {
	int x = 0;
	startup();    /* Initilize Program */
	move(0,0);    /* Move the curser to the top and left most block*/
	refresh();
	while(true) {
		mvaddch(0,x, get_char());
		x += 1;
		refresh();
	}
	terminate();  /* Terminate Program */
}

void startup(void) {
	initscr();   /* Activate curses */
	curs_set(0); /* Preven the cursor from showing */
	//clear();     /* Clear screen that curses provides*/
	cbreak();    /* Change stty so that characters are delieverd to the program as they are typed*/
}

void terminate(void) {
	mvcur(0, COLS-1, LINES-1, 0);
	clear();
	refresh();
	endwin();
}

