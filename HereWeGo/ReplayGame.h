#pragma once
#include "Game.h"
#include <fstream>
#include <vector> 

class ReplayGame : public Game {
	struct Step {
		int tick;
		int playerID;
		std::string command;
	};

	struct ExpectedEvent {
		int time;
		std::string description;
	};

	enum class ReplaySpeed { HALF = 0, NORMAL, DOUBLE, QUADRUPLE, OCTUPLE };
	ReplaySpeed currentSpeed = ReplaySpeed::NORMAL;


	static constexpr int SPEED_HALF = 200;    
	static constexpr int SPEED_NORMAL = 100;  
	static constexpr int SPEED_DOUBLE = 50;   
	static constexpr int SPEED_QUADRUPLE = 25;
	static constexpr float SPEED_OCTUPLE = 12.5f;

	static constexpr int REPLAY_SPEED = 10;

	int currentTick = 0;
	int levelStartTick = 0;

	std::vector<Step> steps;
	std::vector<ExpectedEvent> expectedEvents;
	std::vector<ExpectedEvent> actualEvents;
	int nextStepInd = 0;

	void loadExpectedResult();
	void recordActualEvent(int time, const std::string& description);
	
	void drawReplayUI();
	void drawProgressBar();
	void drawSpeedIndicator();
	void handleSpeedToggle(char c);
	int getCurrentSleepDuration() const;
public:
	ReplayGame(bool silent);
	~ReplayGame();

	void run() override;
	void redrawScreen(Room& currRoom) override;
	char getInput() override;
	char getInteractionInput() override;
	char getCharFromCommand(int playerID, const std::string& command);

	void drawGameFrame(Room& currRoom) override;

	void sleepFrame() override {
		if (!isSilent) Sleep(getCurrentSleepDuration());
	}

	void handleRiddle(int riddleID, Player& player, Room& room) override;
	void onLevelChange(int levelInd) override;
	void onLifeLost() override;
	void onRiddleSolved(bool correct) override;
	void printTimer() override;

};