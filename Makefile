talk.x:  main.o getChar.o
	g++ -ggdb -o talk.x main.o getChar.o -lcurses -ltermcap

main.o: main.c
	g++ -c -ggdb main.c -o main.o

getChar.o: lib/getChar.cpp 
	gcc -c -ggdb lib/getChar.cpp -o getChar.o

clean:
	/bin/rm -f screen.x screen.o getChar.o *~
