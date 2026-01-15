#include "ReplayGame.h"



ReplayGame::ReplayGame(bool silent)
{
	isSilent = silent;

	std::ifstream inFile("adv-world.steps");

	if (inFile.is_open()) {
		int t;
		char k;
		while (inFile >> t >> k) {
			steps.push_back({ t,k });
		}
		inFile.close();
	}
	if (isSilent) {
		loadExpectedResult();
	}
}

ReplayGame::~ReplayGame()
{
	if (isSilent) {
		std::cout << "\n=== TEST VALIDATION ===" << std::endl;
		bool passed = true;

		if (actualEvents.size() != expectedEvents.size()) {
			std::cout << "FAIL: Event count mismatch. Expected "
				<< expectedEvents.size() << ", got " << actualEvents.size() << std::endl;
			passed = false;
		}

		size_t minSize = (std::min)(actualEvents.size(), expectedEvents.size());
		for (size_t i = 0; i < minSize; i++) {
			if (actualEvents[i].description != expectedEvents[i].description) {
				std::cout << "FAIL at event " << i << ":" << std::endl;
				std::cout << "  Expected: " << expectedEvents[i].description << std::endl;
				std::cout << "  Actual:   " << actualEvents[i].description << std::endl;
				passed = false;
			}
		}

		if (passed) {
			std::cout << "TEST PASSED!" << std::endl;
		}
		else {
			std::cout << "TEST FAILED" << std::endl;
		}
	}

}

void ReplayGame::loadExpectedResult()
{
	std::ifstream inFile("adv-world.result");
	if (!inFile.is_open()) return;

	std::string line;
	while (std::getline(inFile, line)) {
		if (line.empty()) continue;

		int spacePos = line.find(' ');
		if (spacePos != std::string::npos) {
			int time = std::stoi(line.substr(0, spacePos));
			std::string description = line.substr(spacePos + 1);
			expectedEvents.push_back({ time,description });
		}
	}
	inFile.close();
}

void ReplayGame::recordActualEvent(int time, const std::string& description)
{
	actualEvents.push_back({ time,description });
}

char ReplayGame::getInput()
{
	auto now = std::chrono::steady_clock::now();
	int diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - levelStartTime).count();
	int currentTick = static_cast<int>(diff / GAME_SPEED);

	// Check if we have a step recorded for this time (or passed it)
	if (nextStepInd < steps.size() && steps[nextStepInd].tick <= currentTick) {
		char keyToPress = steps[nextStepInd].key;
		nextStepInd++;
		return keyToPress;
	}
	return 0; // No input right now
}

void ReplayGame::onLevelChange(int levelInd)
{
	auto now = std::chrono::steady_clock::now();
	int time = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
	recordActualEvent(time, "Level Changed: " + std::to_string(levelInd));
}

void ReplayGame::onLifeLost()
{
	auto now = std::chrono::steady_clock::now();
	int time = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
	recordActualEvent(time, ": " + std::to_string(HP_INCREASE) + " HP lost");
}

void ReplayGame::onRiddleSolved(bool correct)
{
	auto now = std::chrono::steady_clock::now();
	int time = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
	std::string status = correct ? "Correct" : "Wrong";
	recordActualEvent(time, "Riddle: " + status);
}


