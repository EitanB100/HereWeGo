#include <iostream>
#include <sstream> 
#include <iomanip>  
#include <string>
#include "Game.h"
#include "Level_Loader.h"
#include "Tile_Chars.h"

char Game::p1Keys[KEY_COUNT] = { 'W','X','A','D','S','E' };
char Game::p2Keys[KEY_COUNT] = { 'I','M','J','L','K','O' };



void Game::resetLevelTimer() {
	levelStartTime = std::chrono::steady_clock::now();
}


Game::Game() : useColor(getColorMode()) {
	init();
	players[0] = Player(Placement(10, 20), '$', 0, 0, p1Keys);
	players[1] = Player(Placement(9, 15), '&', 0, 0, p2Keys);
}

void Game::handleGameOver()
{
	if (isSilent) return;
	screen.clearScreen();
	gotoxy(35, 10);
	setColor(Color::RED);

	printCentered("G A M E   O V E R", 10);
	printCentered("Press 'H' to exit to menu", 12);
	while (true) {
		if (_kbhit()) {
			char choice = _getch();
			if (choice == 'h' || choice == 'H') {
				// This exits Game::run and goes back to main()
				setColor(Color::WHITE);
				return;
			}
		}
	}
}

void Game::tileMapError()
{
	screen.clearScreen();
	setColor(Color::RED);
	printCentered("CRITICAL ERROR", 10);
	printCentered(loadingErrorMessage, 12);
	printCentered("Press any key to return to menu...", 14);
	setColor(Color::WHITE);
	_getch();
}

void Game::toggleColor() {
	useColor = !useColor;
	setColorMode(useColor);
}

void Game::startInLevel(Level level)
{
	if (!levelLoadedCorrectly || levels.empty()) {
		return;
	}
	currentLevelIndex = static_cast<int>(level);

	if (currentLevelIndex >= levels.size()) currentLevelIndex = 0;
	setGame(currentLevelIndex, false);
}

void Game::init()
{
	loadGlobalSaveConfig(); // collect how 
	Level_Loader::loadRiddles("riddles.txt", riddles);

	for (int i = 1; i <= LEVEL_COUNT; i++) {

		std::string currentFile = "adv_world_0" + std::to_string(i) + ".screen";

		std::ifstream fileCheck(currentFile);
		if (fileCheck.is_open()) {
			fileCheck.close();
			levels.emplace_back(); //create empty room in the vector (adds object to vector using its empty constructor)

			if (!Level_Loader::loadLevel(levels.back(), currentFile, loadingErrorMessage)) {
				loadingErrorMessage = "Level " + std::to_string(i) + " failed - " + loadingErrorMessage;
				levelLoadedCorrectly = false;
				levels.pop_back();
				return;
			}
		}
		else { //missing file or no more levels or level incorretly named
			break;
		}
	}


	if (levels.empty()) levelLoadedCorrectly = false;
}

void Game::saveGame() {
	std::string dataFilename = "savegame" + std::to_string(savefiles) + ".txt"; // dynamic file name for data file
	std::string worldFilename = "world_state" + std::to_string(savefiles) + ".screen"; // dynamic file name for map load file

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
	saveFile << "Recorded_Game 0" << "\n"; // we not on game and not on recording_game
	saveFile.close();
	Level_Loader::saveLevel(levels[currentLevelIndex], worldFilename, players[0].getPos(), players[1].getPos());
	printCentered("GAME SAVED SUCCESSFULLY TO SLOT " + std::to_string(savefiles + 1), 2);
	savefiles++; // new save file added
	saveGlobalSaveConfig(); // save on file how many saves are

	if (!isSilent) Sleep(1000);
}

