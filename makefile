CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -pthread

all: BGRSclient
	g++ -o bin/BGRSclient bin/connectionHandler.o bin/Frame.o bin/Client_Server.o bin/BGRSclient.o $(LDFLAGS)

BGRSclient: bin/connectionHandler.o bin/Frame.o bin/Client_Server.o bin/BGRSclient.o
	
bin/connectionHandler.o: src/connectionHandler.cpp
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/Frame.o: src/Frame.cpp
	g++ $(CFLAGS) -o bin/Frame.o src/Frame.cpp

bin/Client_Server.o: src/Client_Server.cpp
	g++ $(CFLAGS) -o bin/Client_Server.o src/Client_Server.cpp

bin/BGRSclient.o: src/BGRSclient.cpp
	g++ $(CFLAGS) -o bin/BGRSclient.o src/BGRSclient.cpp
	
.PHONY: clean
clean:
	rm -f bin/*
