#include "Recording_Game.h"

char RecordingGame::getInput()
{
	char key = Game::getInput();
	if (key != 0) {
		auto now = std::chrono::steady_clock::now();
		int diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - levelStartTime).count();
		int currentTick = diff / GAME_SPEED;
		recordedSteps.push_back(std::to_string(currentTick) + " " + key);
	}
	return key;
}

RecordingGame::~RecordingGame()
{
	std::ofstream stepFile("adv-world.steps");
	if (stepFile.is_open()) {
		for (const auto& line : recordedSteps) {
			stepFile << line << "\n";
		}
		stepFile.close();
	}

	std::ofstream resultFile("adv-world.result");
	if (resultFile.is_open()) {
		auto totalTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - startTime).count();
		// Format: [Final Score] [Total Time]
		resultFile << score << " " << totalTime << "\n";
		resultFile.close();
	}
}