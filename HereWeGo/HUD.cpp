#include "Game.h"
#include <iostream>
#include <iomanip>
#include <sstream>

void Game::printHUD()
{
	Point hudPos = levels[currentLevelIndex].getLegendLoc();

	gotoxy(hudPos.x, hudPos.y);

	setColor(Color::WHITE);
	std::cout << "P1:" << players[0].getHP() << "/" << players[0].MAX_HP << " [";
	
	const heldItem& item1 = players[0].getItemInHand();

	if (item1.type == ItemType::KEY) {
		setColor(item1.color);
		std::cout << "KEY";
	}
	else if (item1.type == ItemType::TORCH) {
		setColor(item1.color);
		std::cout << "TORCH";
	}
	else if (item1.type == ItemType::BOMB) {
		setColor(item1.color);
		std::cout << "BOMB";
	}
	else std::cout << " ";
	setColor(Color::WHITE);
	std::cout << "]    ";

	gotoxy(hudPos.x, hudPos.y + 1);
	std::cout << "P2:" << players[1].getHP() << "/" << players[1].MAX_HP << " [";

 	const heldItem& item2 = players[1].getItemInHand();
	
	if (item2.type == ItemType::KEY) {
		setColor(item2.color);
		std::cout << "KEY ";
	}
	else if (item2.type == ItemType::TORCH) {
		setColor(item2.color);
		std::cout << "TORCH ";
	}
	else if (item2.type == ItemType::BOMB) {
		setColor(item2.color);
		std::cout << "BOMB";
	}
	else std::cout << " ";
	setColor(Color::WHITE);
	std::cout << "]    ";
}


void Game::printTimer() {

	const Point& hudPos = levels[currentLevelIndex].getLegendLoc();
	gotoxy(hudPos.x, hudPos.y + 2);
	
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

	setColor(Color::GREEN);
	std::cout << "TIME " << formatTime(levelElapsed) << " |";
	
	printScore(hudPos);
}

void Game::printScore(const Point& hudPos) {
	gotoxy(hudPos.x + 12, hudPos.y + 2);

	setColor(Color::GREEN);
	std::cout << " Score: " << score;
}
