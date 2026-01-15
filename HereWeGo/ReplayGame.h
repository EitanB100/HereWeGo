#pragma once
#include "Game.h"
#include <fstream>
#include <vector> 

class ReplayGame : public Game {
	struct Step {
		int tick;
		char key;
	};

	std::vector<Step> steps;
	int nextStepInd = 0;
	bool isSilent;

public:
	ReplayGame(bool silent);
	char getInput() override;
	
	void sleepFrame() override {
		if (!isSilent) Sleep(GAME_SPEED);

	}
};