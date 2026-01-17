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

	static constexpr int REPLAY_SPEED = 10;

	int currentTick = 0;

	std::vector<Step> steps;
	std::vector<ExpectedEvent> expectedEvents;
	std::vector<ExpectedEvent> actualEvents;
	int nextStepInd = 0;

	void loadExpectedResult();
	void recordActualEvent(int time, const std::string& description);
public:
	ReplayGame(bool silent);
	~ReplayGame();

	void run() override;
	char getInput() override;
	char getInteractionInput() override;
	char getCharFromCommand(int playerID, const std::string& command);
	void sleepFrame() override {
		if (!isSilent) Sleep(REPLAY_SPEED);
	}

	void handleRiddle(int riddleID, Player& player, Room& room) override;
	void onLevelChange(int levelInd) override;
	void onLifeLost() override;
	void onRiddleSolved(bool correct) override;
};