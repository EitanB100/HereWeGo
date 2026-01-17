#include "Recording_Game.h"
#include <iostream>
#include <string>

static int getCurrentTime(std::chrono::steady_clock::time_point start) {
	auto now = std::chrono::steady_clock::now();
	return (int)std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
}
char RecordingGame::getInput() {
    currentTick++; 
    char key = 0;

    if (_kbhit()) {
        key = _getch();
        if (key != 0 && key != 27) { // Ignore null and ESC
            int playerID = 0;
            std::string cmd = translateKey(key, playerID);
            
            if (!cmd.empty()) {
                // This captures movement: "5 1 UP"
                recordedSteps.push_back(std::to_string(currentTick) + " " + 
                                        std::to_string(playerID) + " " + cmd);
            }
        }
    }
    return key;
}

char RecordingGame::getInteractionInput() {
	char key = Game::getInteractionInput();
	if (key != 0) {
		int playerID = 0;
		std::string cmd = translateKey(key, playerID);

		if (!cmd.empty()) { // if its a movement, record the command
			recordedSteps.push_back(std::to_string(currentTick) + " " +std::to_string(playerID) + " " + cmd);
		}
		
		else if (key >= '1' && key <= '5') { // if its a riddle answer (1-5), record the raw character
			recordedSteps.push_back(std::to_string(currentTick) + " 0 " + key); //we use '0' for playerID or you can determine it as riddle
		}
	}
	return key;
}

std::string RecordingGame::translateKey(char key, int& playerID) {
	char upperKey = toupper(key);

	// Assuming p1Keys[0] = UP, [1] = DISPOSE, [2] = LEFT, [3] = RIGHT, [4] = DOWN, [5] = STAY

	if (upperKey == p1Keys[0]) { playerID = 1; return "UP"; }
	if (upperKey == p1Keys[1]) { playerID = 1; return "DOWN"; }
	if (upperKey == p1Keys[2]) { playerID = 1; return "LEFT"; }
	if (upperKey == p1Keys[3]) { playerID = 1; return "RIGHT"; }
	if (upperKey == p1Keys[4]) { playerID = 1; return "STAY"; }
	if (upperKey == p1Keys[5]) { playerID = 1; return "DISPOSE"; }

	if (upperKey == p2Keys[0]) { playerID = 2;  return "UP"; }
	if (upperKey == p2Keys[1]) { playerID = 2;  return "DOWN"; }
	if (upperKey == p2Keys[2]) { playerID = 2;  return "LEFT"; }
	if (upperKey == p2Keys[3]) { playerID = 2;  return "RIGHT"; }
	if (upperKey == p2Keys[4]) { playerID = 2;  return "STAY"; }
	if (upperKey == p2Keys[5]) { playerID = 2;  return "DISPOSE"; }

	return "";
}

void RecordingGame::resetRecording()
{
	recordedSteps.clear();
	recordedEvents.clear();
	currentTick = 0;
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