talk.x:  client.o getChar.o server.o
	g++ -ggdb -o talk.x main.o getChar.o -lcurses -ltermcap

server.o: server.cpp
	g++ -c -ggdb server.cpp -o server.o

client.o: client.cpp
	g++ -c -ggdb client.cpp -o main.o

getChar.o: lib/getChar.cpp 
	gcc -c -ggdb lib/getChar.cpp -o getChar.o

clean:
	/bin/rm -f screen.x screen.o getChar.o *~
