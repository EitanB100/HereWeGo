#include <iostream>

#include "Screen.h"
#include "utils.h"
#include "Game.h"

void Screen::createGameScreen() {

	for (int i = 0; i < MAX_Y; i++)
	{
		for (int j = 0; j < MAX_X; j++)
		{
			screen[i][j] = ' ';
		}
	}

}


void Screen::loadMapFromRoom(const char mapData[MAX_Y][MAX_X]) {
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			this->screen[y][x] = mapData[y][x];
		}
	}
}

Screen::Screen() {
	for (int i = 0; i < MAX_Y; i++) {
		screen[i][MAX_X] = '\0';
	}
	createGameScreen();
}

void Screen::draw() const {
	if (isGlobalSilent()) return;
	int y = 0;
	for (const auto& row : screen) {
		gotoxy(0, y++);
		std::cout << row << std::flush;
	}
}



