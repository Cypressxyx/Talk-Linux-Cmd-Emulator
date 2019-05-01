#include <time.h>
#include <thread>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

#include <curses.h>
#include "lib/tools.cpp"

#define PORT_NUMBER 24
#define MAXLINE 64
#define LISTENQ 5


void runTalkServer   (int);

int main(int argc, char **argv) {
	int       listenFD, connFD, pid;
	char      buff[MAXLINE];
	struct    sockaddr_in servaddr, cliaddr;
	socklen_t len;

	// Create end-point for IPv4 Internet Protocol
	if ((listenFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "Socket end Point Init Failed: %s\n", strerror(errno));
		exit(1);
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;             // Communicate using internet domain
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   // Who should we accept conections from
	servaddr.sin_port        = htons(PORT_NUMBER);  // Port to listen on

	// Bind server end point using specs in serveraddr
	if (bind(listenFD, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		fprintf(stderr, "Bind Failed. %s\n", strerror(errno));
		exit(1);
	}

	// Listen on the incoming connections, pule up at most LISTENQ connections
	if (listen(listenFD, LISTENQ) < 0 ) {
		fprintf(stderr, "Listen Failed. %s\n", strerror(errno));
		exit(1);
	}

	for ( ; ; ) {
		// establish connection with an incoming client.
		len = sizeof(cliaddr);
		if ((connFD = accept(listenFD, (struct sockaddr *) &cliaddr, &len)) < 0 ) {
			fprintf(stderr, "Accept failed. %s \n", strerror(errno));
			exit(1);
		}
		printf("Connection from %s port accepted %d\n",
						inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
						ntohs(cliaddr.sin_port));

		if ( (pid=fork()) == 0) {
			close(listenFD);
			runTalkServer(connFD);
			close(connFD);
			exit(0);
		}
		close(connFD);
	}
}

/* Recieve and send from talk client */
void runTalkServer(int connFD) {
	startup();
	move(0,0);
	refresh();
	bool recieveTerminate = false;
	bool sendTerminate    = false;
	std::thread recieveThread(recieveAndWrite, connFD);
	std::thread writeThread(writeAndSend, connFD);
	while (recieveTerminate == false) {}
	recieveTerminate = false;
	sendTerminate    = false;
	close(connFD);
	terminate();
}
