#pragma once
#include "Game.h"
#include <fstream>
#include <vector>
#include <string>

class RecordingGame : public Game {
	std::vector<std::string> recordedSteps;
	std::vector<std::string> recordedEvents;
	int currentTick = 0;
public:
	char getInput() override;
	char getInteractionInput() override;
	
	void onLevelChange(int levelInd) override;
	void onLifeLost() override;
	void onRiddleSolved(bool correct) override;
	
	~RecordingGame(); 

};