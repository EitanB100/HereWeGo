#pragma once
#include "Game.h"
#include <fstream>
#include <vector>
#include <string>

class RecordingGame : public Game {
	std::vector<std::string> recordedSteps;
public:
	char getInput() override;
		
	
	~RecordingGame(); 

};