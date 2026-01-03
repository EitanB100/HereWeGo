#include <iostream>

#include "Screen.h"
#include "utils.h"


void Screen::createGameScreen() {

	for (int i = 0; i < MAX_Y; i++)
	{
		for (int j = 0; j < MAX_X; j++)
		{
			screen[i][j] = ' ';
		}
	}

}

void Screen::loadMapData(const char* mapData[MAX_Y])
{
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			setTile(x, y, mapData[y][x]);
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


void Screen::setTile(int x, int y, char symbol) { //updated to improve readability, removed const
	screen[y][x] = symbol;
}

void Screen::draw() const {
	int y = 0;
	for (const auto& row : screen) {
		gotoxy(0, y++);
		std::cout << row << std::flush;
	}
}



