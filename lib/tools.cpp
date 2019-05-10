/* ---------------------------
  Helper Functions
-----------------------------*/
#include "getChar.hpp"
#include <sstream>
#include <iostream>

bool quitProgram = false;
int writeTo          (int, char *); // write to a socket
int readFrom         (int, char *); // read from a socket
void startup         (void);             // init curses
void terminate       (void);             // end curses
void updatePos       (int &, int &);     // update position
void writeAndSend    (int, bool &, bool &);              // write to screen and send to socket
void recieveAndWrite (int, bool &, bool &);              // read from socket and write to screen
void sanitizeAndUpdateInput(int &, int &, char );
void addNewLine      (int y, int x);
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
		addNewLine(x,y);
	} else
		x += 1;
}

void recieveAndWrite(int connFD, bool &ended, bool &other) {
	char req = ' ';
	int res;
	int x = 0;
	int y = LINES - 1;
	while(1) {
    if (other)
        break;
		if ((res = readFrom(connFD, (char *)&req)) < 0) {
			perror("Read Error\n");
			exit(0);
		}
		if (res == 0) {
			fprintf(stderr, "Connection has been closed\n");
			break;
		}
		sanitizeAndUpdateInput(x,y,req);	
    if (quitProgram)
        break;
		req = ' ';
	}
  terminate();
  ended = true;
	close(connFD);
}


std::string charToStr(char c) {
	std::stringstream stream;
	stream << std::hex << int(c);
	return stream.str();
}

void addNewLine(int x, int y) {
	int lineToMove = y < LINES/2 ? 0 : LINES/2 + 1; //writing prompt or sending prompt?
	move(lineToMove,x); //move curser to begging
	deleteln();  //delete the begging line and move all text up
	move(y,x); //move cursor back to begging y
	insdelln(1); //insert a new line there to reset cursor
}

void sanitizeAndUpdateInput(int &x, int &y, char c) {
	std::string charInStr = charToStr(c);
	if (charInStr ==  "7f" or charInStr == "08") { //delete in mac, backspace in windows
			x = x - 1;
			mvaddch(y,x,' ');
			x = x - 1;
	} else if (charInStr == "4") {
      quitProgram = true;
	} else {
		if (charInStr ==  "d") {    // new line
        x = -1;
		}
		mvaddch(y,x,c);
	}
	updatePos(x,y);
	move(y,x);
	if (charInStr == "d") 
		addNewLine(x,y);
	refresh();
}

void writeAndSend(int sendSocketFD, bool &ended, bool &other) {
	char c;
	int x = 0;
	int y = LINES/2 - 1;
	while(1) {

    if (other)
        break;
		c = get_char();
		sanitizeAndUpdateInput(x,y,c);	
    if (quitProgram)
        break;
		/* send character to the socket */
		if ((writeTo(sendSocketFD, (char *)&c)) < 0 ) {
			fprintf(stderr, "Failed to send character to server\n");
			exit(0);
		}
	}
  ended = true;
  terminate();
}


// Write to a fd a char from buff
int writeTo(int fd, char *buff) {
	return write(fd, buff, 1);
}

// Read from a fd a char to the buf
int readFrom(int fd, char *buff) {
	return read(fd, buff, 1);
}
