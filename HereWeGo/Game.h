#pragma once
#include "utils.h"
#include "Screen.h"
#include "Room.h"
#include "Player.h"

class Game {
	Screen screen;
	Room room;
	Player players[2];

	void printHUD();
public:
	Game(); //defined in .cpp
	void init();
	void run();
};