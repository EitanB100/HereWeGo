#pragma once
#include "utils.h"
#include "Screen.h"
#include "Room.h"
#include "Player.h"

class Game {
	Screen screen;
	Room room;
	Player players[2];

	std::vector<Obstacle> transferredObstacles; //for obstacles that are moved between rooms

	void printHUD();
public:
	Game(); //defined in .cpp
	void init();
	void run();
};