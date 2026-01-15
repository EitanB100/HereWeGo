#include <iostream>
#include <sstream> 
#include <iomanip>  
#include <string>
#include "Game.h"
#include "Level_Loader.h"
#include "Tile_Chars.h"

char Game::p1Keys[KEY_COUNT] = {'W','X','A','D','S','E'};
char Game::p2Keys[KEY_COUNT] = { 'I','M','J','L','K','O' };

void Game::resetLevelTimer() {
	levelStartTime = std::chrono::steady_clock::now();
}

Game::Game() : useColor(getColorMode()), players{
	Player(Placement(10,20),'$',0,0,p1Keys),
	Player(Placement(9,15),'&',0,0,p2Keys)
}
{
	init();
}



void Game::handleGameOver()
{
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

void Game::toggleColor(){ 
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
	saveFile.close();
	Level_Loader::saveLevel(levels[currentLevelIndex], worldFilename, players[0].getPos(), players[1].getPos());
	printCentered("GAME SAVED SUCCESSFULLY TO SLOT " + std::to_string(savefiles+1), 2);
	savefiles++; // new save file added
	saveGlobalSaveConfig(); // save on file how many saves are
	Sleep(1000);
}

bool Game::loadGame(int slot) {
	std::string dataFilename = "savegame" + std::to_string(slot) + ".txt";
	std::string worldFilename = "world_state" + std::to_string(slot) + ".screen";
	std::ifstream saveFile(dataFilename);
	if (!saveFile.is_open()) return false;

	long totalTimeSaved = 0;
	long levelTimeSaved = 0;

	struct TempPlayer {
		int hp = 15, x = 0, y = 0, itemType = 0, itemId = -1, itemColor = 0;
	} temp[2];

	std::string key;
	// Read the key first, then decide how to read the value
	while (saveFile >> key) {
		if (key == "CURRENT_LEVEL") saveFile >> currentLevelIndex;
		else if (key == "SCORE") saveFile >> score;
		else if (key == "TOTAL_TIMER") saveFile >> totalTimeSaved;
		else if (key == "LEVEL_TIMER") saveFile >> levelTimeSaved;
		else if (key == "PLAYER_0_HP") saveFile >> temp[0].hp;
		else if (key == "PLAYER_0_X")  saveFile >> temp[0].x;
		else if (key == "PLAYER_0_Y")  saveFile >> temp[0].y;
		else if (key == "PLAYER_0_ITEM_TYPE") saveFile >> temp[0].itemType;
		else if (key == "PLAYER_0_ITEM_ID")   saveFile >> temp[0].itemId;
		else if (key == "PLAYER_0_ITEM_COLOR") saveFile >> temp[0].itemColor;
		else if (key == "PLAYER_1_HP") saveFile >> temp[1].hp;
		else if (key == "PLAYER_1_X")  saveFile >> temp[1].x;
		else if (key == "PLAYER_1_Y")  saveFile >> temp[1].y;
		else if (key == "PLAYER_1_ITEM_TYPE") saveFile >> temp[1].itemType;
		else if (key == "PLAYER_1_ITEM_ID")   saveFile >> temp[1].itemId;
		else if (key == "PLAYER_1_ITEM_COLOR") saveFile >> temp[1].itemColor;
	}
	saveFile.close();

	std::string error;

	if (Level_Loader::loadLevel(levels[currentLevelIndex], worldFilename, error)) {
		setGame(currentLevelIndex, false);

		// ADJUST THE START TIME: 
		auto now = std::chrono::steady_clock::now();
		startTime = now - std::chrono::seconds(totalTimeSaved);
		levelStartTime = now - std::chrono::seconds(levelTimeSaved);

		for (int i = 0; i < PLAYER_AMOUNT; i++) {
			players[i].setHP(temp[i].hp);
			players[i].setPos({ temp[i].x, temp[i].y });

			// Reconstruct the heldItem struct
			heldItem restoredItem;
			restoredItem.type = static_cast<ItemType>(temp[i].itemType);
			restoredItem.id = temp[i].itemId;
			restoredItem.color = static_cast<Color>(temp[i].itemColor);
			players[i].setItemInHand(restoredItem);
		}
		return true;
	}
	return false;
}

void Game::saveGlobalSaveConfig() {
	std::ofstream configFile("config.txt");
	if (configFile.is_open()) {
		configFile << savefiles;
		configFile.close();
	}
}

void Game::loadGlobalSaveConfig() {
	std::ifstream configFile("config.txt");
	if (configFile.is_open()) {
		if (!(configFile >> savefiles)) {
			savefiles = 0; // Fallback if file is corrupted or empty
		}
		configFile.close();
	}
	else {
		savefiles = 0; // File doesn't exist yet (first time running the game)
	}
}

void Game::setGame(int levelIndex, bool firstSettings) {
	if (levels.empty()) return;

	screen.clearScreen();
	currentLevelIndex = levelIndex;

	levels[currentLevelIndex].drawRoom(screen);
	if (!firstSettings) {
		screen.draw();
		levels[currentLevelIndex].drawTopLayer();
	}

	players[0].setPos(levels[currentLevelIndex].getP1Start());
	players[1].setPos(levels[currentLevelIndex].getP2Start());

	for (auto& player : players) {
		player.setDirection(Directions::STAY);
		player.setFinished(false);
		player.draw();
	}
}


char Game::getInput()
{
	if (_kbhit()) return _getch();
	return 0;
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
		
		if (key != 0) {
			if (key == ESC) {
				auto pauseStart = std::chrono::steady_clock::now();

				setColor(Color::BLUE);
				printCentered("GAME PAUSED", 2);
				printCentered("Press H to exit", 4);
				printCentered("Press S to save", 6);
				key = _getch();

				if (key == 'h' || key == 'H') {
					setColor(Color::WHITE);
					screen.clearScreen();
					break; //main menu exit
				}
				if (key == 's' || key == 'S') {
					setColor(Color::WHITE);
					screen.clearScreen();
					saveGame();
					Sleep(1000);
					break; //main menu exit
				}

				else {
					auto pauseEnd = std::chrono::steady_clock::now(); //we store the time player paused and resumed to deduct it from actual playing time
					auto pauseDuration = pauseEnd - pauseStart;
					levelStartTime += pauseDuration;
					startTime += pauseDuration;

					setColor(Color::WHITE);
					screen.draw();
					currRoom.drawTopLayer();
					for (int i = 0; i < PLAYER_AMOUNT; i++) {
						players[i].draw();
					}
				}
				
			}
		}
		
		currRoom.resetObstacles();
		Point currentExitPoint = currRoom.getExitPos();

		//update loop
		for (int i = 0; i < PLAYER_AMOUNT; i++) {
			Player& p = players[i];
			Player& other = players[1 - i];

			p.inputManager(key, currRoom);
			setColor(Color::WHITE);

			p.updateSpringPhysics(currRoom, &other);
			
			int eventID = p.move(currRoom, &other);
			
			if (eventID != 0) {
				handleRiddle(eventID, p, currRoom);
			}

			//check level completion for a player
			if (currentExitPoint.x != -1 && p.getPos() == currentExitPoint) {
				if (!p.isFinished()) {
					p.setFinished(true);
					gotoxy(50, 0);
					std::cout << "Player " << p.getSymbol() << " Is waiting...";
				}
			}
		}

		bool isVictory = checkLevelTransition(currentLevelIndex, players[0].getPos(), players[1].getPos());
		if (isVictory) break;
		
		if (boomDustCleaningNeeded) {
			currRoom.clearExplosions();
			currRoom.drawRoom(screen);
			
			screen.draw(); 
			currRoom.drawTopLayer();
			boomDustCleaningNeeded = false;
		}
		
		currRoom.updateBombs(players, PLAYER_AMOUNT , screen); // update bombs
		
		if (currRoom.hasExplosions()) {
			boomDustCleaningNeeded = true;
			
		}

		bool gameOver = false;
		for (int i = 0; i < PLAYER_AMOUNT; i++) {
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
		printHUD();
		printTimer();
		sleepFrame();
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

		if (currentLevelIndex < (int)levels.size() - 1)
		{
			currentLevelIndex++;
			onLevelChange(currentLevelIndex);
			setGame(currentLevelIndex, false);

			resetLevelTimer();

			printTimer();
			return false;
		}
		else
		{

			showEndingScreen();
			return true; 
		}
	}

	return false;
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

	screen.clearScreen();
	setColor(Color::CYAN);
	printCentered("=== RIDDLE TIME! ===", 5);
	setColor(Color::WHITE);
	printCentered(riddle.question, 8);

	for (int i = 0; i < riddle.options.size(); i++) {
		std::string currOption = "(" + std::to_string(i + 1) + ") " + riddle.options[i];
		printCentered(currOption, 11 + i * 2);
	}
	while (_kbhit()) _getch();

	while (true) {
		if (_kbhit()) {
			char c = _getch();
			if (c < '1' || c  > '5') continue; //maximum 5 options 
			int choice = c - '0';
			if (choice - 1 == riddle.correctAnswer) {

				onRiddleSolved(true);

				setColor(Color::GREEN);
				printCentered("CORRECT!", 20);
				Sleep(500);

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
				onRiddleSolved(false);
				onLifeLost();
				setColor(Color::RED);
				printCentered("WRONG! -" + std::to_string(HP_INCREASE) + " HP •`_´•", 20);
				player.takeDamage(HP_INCREASE);
				Sleep(500);
			}
			break;
		}
	}
	setColor(Color::WHITE);
	screen.clearScreen();
	room.drawRoom(screen);
	screen.draw();
	room.drawTopLayer();
}

void Game::showEndingScreen()
{
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
