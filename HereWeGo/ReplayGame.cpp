#include "ReplayGame.h"
#include <sstream>


ReplayGame::ReplayGame(bool silent) {
	this->isSilent = silent;
	this->isLoadMode = true;

	std::ifstream inFile("adv-world.steps");
	if (inFile.is_open()) {
		std::string line;
		int step, playerID;
		std::string command;

		while (inFile >> step >> playerID >> command) { // While we can successfully read 3 pieces of data...
			steps.push_back({ step, playerID, command }); // Push them into our vector
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

void ReplayGame::run() {
	if (!levelLoadedCorrectly) {
		tileMapError();
		return;
	}

	bool boomDustCleaningNeeded = false;
	currentTick = 0; // Reset tick for replay
	nextStepInd = 0; // Reset vector pointer
	startTime = std::chrono::steady_clock::now();
	levelStartTime = startTime;
	while (true) {
		Room& currRoom = levels[currentLevelIndex];

		char key = getInput();
		updateGameLogic(key, currRoom, boomDustCleaningNeeded);

		bool isVictory = checkLevelTransition(currentLevelIndex, players[0].getPos(), players[1].getPos()); //check Victory Nature
		if (isVictory) break;


		bool gameOver = false; // check Death Nature
		for (int i = 0; i < PLAYER_AMOUNT; i++) {
			if (players[i].isDead()) {
				gameOver = true;
				break;
			}
		}

		if (gameOver) {
			if (!isSilent) handleGameOver(); // we don't want the "You Lost" menu in silent tests
			break;
		}

		if (!isSilent) {
			// only draw and wait if the user is actually watching the replay
			for (int i = 0; i < PLAYER_AMOUNT; i++)
				players[i].draw();
			printHUD();
			printTimer();
			Sleep(REPLAY_SPEED);
		}
		else {
			// In SILENT mode
		}

		if (nextStepInd >= steps.size()) {
			break;
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

char ReplayGame::getInput() {
	currentTick++; // incresment every intraction

	if (nextStepInd < steps.size() && steps[nextStepInd].tick <= currentTick) {
		const std::string& cmd = steps[nextStepInd].command;
		int playerID = steps[nextStepInd].playerID; // use the stored playerID
		bool isRiddleAns = (cmd.length() == 1 && cmd[0] >= '1' && cmd[0] <= '5'); // identify if this step is a riddle answer (1-5)
		
		if (!isRiddleAns) { // only handle movement/dispose; skip riddle answers
			char key = getCharFromCommand(playerID, cmd); // pass the pID here
			nextStepInd++;
			return key;
		}
	}
	return 0;
}

char ReplayGame::getInteractionInput() {
	if (nextStepInd < steps.size() && steps[nextStepInd].tick <= currentTick) {
		const std::string& cmd = steps[nextStepInd].command;
		if (cmd.length() == 1 && cmd[0] >= '1' && cmd[0] <= '5') { // only handle if it's a riddle answer 
			char key = cmd[0]; // the command is already the key '1', '2', etc.
			nextStepInd++;    // move to next step
			return key;
		}
	}
	return 0;
}


char ReplayGame::getCharFromCommand(int playerID, const std::string& command) {
	if (command.length() == 1 && command[0] >= '1' && command[0] <= '5') { // if a riddle answerd 
		return command[0];
	}
	auto& keys = (playerID == 1) ? p1Keys : p2Keys;

	if (command == "UP") 
		return keys[0];
	if (command == "DOWN")    
		return keys[1];
	if (command == "LEFT")    
		return keys[2];
	if (command == "RIGHT")   
		return keys[3];
	if (command == "STAY")    
		return keys[4];
	if (command == "DISPOSE") 
		return keys[5];

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
	levelStartTick = currentTick;
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

void ReplayGame::printTimer() { // helped by AI
	const Point& hudPos = levels[currentLevelIndex].getLegendLoc();
	gotoxy(hudPos.x, hudPos.y + 2);

	// Calculate logical elapsed time based on ticks
	// Formula: (Current Tick * Delay between frames in ms) / 1000
	// If your original game used a 50ms Sleep, use 50 here.
	long totalSeconds = (currentTick * 100) / 1000;
	long levelSeconds = ((currentTick - levelStartTick) * 100) / 1000;
	// For Replay, Level Time and Total Time usually start together from the save point,
	// but you can adjust this if you track 'levelStartTick'.
	auto formatTime = [](long totalSecs) -> std::string {
		int m = static_cast<int>((totalSecs % 3600) / 60);
		int s = static_cast<int>(totalSecs % 60);
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(2) << m << ":"
			<< std::setfill('0') << std::setw(2) << s;
		return ss.str();
		};
	// Display based on logic ticks, not the system clock
	std::cout << "LEVEL " << formatTime(levelSeconds) // Or specific level ticks
		<< " | TOTAL " << formatTime(totalSeconds) << " |";
}

