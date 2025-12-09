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

void Screen::Lvl1Screen() {
	const char* level1Layout[MAX_Y] = {
		//01234567890123456789012345678901234567890123456789012345678901234567890123456789
		 "                                                                                ",// 0
		 "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 1
		 "W              W                                    W                          W", // 2 
		 "W              W                                    W                          W", // 3, 
		 "W              W            W                       W                          W", // 4, 
		 "W              W            W                       W                          W", // 5, 
		 "WWWWWWWWWWWW WWWWWWWWWWWWWWWWWWWWWWWWWWWWWW   WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 6, 
		 "W                                                                              W", // 7, 
		 "W                                                                              W", // 8, 
		 "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 9, 
		 "W                    W                           W                             W", // 10, 
		 "W                    W                           W                             W", // 11,
		 "W                    W                           W                             W", // 12, 
		 "WWWWWWWWWWWWWWWWWWWW                             W                             W", // 13, 
		 "W                  W W                           W                             W", // 14, 
		 "W                  W W                           W                             W", // 15, 
		 "W                                                W                             W", // 16, 
		 "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW WWWWWWWWWWWWWWWWWW WWWWWWWWWWWWWWWW", // 17, 
		 "W       W          W               W            W                              W", // 18, 
		 "W       W          WWWWWWWWWWWWW   W            W                              W", // 19, 
		 "W     WWW          W               W                                           W", // 20, 
		 "W           WWWWWWWW WWWWWWWWWWW   W                                           W", // 21, 
		 "WWW  WW     W         W        W                                                ", // 22, 
		 "W     W          W    W   W        W            W                              W", // 23,  
		 "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 24
	};
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			screen[y][x] = level1Layout[y][x];
		}
	}
}

void Screen::Lvl2Screen() {
	const char* level1Layout[MAX_Y] = {
		//01234567890123456789012345678901234567890123456789012345678901234567890123456789
		 "                                                                                ",// 0
		 "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 1
		 "W        W                                                                     W", // 2 
		 "W        W                                                                     W", // 3, 
		 "WWWWWW   WWWWWWWWW  WWWWWWWWW  WWWWWWW  WWWWWWWWW  WWWWWWW  WWWWWWWWWWWWWWW    W", // 4, 
		 "W                                                                         W    W", // 5, 
		 "W                                                                         W    W", // 6, 
		 "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW    W", // 7, 
		 "W             W                                                                W", // 8, 
		 "W             W                                                                W", // 9, 
		 "W   WWWWW     WWWWWWWWWWW  WWWWWWWW  WWWWWWWW  WWWWWWWW  WWWWWWWW   WWWWWWWWWWWW", // 10, 
		 "W      W                                                                       W", // 11,
		 "W      W                                                                       W", // 12, 
		 "W      WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 13, 
		 "W                                                 W              W             W", // 14, 
		 "W                                                 W              W             W", // 15, 
		 "W                                                 W              W             W", // 16, 
		 "WWWWWWWWWWW  WWWWWWWWWWW   WWWWWWWWWWW            WWWWWWWWWWWWWWWW             W", // 17, 
		 "W                W         W         W                                         W", // 18, 
		 "W                W  WWWWWWWW WWWWW   W                                         W", // 19, 
		 "W                W           W   W   W                                         W", // 20, 
		 "W                WWWWWWWWWWWWW   W   W                                         W", // 21, 
		 "WGGGGGGGGGGGGGGGGW           W   W   W                                          ", // 22, 
		 "W                W           W       W                                         W", // 23,  
		 "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 24
	};
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			screen[y][x] = level1Layout[y][x];
		}
	}
}

void Screen::Lvl3Screen() {
	const char* level1Layout[MAX_Y] = {
		 "                                                                                ",// 0
		 "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 1
		 "W              W                                    W                          W", // 2 
		 "W              W                                    W                          W", // 3, 
		 "W              W            W                       W                          W", // 4, 
		 "W              W            W                       W                          W", // 5, 
		 "WWWWWWWWWWWW WWWWWWWWWWWWWWWWWWWWWWWWWWWWWW   WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 6, 
		 "W                                                                              W", // 7, 
		 "W                                                                              W", // 8, 
		 "WWWWWW                                                                   WWWWWWW", // 9, 
		 "W                    W                           W                             W", // 10, 
		 "W                    W                           W                             W", // 11,
		 "W                    W                           W                             W", // 12, 
		 "WWWWWWWWWWWWWWWWWWWW                             W                             W", // 13, 
		 "W                  W W                           W                             W", // 14, 
		 "W                  W W                           W                             W", // 15, 
		 "W                                                W                             W", // 16, 
		 "WWWWWWWWW                                                     W WWWWWWWWWWWWWWWW", // 17, 
		 "W       W          W               W            W                              W", // 18, 
		 "W       W          WWWWWWWWWWWWW   W            W                              W", // 19, 
		 "W     WWW          W               W                                           W", // 20, 
		 "W           WWWWWWWW WWWWWWWWWWW   W             **                            W", // 21, 
		 "WWW  WW     W         W        W                                                ", // 22, 
		 "W     W          W    W   W        W            W                              W", // 23,  
		 "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 24
	};
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			screen[y][x] = level1Layout[y][x];
		}
	}
}