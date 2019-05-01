/* ---------------------------
  Helper Functions
-----------------------------*/
#include "getChar.hpp"
#include <sstream>

int writeTo          (int, char *); // write to a socket
int readFrom         (int, char *); // read from a socket
void startup         (void);             // init curses
void terminate       (void);             // end curses
void updatePos       (int &, int &);     // update position
void writeAndSend    (int);              // write to screen and send to socket
void recieveAndWrite (int);              // read from socket and write to screen
void sanitizeAndUpdateInput(int &, int &, char );
std::string charToStr(char ); 

void startup(void) {
	initscr();
	cbreak();
	mvhline(LINES / 2, 0,ACS_HLINE,COLS);

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
		y -= 1;
	} else
		x += 1;
}

void recieveAndWrite(int connFD) {
	char req = ' ';
	int res;
	int x = 0;
	int y = LINES - 1;
	while(1) {
		if ((res = readFrom(connFD, (char *)&req)) < 0) {
			perror("Read Error\n");
			exit(0);
		}
		if (res == 0) {
			fprintf(stderr, "Connection has been closed\n");
			break;
		}
		sanitizeAndUpdateInput(x,y,req);	
		req = ' ';
	}
	close(connFD);
}


std::string charToStr(char c) {
	std::stringstream stream;
	stream << std::hex << int(c);
	return stream.str();
}

void sanitizeAndUpdateInput(int &x, int &y, char c) {
	std::string charInStr = charToStr(c);
	if (charInStr ==  "7f" or charInStr == "08") { //delete in mac, backspace in windows
			x = x - 1;
			mvaddch(y,x,' ');
			x = x - 1;
	} else {
		if (charInStr ==  "d") {    // new line
			x = -1;
		}
		mvaddch(y,x,c);
	}
	updatePos(x,y);
	move(y,x);
	if (charInStr == "d") {
		int lineToMove = y < LINES/2 ? 0 : LINES/2 + 1; //writing prompt or sending prompt?
		move(lineToMove,x); //move curser to begging
		deleteln();  //delete the begging line and move all text up
		move(y,x); //move cursor back to begging y
		insdelln(1); //insert a new line there to reset cursor
	}
	refresh();
}

void writeAndSend(int sendSocketFD) {
	char c;
	int x = 0;
	int y = LINES/2 - 1;
	while(1) {
		c = get_char();
		sanitizeAndUpdateInput(x,y,c);	
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
