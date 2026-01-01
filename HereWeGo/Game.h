#pragma once
#include <iostream>
#include <iomanip> // For formatting output
#include <chrono>  // For time-related functions
#include <thread>  // For sleeping/pausing
#include "utils.h"
#include "Screen.h"
#include "Room.h"
#include "Player.h"
#include "Riddle.h"

static constexpr int ROOM_AMOUNT = 4; //4th is test room
static constexpr int PLAYER_AMOUNT = 2;
static constexpr int MAX_SCORE = 100000;

static constexpr int KEY_COUNT = static_cast<int>(CommandKeys::NUM_KEYS);
extern char p1Keys[KEY_COUNT];
extern char p2Keys[KEY_COUNT];

class Game {
	Screen screen;
	Room levels[ROOM_AMOUNT];
	Player players[PLAYER_AMOUNT];
	Point exitPoints[ROOM_AMOUNT];
	Point p1StartPoints[ROOM_AMOUNT];
	Point p2StartPoints[ROOM_AMOUNT];
	std::chrono::steady_clock::time_point startTime;      // Total game time
	std::chrono::steady_clock::time_point levelStartTime; // Current level time
	Level currentLevelID;
	int score = 0;
	bool useColor;

	std::vector<Riddle> riddles;
	void handleRiddle(int riddleID, Player& player, Room& room);

	void printHUD();
	void printTimer();
	void resetLevelTimer();
	void checkLevelTransition(Level& currentLevel, Point p1, Point p2);
	void printScore();
	void updatePlayerKeys(char keys[], int playerNum);
	void handleGameOver();


public:
	Game(); //defined in .cpp
	void setGame(Level level, bool firstSettings);

	bool getUseColor() { return useColor; }
	void toggleColor();

	void startInLevel(Level level);
	
	void init();
	void run();

	void initLevel1Props(Room& r);
	void initLevel2Props(Room& r);
	void initLevel3Props(Room& r);
	void initLevel4Props(Room& r);
	void settingsMenu();
};