#include "Recording_Game.h"
#include <iostream>
#include <string>
#include "Level_Loader.h"

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
		// If we are in getInteractionInput, we are usually answering a riddle.
		// We should record the raw key specifically as a riddle answer to avoid 
		// conflicts with player keys.
		int playerID = 0;
		if (key >= '1' && key <= '5') {
			recordedSteps.push_back(std::to_string(currentTick) + " " + std::to_string(playerID) + " " + key);
		}
		else {
			// Check if it was a player movement key (though usually not active during riddles)
			std::string cmd = translateKey(key, playerID);
			if (!cmd.empty()) {
				recordedSteps.push_back(std::to_string(currentTick) + " " + std::to_string(playerID) + " " + cmd);
			}
		}
	}
	return key;
}


std::string RecordingGame::translateKey(char key, int& playerID) {
	char upperKey = toupper(key);
	// we know the keys order
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
	std::string screenName = (levelInd < 3) ? SCREEN_FILES[levelInd] : "unknown";
	recordedEvents.push_back(std::to_string(currentTick) + " Level Changed: " + screenName);
}

void RecordingGame::onLifeLost(){
	recordedEvents.push_back(std::to_string(currentTick) + ": " + std::to_string(HP_INCREASE) + " HP lost");
}

void RecordingGame::onRiddleSolved(bool correct, const std::string& question, const std::string& answer) {
	std::string status = correct ? "Correct" : "Wrong";
	recordedEvents.push_back(std::to_string(currentTick) + " Riddle: " + question + " | Answer: " + answer + " | " + status);
}

void RecordingGame::writeResultsToBackup(const std::string& destName) {
	std::ofstream destination(destName);
	if (destination.is_open()) {
		// Write all historical and current events
		for (const auto& event : recordedEvents) {
			destination << event << "\n";
		}
		destination.close();
	}
}

void RecordingGame::writeStepsToBackup(const std::string& destName) {
	std::ofstream destination(destName); 
	if (destination.is_open()) {
		for (const auto& step : recordedSteps) {
			destination << step << "\n";
		}
		destination.close();
	}
}



void RecordingGame::saveGame() {
	std::string dataFilename = "savegame" + std::to_string(savefiles) + ".txt";
	std::string worldFilename = "world_state" + std::to_string(savefiles) + ".screen";
	std::string stepsBackupName = "saved_game" + std::to_string(savefiles) + ".steps";
	std::string resultsBackup = "saved_game" + std::to_string(savefiles) + ".result";

	std::ofstream saveFile(dataFilename);
	if (!saveFile.is_open()) return;

	auto now = std::chrono::steady_clock::now();
	auto totalElapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
	auto levelElapsed = std::chrono::duration_cast<std::chrono::seconds>(now - levelStartTime).count();

	saveFile << "CURRENT_LEVEL " << currentLevelIndex << "\n"; // save info and score
	saveFile << "SCORE " << score << "\n";
	saveFile << "TOTAL_TIMER " << totalElapsed << "\n";
	saveFile << "LEVEL_TIMER " << levelElapsed << "\n";
	for (int i = 0; i < PLAYER_AMOUNT; i++) { // save each player info
		saveFile << "PLAYER_" << i << "_HP " << players[i].getHP() << "\n";
		saveFile << "PLAYER_" << i << "_X " << players[i].getPos().x << "\n";
		saveFile << "PLAYER_" << i << "_Y " << players[i].getPos().y << "\n";

		heldItem currentItem = players[i].getItemInHand(); // Accessing itemInHand
		saveFile << "PLAYER_" << i << "_ITEM_TYPE " << static_cast<int>(currentItem.type) << "\n";
		saveFile << "PLAYER_" << i << "_ITEM_ID " << currentItem.id << "\n";
		saveFile << "PLAYER_" << i << "_ITEM_COLOR " << static_cast<int>(currentItem.color) << "\n";
	}
	saveFile << "Recorded_Game 1" << "\n"; // we not on game and on recording_game
	saveFile.close();

	Level_Loader::saveLevel(levels[currentLevelIndex], worldFilename, players[0].getPos(), players[1].getPos());

	std::string endMarker = std::to_string(currentTick) + " 0 END";
	recordedSteps.push_back(endMarker);

	writeStepsToBackup(stepsBackupName);
	writeResultsToBackup(resultsBackup);
	//we write backup as we might save recorded game and than record another game
	// and if we load the saved game the recorded steps and results will be lost

	recordedSteps.pop_back(); // remove end marker to continue recording if needed

	printCentered("GAME SAVED AND RECORDED SUCCESSFULLY TO SLOT " + std::to_string(savefiles + 1), 2);
	savefiles++; // new save file added
	saveGlobalSaveConfig(); // save on file how many saves are
	if (!isSilent) Sleep(1000);
}

bool RecordingGame::loadGame(int slot) {
	if (!Game::loadGame(slot)) return false; // load base game data

	std::string stepsBackupName = "saved_game" + std::to_string(slot) + ".steps"; // load backup steps of recorded file 
	std::ifstream stepsFile(stepsBackupName);

	if (stepsFile.is_open()) {
		recordedSteps.clear();
		// clearing any movement between saved and unsaved time
		// for example save game , played more on the game and than loaded back to save file
		std::string line;
		int maxTick = 0;

		while (std::getline(stepsFile, line)) { // get line
			if (line.empty() || line[0] == '#') 
				continue; // pass comments or empty lines
			if(line.find("END") != std::string::npos) 
				continue; // skip the END line to continue recording after loading
			recordedSteps.push_back(line); //add the valid step to record vector

			size_t firstSpace = line.find(' '); //;ocate the position of the first space to isolate the tick number
			if (firstSpace != std::string::npos) { // when space is found get the num of intarction
				int tick = std::stoi(line.substr(0, firstSpace)); // get the interaction
				maxTick = tick; // get new max tick 
			}
		}
		currentTick = maxTick; // the tick will be the max value to continoue the record 
		stepsFile.close();
	}
	std::string resultsBackupName = "saved_game" + std::to_string(slot) + ".result";
	std::ifstream resultsFile(resultsBackupName);
	if (resultsFile.is_open()) {
		recordedEvents.clear(); // Clear current session events
		std::string line;
		while (std::getline(resultsFile, line)) {
			// Do not import the "Final Score" or "Total Ticks" lines from the old save
			if (line.find("Final Score") != std::string::npos ||
				line.find("Total Ticks") != std::string::npos) continue;

			if (!line.empty()) {
				recordedEvents.push_back(line);
			}
		}
		resultsFile.close();
	}
	return true;
}

RecordingGame::~RecordingGame()
{
	
	if (recordedSteps.empty()) return; // If the vector is empty, do not overwrite files with empty data

	if (recordedSteps.back().find("END") == std::string::npos) { // chcek if the last line is not with END
		recordedSteps.push_back(std::to_string(currentTick) + " 0 END");
	}

	std::ofstream stepFile("adv-world.steps");
	if (stepFile.is_open()) {

		stepFile << "# ============================================\n";
		stepFile << "# Recorded Game Steps\n";
		stepFile << "# Screen Files: adv_world_01.screen, adv_world_02.screen, adv_world_03.screen\n";
		stepFile << "# Recorded: " << __DATE__ << " " << __TIME__ << "\n";
		stepFile << "# Format: <tick> <playerID> <command>\n";
		stepFile << "# ============================================\n";

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
		resultFile << currentTick << " Game Ended: Score " << score << "\n";

		resultFile.close();
	}
}