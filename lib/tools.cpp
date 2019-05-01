/* ---------------------------
  Helper Functions 
-----------------------------*/
#include "getChar.hpp"

void startup    (void);
void terminate  (void);
void updatePos  (int &, int &);
int writeTo     (int, char *, int);
int readFrom    (int, char *, int);

void startup(void) {
	initscr();
	cbreak();
	mvaddstr(25,0,"----------");

}

void terminate(void) {
	mvcur(0, COLS-1, LINES-1, 0);
	clear();
	refresh();
	endwin();
}

void updatePos(int &x, int &y) {
	if ( x > 65) {
		x =  0;
		y += 1;
	} else 
		x += 1;
}

// Write to a fd a char from buff
int writeTo(int fd, char *buff) {
	return write(fd, buff, 1);
}

// Read from a fd a char to the buf 
int readFrom(int fd, char *buff) {
	return read(fd, buff, 1);
}