bool Game::loadGame(int slot) {
	std::string filename = "savegame" + std::to_string(slot) + ".txt";
	std::string worldFilename = "world_state" + std::to_string(slot) + ".screen";
	std::ifstream file(filename);

	if (!file.is_open()) return false;

	std::string key;
	while (file >> key) {
		if (key == "CURRENT_LEVEL") {
			file >> currentLevelIndex;
		}
		else if (key == "SCORE") {
			file >> score;
		}
		else if (key == "TOTAL_TIMER") {
			long long sec; file >> sec;
			startTime = std::chrono::steady_clock::now() - std::chrono::seconds(sec);
		}
		else if (key == "LEVEL_TIMER") {
			long long sec; file >> sec;
			levelStartTime = std::chrono::steady_clock::now() - std::chrono::seconds(sec);
		}
		else if (key.find("PLAYER_") != std::string::npos) { // if the key is on Player_<x> found 
			int pIdx = key[7] - '0'; // we getting 0 or 1 as int with casting
			if (pIdx < 0 || pIdx >= PLAYER_AMOUNT) 
				continue;

			heldItem currentItem = players[pIdx].getItemInHand();

			if (key.find("_HP") != std::string::npos) { // hp found
				int hp; file >> hp; players[pIdx].setHP(hp);
			}
			else if (key.find("_X") != std::string::npos) { // x possiton found
				int x; file >> x;
				Point p = players[pIdx].getPos();
				players[pIdx].setPos({ x, p.y });
			}
			else if (key.find("_Y") != std::string::npos) { //y axis possion found
				int y; file >> y;
				Point p = players[pIdx].getPos();
				players[pIdx].setPos({ p.x, y });
			}
			else if (key.find("_ITEM_TYPE") != std::string::npos) {
				int type; file >> type;
				currentItem.type = static_cast<ItemType>(type);
				players[pIdx].setItemInHand(currentItem);
			}
			else if (key.find("_ITEM_ID") != std::string::npos) {
				int id; file >> id;
				currentItem.id = id; // Here we restore the ID
				players[pIdx].setItemInHand(currentItem);
			}
			else if (key.find("_ITEM_COLOR") != std::string::npos) {
				int color; file >> color;
				currentItem.color = static_cast<Color>(color); // Here we restore the Color
				players[pIdx].setItemInHand(currentItem);
			}
		}
		else if (key == "Recorded_Game") {
			int val; file >> val;
		}
	}
	file.close();

	std::string error;
	levelLoadedCorrectly = Level_Loader::loadLevel(levels[currentLevelIndex], worldFilename,error);
	return levelLoadedCorrectly;
}

bool Game::isSlotRecorded(int slot) {
	std::string filename = "savegame" + std::to_string(slot) + ".txt";
	std::ifstream file(filename);
	if (!file.is_open()) return false;

	std::string line;
	while (std::getline(file, line)) {
		if (line.find("Recorded_Game 1") != std::string::npos) {
			return true;
		}
	}
	return false;
}

void Game::saveGlobalSaveConfig() {
	std::ofstream configFile("config.txt");
	if (configFile.is_open()) {
		configFile << savefiles << "\n";

		configFile << "[KEYS]\n";
		configFile << "P1";
		for (int i = 0; i < KEY_COUNT; i++) {
			configFile << " " << p1Keys[i];
		}

		configFile << "\nP2";
		for (int i = 0; i < KEY_COUNT; i++) {
			configFile << " " << p2Keys[i];
		}

		configFile << "\n";
		configFile.close();
	}
}

void Game::loadGlobalSaveConfig() {
	std::ifstream configFile("config.txt");
	if (configFile.is_open()) {
		if (!(configFile >> savefiles)) {
			savefiles = 0; // Fallback if file is corrupted or empty
		}

		std::string line;
		while (std::getline(configFile, line)) {
			if (line.empty()) continue;
			if (line == "[KEYS]") continue;

			std::stringstream parser(line);
			std::string player;
			if (!(parser >> player)) continue;

			if (player == "P1") {
				for (int i = 0; i < KEY_COUNT; i++) {
					char key;
					if (parser >> key) {
						p1Keys[i] = key;
					}
				}
			}

			//run 1 - original buttons 
			//run 2 - changed buttons
			else if (player == "P2") {
				for (int i = 0; i < KEY_COUNT; i++) {
					char key;
					if (parser >> key) {
						p2Keys[i] = key;
					}
				}
			}
		}

		configFile.close();
	}
	else {
		savefiles = 0; // File doesn't exist yet (first time running the game)
	}
}

void Game::setGame(int levelIndex, bool firstSettings) {
	if (levels.empty()) return;

	currentLevelIndex = levelIndex;



	players[0].setPos(levels[currentLevelIndex].getP1Start());
	players[1].setPos(levels[currentLevelIndex].getP2Start());

	for (auto& player : players) {
		player.setDirection(Directions::STAY);
		player.setFinished(false);
	}

	if (!isSilent) {
		screen.clearScreen();
		levels[currentLevelIndex].drawRoom(screen);
		if (!firstSettings) {
			screen.draw();
			levels[currentLevelIndex].drawTopLayer(); //already in not silent
		}

		for (auto& player : players) {
			player.draw();
		}
	}
}


