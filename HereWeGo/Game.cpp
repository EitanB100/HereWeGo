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
	startTime = std::chrono::steady_clock::now();
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

	std::string endingFile = "ending.screen";
	std::ifstream endCheck(endingFile);
	if (endCheck.is_open()) {
		endCheck.close();
		levels.emplace_back();

		if (!Level_Loader::loadLevel(levels.back(), endingFile, loadingErrorMessage)) {
			loadingErrorMessage = "Ending Screen failed - " + loadingErrorMessage;
			levels.pop_back();
			return;
		}
	}

	if (levels.empty()) levelLoadedCorrectly = false;
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


void Game::run()
{
	if (!levelLoadedCorrectly) {
		tileMapError();
		return; // Exits run(), returning to main() menu
	}
	resetLevelTimer();
	bool boomDustCleaningNeeded = false;
	while (true) {
		Room& currRoom = levels[currentLevelIndex]; 
		char key = 0;
		
		if (_kbhit()) {
			key = _getch();
			if (key == ESC) {
				auto pauseStart = std::chrono::steady_clock::now();

				setColor(Color::BLUE);
				printCentered("GAME PAUSED", 2);
				key = _getch();

				if (key == 'h' || key == 'H') {
					setColor(Color::WHITE);
					screen.clearScreen();
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
		Sleep(GAME_SPEED);
	}

}

bool Game::checkLevelTransition(int& currentLevelIndex, Point p1, Point p2)
{
	Point exit = levels[currentLevelIndex].getExitPos();
	if (exit.x == -1) return false; // No standard exit

	if (p1 == exit && p2 == exit)
	{
		// 1. Calculate Score BEFORE resetting the level timer
		// This ensures the player is rewarded based on the time spent on the level they just finished
		auto now = std::chrono::steady_clock::now();
		int levelSeconds = std::chrono::duration_cast<std::chrono::seconds>(now - levelStartTime).count();
		score += (MAX_SCORE / (levelSeconds + 1));

		// 2. Handle Special Case (Obstacle carry over)
		if (currentLevelIndex < levels.size() - 1)
		{
			currentLevelIndex++;

			setGame(currentLevelIndex, false);

			resetLevelTimer();

			printTimer(); // Force a timer update immediately so it doesn't show the old time for 75ms
			return false;
		}
		else {
			setColor(Color::GREEN);
			printCentered("THANKS FOR PLAYING!", 12);
			Sleep(1500);
			setColor(Color::WHITE);
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
