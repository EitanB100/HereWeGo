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