char Game::getInteractionInput()
{
	while (true) {
		if (_kbhit()) return _getch();
	}
}

char Game::getInput()
{
	if (_kbhit()) return _getch();
	return 0;
}

void Game::updateGameLogic(char key, Room& currRoom, bool& boomDustCleaningNeeded, bool isSilent) { 
	currRoom.setSilent(isSilent);

	currRoom.resetObstacles();
	Point currentExitPoint = currRoom.getExitPos();

	//update loop
	for (int i = 0; i < PLAYER_AMOUNT; i++) {
		Player& p = players[i];
		p.setSilent(isSilent);
		Player& other = players[1 - i];

		p.inputManager(key, currRoom);
		if (!isSilent) setColor(Color::WHITE);

		p.updateSpringPhysics(currRoom, &other);

		int eventID = p.move(currRoom, &other);

		if (eventID != 0) {
			handleRiddle(eventID, p, currRoom);
		}

		//check level completion for a player
		if (currentExitPoint.x != -1 && p.getPos() == currentExitPoint) {
			if (!p.isFinished()) {
				p.setFinished(true);
				if (!isSilent) {
					gotoxy(50, 0);
					std::cout << "Player " << p.getSymbol() << " Is waiting...";
				}
			}
		}
	}
	if (boomDustCleaningNeeded) {
		currRoom.clearExplosions();
		redrawScreen(currRoom, isSilent);
		boomDustCleaningNeeded = false;
	}

	currRoom.updateBombs(players, PLAYER_AMOUNT, screen); // update bombs

	if (currRoom.hasExplosions()) {
		boomDustCleaningNeeded = true;
		redrawScreen(currRoom, isSilent);
	}

}

void Game::redrawScreen(Room& currRoom, bool isSilent) {
	currRoom.drawRoom(screen);
	if (!isSilent) screen.draw();
	currRoom.drawTopLayer();
}

void Game::run()
{
	if (!levelLoadedCorrectly) {
		tileMapError();
		return; // Exits run(), returning to main() menu
	}

	// Only initialize for a brand new game
	if (score == 0 && currentLevelIndex == 0 &&
		startTime.time_since_epoch().count() == 0) {
		startTime = std::chrono::steady_clock::now();
		levelStartTime = startTime;
	}

	bool boomDustCleaningNeeded = false;
	while (true) {
		Room& currRoom = levels[currentLevelIndex];
		char key = getInput();

		if (key == ESC) {
			auto pauseStart = std::chrono::steady_clock::now();

			setColor(Color::BLUE);
			printCentered("GAME PAUSED", 2);
			printCentered("Press H to exit", 4);
			printCentered("Press S to save", 6);

			char menuChoice = _getch();

			if (menuChoice == 'h' || menuChoice == 'H') {
				setColor(Color::WHITE);
				screen.clearScreen();
				break;
			}
			if (menuChoice == 's' || menuChoice == 'S') {
				setColor(Color::WHITE);
				screen.clearScreen();
				saveGame();
				Sleep(1000);
				break;
			}
			else {
				auto pauseEnd = std::chrono::steady_clock::now();
				auto pauseDuration = pauseEnd - pauseStart;
				levelStartTime += pauseDuration;
				startTime += pauseDuration;

				setColor(Color::WHITE);
				redrawScreen(currRoom, false);
				for (int i = 0; i < PLAYER_AMOUNT; i++) {
					players[i].draw();
				}
				key = 0; // reset key after handling pause
			}

		}


		updateGameLogic(key, currRoom, boomDustCleaningNeeded, isSilent);

		bool isVictory = checkLevelTransition(currentLevelIndex, players[0].getPos(), players[1].getPos());
		if (isVictory) break;

		bool gameOver = false;
		for (int i = 0; i < PLAYER_AMOUNT; i++) {
			if (!isSilent)
				players[i].draw();
			if (players[i].isDead()) {
				gameOver = true;
				break;
			}
		}

		if (gameOver) {
			handleGameOver();
			break;
		}
		//HUD renderer
		if (!isSilent) {
			printHUD();
			printTimer();
		}
		Sleep(GAME_SPEED);
	}

}


