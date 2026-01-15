#include "Recording_Game.h"

static int getCurrentTime(std::chrono::steady_clock::time_point start) {
	auto now = std::chrono::steady_clock::now();
	return (int)std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
}

char RecordingGame::getInput()
{
	currentTick++;

	char key = Game::getInput();
	if (key != 0) {
		recordedSteps.push_back(std::to_string(currentTick) + " " + key);
	}
	return key;
}

void RecordingGame::onLevelChange(int levelInd){
	int time = getCurrentTime(startTime);
	recordedEvents.push_back(std::to_string(time) + " Level Changed: " + std::to_string(levelInd));
}

void RecordingGame::onLifeLost(){
	int time = getCurrentTime(startTime);
	recordedEvents.push_back(std::to_string(time) + ": " + std::to_string(HP_INCREASE) + " HP lost");
}

void RecordingGame::onRiddleSolved(bool correct) {
	int time = getCurrentTime(startTime);
	std::string status = correct ? "Correct" : "Wrong";
	recordedEvents.push_back(std::to_string(time) + " Riddle: " + status);
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
		for (const auto& line : recordedEvents) {
			resultFile << line << "\n";
		}

		auto totalTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - startTime).count();
		
		resultFile << "Final Score: " << score << "\n";
		resultFile << "Total Time: " << totalTime << "\n";
		resultFile.close();
	}
}