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
	void initLevel2(Room& r);
	void initLevel3(Room& r);

	void printHUD();

public:
	Game(); //defined in .cpp
	void setGame(int level);
	void init();
	void run();
	void level1props(Room& r);
	void level2props(Room& r);
	void level3props(Room& r);
};