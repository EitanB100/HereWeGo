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

Screen::Screen() {
	for (int i = 0; i < MAX_Y; i++) {
		screen[i] = new char[MAX_X + 1];
		screen[i][MAX_X] = '\0';
	}
	createGameScreen();
}

Screen::~Screen() {  // Destructor to free allocated memory
	for (int i = 0; i < MAX_Y; i++) {
		delete[] screen[i];
	}
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

