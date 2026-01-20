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
	std::string translateKey(char key, int& playerID);
	void resetRecording() override;

	void onLevelChange(int levelInd) override;
	void onLifeLost() override;
	void onRiddleSolved(bool correct, const std::string& question, const std::string& answer) override;
	void writeStepsToBackup(const std::string& destName); // basiclly copy the steps of the recording to a file
	void writeResultsToBackup(const std::string& destName); // same but for results
	void saveGame() override;
	bool loadGame(int slot) override;
	~RecordingGame(); 
	
};