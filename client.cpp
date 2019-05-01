/* PORT NUMBER 8012 8013*/
#include <iostream>
#include <curses.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

//#include "lib/getChar.hpp"
#include "lib/tools.cpp"

#define PORT_NUMBER 24
#define MAXLINE 200
void runTalkClient(int);

int main(int argc, char **argv) {

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
	runTalkClient(sockFD);
}

void runTalkClient(int clientFD) {
	startup();    /* Initilize Program */
	move(0,0);    /* Move the curser to the top and left most block*/
	refresh();    /* Refresh the screen */
	std::thread writeThread(writeAndSend, clientFD);
	std::thread recieveThread(recieveAndWrite, clientFD);
	while(1) {}
	terminate();  /* Terminate Program */
}
