#include <windows.h>
#include <iostream>
#include <cstdlib>
#include "utils.h"
#include "Game.h"


static bool colorEnabled = true;

void gotoxy(int x, int y) {
    std::cout.flush();
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE; // Hide the cursor
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}



void setColorMode(bool useColor)
{
	colorEnabled = useColor;
}

void setColor(Color c) // set console text color - logic suggested by gemini
{
	setColor(c, colorEnabled);
}

void setColor(Color c, bool allowColor)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (allowColor) {
		
		SetConsoleTextAttribute(hConsole, int(c));
	}
	else
		SetConsoleTextAttribute(hConsole, static_cast<int>(Color::WHITE));
}


void printCentered(std::string text, int y)
{
	size_t length = text.length();


	int x = (80 - length) / 2;
	gotoxy(x, y);
	std::cout << text;
}

void printInstructions() {
	system("cls");
	printCentered("Instructions:",5);
	printCentered("Use W/A/X/D to move Player 1, S to stop in place, Q to drop an Item",8);
	printCentered("Use I/J/M/L to move Player 2, K to stop in place, O to drop an Item",10);
	printCentered("Press any key to return",12);
	_getch();
}