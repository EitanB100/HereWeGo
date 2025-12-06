#pragma once
#include "utils.h"
#include "Screen.h"
#include "Room.h"
#include "Player.h"
static constexpr int ROOM_AMOUNT = 3;
static constexpr int PLAYER_AMOUNT = 2;

class Game {
	Screen screen;
	Room levels[ROOM_AMOUNT];
	Player players[PLAYER_AMOUNT];
	
	int currentLevelID = 0;

	void initLevel1(Room& r);
	void initLevel2(Room& room);
	void initLevel3(Room& room);

	void printHUD();

public:
	Game(); //defined in .cpp
	void init();
	void run();
};