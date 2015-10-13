all: *.o *.h main.cc Node.o Functions.o
	g++ main.cc Record.o Node.o Functions.o

Record.o: Record.cc Record.h
	g++ -c Record.cc

Node.o:  Node.cc Node.h
	g++ -c Node.cc

Functions.o: Functions.cc Functions.h
	g++ -c Functions.cc
