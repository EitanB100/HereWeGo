#include "Game.h"
#include <iostream>
#include <iomanip>
#include <sstream>
void Game::printHUD()
{
	gotoxy(0, 0);
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

	std::cout << " Hitpoints " << players[0].getHP() << "/" << Player::STARTING_HP;

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

	printScore();
}

void Game::printScore() {
	setColor(Color::YELLOW);
	gotoxy(55, 1);
	std::cout << "Score: " << score;
}
