#include <iostream>

#include "Screen.h"
#include "utils.h"

void Screen::draw() const {
	int y = 0;
	for (const auto& row : screen) {
		gotoxy(0, y++);
		std::cout << row << std::flush;
	}
}

void Screen::setTile(int x, int y, char symbol) const {
	screen[x][y] = symbol;
}

void Screen::inLimit(Placement& p)
{

}


void Screen::createGameScreen() {

	for (int i = 0; i < MAX_Y; i++)
	{
		for (int j = 0; j < MAX_X; j++)
		{
			screen[i][j] = 'W';
		}
	}

}
