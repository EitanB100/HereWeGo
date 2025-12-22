#pragma once
#include "utils.h"
#include "Screen.h"
#include "Room.h"
#include "Player.h"
static constexpr int ROOM_AMOUNT = 4; //4th is test room
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

	void checkLevelTransition(int& currentLevel, Point p1, Point p2);

public:
	Game(); //defined in .cpp
	void setGame(int level);

	void startInLevel(int levelID) {
		currentLevelID = levelID;
		setGame(levelID);
	}
	
	void init();
	void run();

	void initLevel1Props(Room& r);
	void initLevel2Props(Room& r);
	void initLevel3Props(Room& r);
	void initLevel4Props(Room& r);
};