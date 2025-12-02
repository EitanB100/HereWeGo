#include <windows.h>
#include <iostream>
#include <cstdlib>
#include "utils.h"

using namespace std;

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



void setColor(Color c) // set console text color - logic suggested by gemini
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, int(c));  
}


void printCentered(std::string text, int y)
{
	size_t length = text.length();


	int x = (80 - length) / 2;
	gotoxy(x, y);
	cout << text;
}

void printInstructions() {
	system("cls");
	cout << "Instructions:" << endl;
	cout << "Use W/A/S/D to move Player 1" << endl;
	cout << "Use I/J/K/L to move Player 2" << endl;
	cout << "Press any key to go back";
	_getch();
}