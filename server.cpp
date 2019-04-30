#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#define PORT_NUMBER 24
#define MAXLINE 64
#define LISTENQ 5

void doReadChars(int);
int readN(int fd, char *ptr, int size);

int main(int argc, char **argv) {
	int       listenFD, connFD, pid;
	time_t    ticks;
	char      buff[MAXLINE];
	struct    sockaddr_in servaddr, cliaddr;
	socklen_t len;
	
	// Create end-point for IPv4 Internet Protocol
	if ((listenFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {	
		fprintf(stderr, "Socket end Point Init Failed: %s\n", strerror(errno));
		exit(1);
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family        = AF_INET;             // Communicate using internet domain
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
			doReadChars(connFD);
			close(connFD);
			exit(0);
		}
		close(connFD);
	}
}

/* get char from client */
void doReadChars(int connFD) {
	char req = ' ';
	int res;
	while(true) {
		req = ' ';
		if ((res = readN(connFD, (char *)&req, sizeof(req))) < 0) {
			perror("Read Error");
			exit(0);
		}
		if (res == 0) {
			fprintf(stderr, "Connection Closed");
			close(connFD);
			break;
		}
		fprintf(stderr, "%c", req);
		
	}
	close(connFD);
}

int readN(int connFD, char *c, int size) {
	int val = read(connFD,c, 1);
	return val;
} 
