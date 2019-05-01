/* ---------------------------
  Helper Functions
-----------------------------*/
#include "getChar.hpp"

int writeTo          (int, char *); // write to a socket
int readFrom         (int, char *); // read from a socket
void startup         (void);             // init curses
void terminate       (void);             // end curses
void updatePos       (int &, int &);     // update position
void writeAndSend    (int);              // write to screen and send to socket
void recieveAndWrite (int);              // read from socket and write to screen

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

void recieveAndWrite(int connFD) {
	char req = ' ';
	int res;
	int x = 0;
	int y = 25;
	while(1) {
		if ((res = readFrom(connFD, (char *)&req)) < 0) {
			perror("Read Error\n");
			exit(0);
		}
		if (res == 0) {
			fprintf(stderr, "Connection has been closed\n");
			break;
		}
		mvaddch(y,x,req);
		updatePos(x,y);
		refresh();
		req = ' ';
	}
	close(connFD);
}

void writeAndSend(int sendSocketFD) {
	char c;
	int x = 0;
	int y = 0;
	while(1) {
		c = get_char();
		mvaddch(y,x,c);
		updatePos(x,y);
		refresh();
		/* send character to the socket */
		if ((writeTo(sendSocketFD, (char *)&c)) < 0 ) {
			fprintf(stderr, "Failed to send character to server\n");
			exit(0);
		}
	}
}


// Write to a fd a char from buff
int writeTo(int fd, char *buff) {
	return write(fd, buff, 1);
}

// Read from a fd a char to the buf
int readFrom(int fd, char *buff) {
	return read(fd, buff, 1);
}
