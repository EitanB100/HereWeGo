#pragma once
#include "Game.h"
#include <fstream>
#include <vector>
#include <string>

class RecordingGame : public Game {
	std::vector<std::string> recordedSteps;
public:
	char getInput() override {
		char key = Game::getInput();
		if (key != 0) {
			auto now = std::chrono::steady_clock::now();
			int diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - levelStartTime).count();
			int currentTick = diff / GAME_SPEED;
			recordedSteps.push_back(std::to_string(currentTick) + " " + key);
		}
		return key;
	}
	~RecordingGame() {} // what to do here? lol

};