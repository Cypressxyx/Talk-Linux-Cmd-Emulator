talk.x:  client.o getChar.o server.o server.x
	g++ -ggdb -o talk.x main.o getChar.o -lcurses -ltermcap -pthread -std=c++17

server.x: getChar.o
	g++ -ggdb -o server.x server.cpp getChar.o -lcurses -ltermcap -pthread -std=c++17

server.o: server.cpp
	g++ -c -ggdb server.cpp -o server.o -pthread --std=c++17

client.o: client.cpp
	g++ -c -ggdb client.cpp -o main.o -pthread --std=c++17

getChar.o: lib/getChar.cpp 
	gcc -c -ggdb lib/getChar.cpp -o getChar.o

clean:
	/bin/rm -f *.x *.o *.out *~ 
