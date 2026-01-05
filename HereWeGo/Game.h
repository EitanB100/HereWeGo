#pragma once
#include <iostream>
#include <iomanip> // For formatting output
#include <chrono>  // For time-related functions
#include <thread>  // For sleeping/pausing
#include <vector>
#include "utils.h"
#include "Screen.h"
#include "Room.h"
#include "Player.h"
#include "Riddle.h"

static constexpr int GAME_SPEED = 100;

static constexpr int LEVEL_COUNT = 4;

static constexpr int PLAYER_AMOUNT = 2;
static constexpr int MAX_SCORE = 100000;

static constexpr int KEY_COUNT = static_cast<int>(CommandKeys::NUM_KEYS);
extern char p1Keys[KEY_COUNT];
extern char p2Keys[KEY_COUNT];

class Game {
	Screen screen;
	std::vector<Room> levels;
	bool levelLoadedCorrectly = true;

	std::string loadingErrorMessage = "Unknown error";

	Player players[PLAYER_AMOUNT];
	
	std::chrono::steady_clock::time_point startTime;      // Total game time
	std::chrono::steady_clock::time_point levelStartTime; // Current level time
	
	int currentLevelIndex = 0;
	int score = 0;
	bool useColor;

	std::vector<Riddle> riddles;
	void handleRiddle(int riddleID, Player& player, Room& room);

	void printHUD();
	void printTimer();
	void resetLevelTimer();
	bool checkLevelTransition(int& currentLevelIndex, Point p1, Point p2);
	void printScore(const Point& hudPos);
	void updatePlayerKeys(char keys[], int playerNum);
	void handleGameOver();
	void tileMapError();

public:

	static char p1Keys[KEY_COUNT];
	static char p2Keys[KEY_COUNT];
	
	Game(); //defined in .cpp
	void setGame(int levelIndex, bool firstSettings);

	bool getUseColor() { return useColor; }
	void toggleColor();

	void startInLevel(Level level);
	
	void init();
	void run();
	
	void settingsMenu();
};