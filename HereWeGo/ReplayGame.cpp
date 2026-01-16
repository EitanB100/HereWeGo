#include "ReplayGame.h"
#include <sstream>


ReplayGame::ReplayGame(bool silent)
{
	this->isSilent = silent;

	std::ifstream inFile("adv-world.steps");

	if (inFile.is_open()) {  
		std::string line;
		while (std::getline(inFile, line)) {
			if (line.empty()) continue;

			std::stringstream parser(line);
			int t;
			char k;
			std::string interaction;
			if (parser >> t >> k) {
				bool isInteractable = (parser >> interaction && interaction == "i");
				steps.push_back({ t,k,isInteractable });
			}
		}
		inFile.close();
	}
	if (isSilent) {
		loadExpectedResult();
	}
	else {
		hideCursor();
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
		
		std::stringstream parser(line);
		int time;
		if (!(parser >> time)) continue;

		std::string description;
		std::getline(parser, description);

		if (!description.empty() && description[0] == ' ') {
			description = description.substr(1);
		}
		expectedEvents.push_back({ time,description });
	}
		
	inFile.close();
}

void ReplayGame::recordActualEvent(int time, const std::string& description)
{
	actualEvents.push_back({ time,description });
}

char ReplayGame::getInput()
{
	currentTick++;  // Increment every game loop call

	if (nextStepInd < steps.size() && steps[nextStepInd].tick <= currentTick) {
		if (!steps[nextStepInd].isInteraction) {
			return steps[nextStepInd++].key;
		}
	}
	return 0;

}

char ReplayGame::getInteractionInput()
{
	if (nextStepInd < steps.size() && steps[nextStepInd].tick == currentTick) {
		if (steps[nextStepInd].isInteraction)
			return steps[nextStepInd++].key;
	}
	return 0;
}

void ReplayGame::handleRiddle(int riddleID, Player& player, Room& room) //temporary. need full riddle file to write to
{
	// Logic split: Silent mode (simulation) vs Visual mode (base class logic)
	if (isSilent) {
		Riddle* currentRiddle = nullptr;
		for (auto& r : riddles) if (r.id == riddleID) currentRiddle = &r;

		if (!currentRiddle) return;

		// Simulate the loop without printing
		while (true) {
			char c = getInteractionInput();
			if (c == 0) break;
			if (c < '1' || c > '5') continue;

			int choice = c - '0';
			bool correct = (choice - 1 == currentRiddle->correctAnswer);

			onRiddleSolved(correct); // Log result

			if (correct) {
				// Logic to remove riddle (copied from Game::handleRiddle logic)
				Point p = player.getPos();
				Point neighbors[4] = { {p.x + 1,p.y},{p.x - 1,p.y},{p.x,p.y + 1},{p.x,p.y - 1} };
				for (const auto& neighbor : neighbors) {
					if (room.getObjectAt(neighbor) == RIDDLE_TILE && room.getRiddleID(neighbor) == riddleID) {
						room.removeRiddle(neighbor);
						break;
					}
				}
			}
			else {
				onLifeLost(); // Log life lost
				player.takeDamage(HP_INCREASE);
			}
			break; // Exit loop after processing answer
		}
	}
	else {
		// Not silent? Run normal visual logic.
		// It will calls our getInteractionInput() to get the recorded keys.
		Game::handleRiddle(riddleID, player, room);
	}
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