bool Game::checkLevelTransition(int& currentLevelIndex, Point p1, Point p2)
{
	Point exit = levels[currentLevelIndex].getExitPos();
	if (exit.x == -1) return false;

	if (p1 == exit && p2 == exit)
	{
		auto now = std::chrono::steady_clock::now();
		int levelSeconds = std::chrono::duration_cast<std::chrono::seconds>(now - levelStartTime).count();
		score += (MAX_SCORE / (levelSeconds + 1));

		int destination = levels[currentLevelIndex].getExitDestination();

		if (destination == -1) destination = currentLevelIndex + 1;

		if (destination < levels.size()) {
			currentLevelIndex = destination;
			onLevelChange(currentLevelIndex);
			setGame(currentLevelIndex, false);
			resetLevelTimer();
			if (!isSilent) printTimer();
			return false;
		}
		else {
			showEndingScreen();
			return true;
		}

	}

	return false;
}


void Game::drawGameFrame(Room& currRoom)
{
	currRoom.drawRoom(screen);
	screen.draw();
	currRoom.drawTopLayer();

	for (int i = 0; i < PLAYER_AMOUNT; i++) players[i].draw();

	printHUD();
	printTimer();
}

void Game::handleRiddle(int riddleID, Player& player, Room& room)
{
	Riddle* currentRiddle = nullptr;

	for (int i = 0; i < riddles.size(); i++) {
		if (riddles[i].id == riddleID) {
			currentRiddle = &riddles[i];
			break;
		}
	}

	if (currentRiddle == nullptr) return;

	const Riddle& riddle = *currentRiddle; // for readability

	if (!isSilent) {
		screen.clearScreen();
		setColor(Color::CYAN);
		printCentered("=== RIDDLE TIME! ===", 5);
		setColor(Color::WHITE);
		printCentered(riddle.question, 8);

		for (int i = 0; i < riddle.options.size(); i++) {
			std::string currOption = "(" + std::to_string(i + 1) + ") " + riddle.options[i];
			printCentered(currOption, 11 + i * 2);
		}
	}
	while (_kbhit()) _getch();

	while (true) {
		char c = getInteractionInput();

		if (c < '0' + MIN_RIDDLE_CHOICE || c > '0' + MAX_RIDDLE_CHOICE) continue; //maximum 5 options 
		int choice = c - '0';
		if (choice - 1 == riddle.correctAnswer) {


			onRiddleSolved(true, currentRiddle->question, std::to_string(choice));
			if (!isSilent) {
				setColor(Color::GREEN);
				printCentered("CORRECT!", 20);
				Sleep(500);
			}

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
			onRiddleSolved(false, currentRiddle->question, std::to_string(choice));
			if (!isSilent) {
				setColor(Color::RED);
				printCentered("WRONG! -" + std::to_string(HP_INCREASE) + " HP •`_´•", 20);
				Sleep(500);
			}
			player.takeDamage(HP_INCREASE);
			onLifeLost();
		}
		break;
	}
	if (!isSilent) {
		setColor(Color::WHITE);
		screen.clearScreen();
		room.drawRoom(screen);
		screen.draw();
		room.drawTopLayer();
	}
}


void Game::showEndingScreen()
{
	if (isSilent) return;
	screen.clearScreen();

	setColor(Color::CYAN);
	printCentered("===============================================", 3);
	printCentered("||                                           ||", 4);
	printCentered("||           THANKS FOR PLAYING!             ||", 5);
	printCentered("||                                           ||", 6);
	printCentered("===============================================", 7);


	setColor(Color::YELLOW);
	printCentered("       $             &       ", 10);
	setColor(Color::MAGENTA);
	printCentered("    (We made it!)     ", 12);

	setColor(Color::GREEN);
	std::string finalScoreStr = "FINAL SCORE: " + std::to_string(score);

	//Rank based on score
	std::string rank = "Rank: Novice Explorer";
	if (score > 1000) rank = "Rank: Dungeon Master";
	if (score > 5000) rank = "Rank: C++ Wizard";

	printCentered(finalScoreStr, 14);
	setColor(Color::LIGHT_GRAY);
	printCentered(rank, 15);

	setColor(Color::WHITE);

	printCentered("GAME CREATED BY", 20);
	printCentered("EITAN BAR", 21);
	printCentered("HAREL BEN-ABIR", 22);

	setColor(Color::DARK_GRAY);
	printCentered("Press any key to return to menu...", 24);

	while (_kbhit()) _getch();
	_getch();

	setColor(Color::WHITE); // Reset for menu

}
