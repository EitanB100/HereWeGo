#include "Game.h"
#include <iostream>
#include <iomanip>
#include <sstream>
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

	while (true) {
		if (_kbhit) {
			char c = _getch();
			int choice = c - '0';
			if (choice == riddle.correctAnswer) {
				setColor(Color::GREEN);
				printCentered("CORRECT!", 20);
				Sleep(500);

				Point targetPos = { player.getPos().x + player.getDirX(), player.getPos().y + player.getDirY() };

				if (room.getObjectAt(targetPos) == RIDDLE_TILE) {
					room.removeRiddle(targetPos);
					break;
				}
			}

			else {
				setColor(Color::RED);
				printCentered("WRONG! -" + std::to_string(HP_INCREASE) + " HP •`_´•", 20);
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

void Game::printHUD()
{
	Point hudPos = levels[*currentLevelID].getLegendLoc();

	gotoxy(hudPos.x, hudPos.y);

	setColor(Color::WHITE);
	std::cout << "Player 1: ";
	const heldItem& item1 = players[0].getItemInHand();
	if (item1.type == ItemType::KEY) {
		setColor(item1.color);
		std::cout << "KEY ";
	}
	else if (item1.type == ItemType::TORCH) {
		setColor(item1.color);
		std::cout << "TORCH ";
	}

	else std::cout << "EMPTY ";
	setColor(Color::WHITE);

	std::cout << " Hitpoints " << players[0].getHP() << "/" << Player::MAX_HP;

	std::cout << "| Player 2: ";
	const heldItem& item2 = players[1].getItemInHand();
	if (item2.type == ItemType::KEY) {
		setColor(item2.color);
		std::cout << "KEY ";
	}

	else if (item2.type == ItemType::TORCH) {
		setColor(item2.color);
		std::cout << "TORCH ";
	}

	else std::cout << "EMPTY ";
	setColor(Color::WHITE);
	std::cout << " Hitpoints " << players[1].getHP() << "/15 ";
}


void Game::printTimer() {

	Point hudPos = levels[*currentLevelID].getLegendLoc();
	gotoxy(hudPos.x, hudPos.y + 1);
	
	auto currentTime = std::chrono::steady_clock::now();

	// Calculate Total Time from the very beginning of the game
	auto totalElapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

	// Calculate Level Time from the last reset (start of level)
	auto levelElapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - levelStartTime).count();

	// Lambda to format seconds into MM:SS
	auto formatTime = [](long long totalSeconds) -> std::string {
		int m = static_cast<int>((totalSeconds % 3600) / 60);
		int s = static_cast<int>(totalSeconds % 60);
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(2) << m << ":"
			<< std::setfill('0') << std::setw(2) << s;
		return ss.str();
		};

	gotoxy(0, 1);
	setColor(Color::YELLOW);
	std::cout << "Total Time: " << formatTime(totalElapsed)
		<< " | Level Time: " << formatTime(levelElapsed) << " ";
	gotoxy(hudPos.x + 20, hudPos.y + 1);
	printScore();
}

void Game::printScore() {
	setColor(Color::YELLOW);
	gotoxy(55, 1);
	std::cout << "Score: " << score;
}
