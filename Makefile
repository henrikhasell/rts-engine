all:
	g++ -g -Wall -oengine -std=c++11 main.cpp -llua5.2
irrlicht:
	g++ -g -Wall -oengine -std=c++11 irrlicht.cpp -lIrrlicht
