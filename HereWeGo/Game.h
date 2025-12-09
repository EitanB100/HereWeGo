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
	Point exitPoints[ROOM_AMOUNT];

	Point p1StartPoints[ROOM_AMOUNT];
	
	Point p2StartPoints[ROOM_AMOUNT];

	int currentLevelID = 0;

	void printHUD();

public:
	Game(); //defined in .cpp
	void setGame(int level);
	void init();
	void run();
	void initLevel1Props(Room& r);
	void initLevel2Props(Room& r);
	void initLevel3Props(Room& r);
};