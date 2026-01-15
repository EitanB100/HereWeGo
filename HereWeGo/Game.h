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

static constexpr int LEVEL_COUNT = 3;

static constexpr int PLAYER_AMOUNT = 2;
static constexpr int MAX_SCORE = 100000;

static constexpr int KEY_COUNT = static_cast<int>(CommandKeys::NUM_KEYS);
extern char p1Keys[KEY_COUNT];
extern char p2Keys[KEY_COUNT];

class Game {
	

	std::string loadingErrorMessage = "Unknown error";

	Player players[PLAYER_AMOUNT];
	
	
	int savefiles = 0; // ho much save files there 
	
	bool useColor;

	std::vector<Riddle> riddles;
	virtual void handleRiddle(int riddleID, Player& player, Room& room);


	void printHUD();
	void printTimer();
	void resetLevelTimer();
	bool checkLevelTransition(int& currentLevelIndex, Point p1, Point p2);
	void printScore(const Point& hudPos);
	void updatePlayerKeys(char keys[], int playerNum);
	void handleGameOver();
	void tileMapError();
	void saveGame();
	void saveGlobalSaveConfig(); // type on file how many saves are
	void loadGlobalSaveConfig(); // load how many saves are into savefiles
	
protected:
	Screen screen;
	std::vector<Room> levels;
	bool levelLoadedCorrectly = true;

	std::chrono::steady_clock::time_point startTime;      // Total game time
	std::chrono::steady_clock::time_point levelStartTime; // Current level time
	
	
	int currentLevelIndex = 0;
	int score = 0;

	virtual char getInput();
	virtual void sleepFrame() { Sleep(GAME_SPEED); }

	virtual void onLevelChange(int levelInd) {} //transitioning between levels
	virtual void onLifeLost() {} //when getting hit/getting a riddle wrong
	virtual void onRiddleSolved(bool correct) {} //when answering a riddle

public:

	static char p1Keys[KEY_COUNT];
	static char p2Keys[KEY_COUNT];
	
	Game(); //defined in .cpp
	void setGame(int levelIndex, bool firstSettings);
	void setCurrentLevelIndex(int level) { currentLevelIndex = level; }
	void setScore(int val) { score = val; }
	int getSavefilesCount() const { return savefiles; }
	bool getUseColor() { return useColor; }
	void toggleColor();

	void startInLevel(Level level);

	void showEndingScreen();

	void init();
	void run();
	
	void settingsMenu();
	bool loadGame(int slot); // slot is the number (id of the game file)

	void loadMenu(); // for picking a save file
};