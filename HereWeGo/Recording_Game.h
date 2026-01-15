#pragma once
#include "Game.h"
#include "Utils.h"
class RecordingGame : public Game {
	std::vector<std::string> recordedSteps;
public:
	char getInput() override {
		char key = Game::getInput();
		if (key != 0) {
			recordedSteps.push_back(std::to_string(levelTimer) + " " + key);
		}
		return key;
	}
	~RecordingGame() {} // what to do here? lol

};