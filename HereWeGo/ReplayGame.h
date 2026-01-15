#pragma once
#include "Game.h"
#include <fstream>
#include <vector> 

class ReplayGame : public Game {
	struct Step {
		int tick;
		char key;
	};
	
	struct ExpectedEvent {
		int time;
		std::string description;
	};

	int currentTick = 0;

	std::vector<Step> steps;
	std::vector<ExpectedEvent> expectedEvents;
	std::vector<ExpectedEvent> actualEvents;
	int nextStepInd = 0;
	bool isSilent;

	void loadExpectedResult();
	void recordActualEvent(int time, const std::string& description);
public:
	ReplayGame(bool silent);
	~ReplayGame();

	char getInput() override;
	char getInteractionInput() override;
	
	void sleepFrame() override {
		if (!isSilent) Sleep(GAME_SPEED);
	}

	void handleRiddle(int riddleID, Player& player, Room& room) override;
	void onLevelChange(int levelInd) override;
	void onLifeLost() override;
	void onRiddleSolved(bool correct) override;
};