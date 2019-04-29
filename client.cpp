/* PORT NUMBER 8012 8013*/
#include <iostream>
#include <curses.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "lib/getChar.hpp"
#define PORT_NUMBER 24
#define MAXLINE 200

void startup   (void);
void terminate (void);
void inBounds  (int &, int &);
int written(int , char *, int);

int main(int argc, char **argv) {
	/* Create curses */
	int x,y = 0;
	startup();    /* Initilize Program */
	move(x,y);    /* Move the curser to the top and left most block*/
	refresh();    /* Refresh the screen */
	/* end of create curses */

	/* Create client to server connection */
	int sockFD, n;
	char recvline[MAXLINE + 1];
	struct sockaddr_in servaddr;
	if (argc != 2) {
		fprintf(stderr, "Usage: %s UserName\n", argv[0]);	
		exit(1);
	}
	// create socket end point fo communcation. Using IPv4 format and stream socket
	if ((sockFD = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		fprintf(stderr, "Socket Error. %s\n", strerror(errno));
		exit(1);
	}

	//build a profile for the server 
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(PORT_NUMBER);
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
		fprintf(stderr, "inet ptron failed. %s\n", argv[1]);
		exit(1);
	}
	
	//build a connection to the server
	if (connect(sockFD, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
		fprintf(stderr, "Connect error: %s\n", strerror(errno));
		exit(1);
	}
	
	/* end of creatin connections */
	char c;
	while(true) {
		c = get_char();
		mvaddch(y,x,c);
		inBounds(x,y);
		refresh();
		/* send character to the server*/
		if ((written(sockFD, (char *)&c, sizeof(c))) < 0) {
			fprintf(stderr, "Failed to send character to server\n");
			exit(0);
		}
	}
	terminate();  /* Terminate Program */
}

void inBounds(int &x, int &y) {
	if (x > 65) {
		x = 0;
		y += 1;
	} else 
		x += 1;
}

void startup(void) {
	initscr();   /* Activate curses */
	curs_set(0); /* Preven the cursor from showing */
	cbreak();    /* Change stty so that characters are delieverd to the program as they are typed*/
	mvaddstr(25, 0, "______________________________________________________________________________________________________");
}

void terminate(void) {
	mvcur(0, COLS-1, LINES-1, 0);
	clear();
	refresh();
	endwin();
}

int written(int fd, char *c, int size) {
	int no_left, no_written;
	no_left = size;
	while ( no_left > 0) {
		no_written = write(fd, c, no_left);
		if ( no_written <= 0) 
			return no_written;
		no_left -= no_written;
		c += no_written;
	}
	return size - no_left;
}

