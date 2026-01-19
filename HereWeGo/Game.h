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
	bool useColor;
	void resetLevelTimer();
	void printScore(const Point& hudPos);
	void updatePlayerKeys(char keys[], int playerNum);
	void loadGlobalSaveConfig(); // load how many saves are into savefiles

protected:
	Screen screen;
	std::vector<Room> levels;
	Player players[PLAYER_AMOUNT];
	bool levelLoadedCorrectly = true;
	bool isLoadMode = false;
	bool isSilent = false;
	int savefiles = 0; // ho much save files there 

	std::chrono::steady_clock::time_point startTime;      // Total game time
	std::chrono::steady_clock::time_point levelStartTime; // Current level time

	std::vector<Riddle> riddles;

	int currentLevelIndex = 0;
	int score = 0;

	virtual void drawGameFrame(Room& currRoom);

	virtual void handleRiddle(int riddleID, Player& player, Room& room);
	virtual char getInteractionInput();
	virtual void saveGame();
	virtual void resetRecording() {}
	virtual char getInput();
	virtual void sleepFrame() { Sleep(GAME_SPEED); }
	virtual void onLevelChange(int levelInd) {} //transitioning between levels
	virtual void onLifeLost() {} //when getting hit/getting a riddle wrong
	virtual void onRiddleSolved(bool correct) {} //when answering a riddle

	void updateGameLogic(char key, Room& currRoom, bool& boomDustCleaningNeeded, bool isSilent); // how each interation in run will do what the game logic asks
	// kids will allow to use it from the father
	void tileMapError();
	bool checkLevelTransition(int& currentLevelIndex, Point p1, Point p2);
	void handleGameOver();

	void printHUD();
	virtual void printTimer();
	void saveGlobalSaveConfig(); // type on file how many saves are

public:
	Game(); //defined in .cpp
	virtual ~Game() {};

	static char p1Keys[KEY_COUNT];
	static char p2Keys[KEY_COUNT];

	

	Room& getLevel(int index) { return levels[index]; }
	int getCurrentLevelIdx() const { return currentLevelIndex; }
	Player& getPlayer(int index) { return players[index]; }
	void setGame(int levelIndex, bool firstSettings);
	void setCurrentLevelIndex(int level) { currentLevelIndex = level; }
	void setScore(int val) { score = val; }
	int getSavefilesCount() const { return savefiles; }
	bool getUseColor() { return useColor; }
	void toggleColor();
	virtual void run();
	void startInLevel(Level level);
	void showEndingScreen();
	bool isSlotRecorded(int slot);
	void init();

	virtual void redrawScreen(Room& currRoom, bool isSilent);


	void settingsMenu();
	virtual bool loadGame(int slot); // slot is the number (id of the game file)

	void setLoadMode(bool mode) { isLoadMode = mode; }
	//void loadMenu(); // for picking a save file
	int getSlotFromLoadMenu();
	

};