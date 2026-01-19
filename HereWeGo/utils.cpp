#include <windows.h>
#include <iostream>
#include <cstdlib>
#include "utils.h"
#include "Game.h"


static bool colorEnabled = true;



void gotoxy(int x, int y) {
	if (isGlobalSilent()) return;

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

bool getColorMode()
{
	return colorEnabled;
}

void setColor(Color c) // set console text color - logic suggested by gemini
{
	if (isGlobalSilent()) return;
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
	if (isGlobalSilent()) return;

	int length = static_cast<int>(text.length());
	int x = (MAX_X - length) / 2;
	if (x < 0) x = 0;
	gotoxy(x, y);
	std::cout << text;
}

void printInstructions() {
	std::string p1Text = "P1: Move " + std::string(1, Game::p1Keys[0]) +
		"/" + std::string(1, Game::p1Keys[2]) +
		"/" + std::string(1, Game::p1Keys[1]) +
		"/" + std::string(1, Game::p1Keys[3]) + 
		", " + std::string(1,Game::p1Keys[4]) + " to stay, " +
		std::string(1,Game::p1Keys[5]) + " to dispose";

	std::string p2Text = "P2: Move " + std::string(1, Game::p2Keys[0]) +
		"/" + std::string(1, Game::p2Keys[2]) +
		"/" + std::string(1, Game::p2Keys[1]) +
		"/" + std::string(1, Game::p2Keys[3]) +
		", " + std::string(1, Game::p2Keys[4]) + " to stay, " +
		std::string(1, Game::p2Keys[5]) + " to dispose";

	system("cls");
	printCentered("Instructions:",5);
	printCentered(p1Text,8);
	printCentered(p2Text,10); 
	printCentered("Press any key to return", 12);
	printCentered("Tip - When you drop a bomb, it explodes automatically and destroyes everything around it. Better be careful!", 20);
	
	_getch();
}