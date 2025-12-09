#include <windows.h>
#include <conio.h>
#include <iostream>
#include "Game.h"
//removed all the inclusions!


int main() {
	
	bool exitProgram = false;
	hideCursor();
	while (!exitProgram)
	{
		system("cls");

		printCentered("Welcome to the game!", 5);
		printCentered("(1) Start new game", 8);
		printCentered("(8) Instructions", 10);
		printCentered("(9) EXIT", 12);
		gotoxy(0, 15);
		std::cout << "for easy playtesting:" << std::endl;
		std::cout << "(2) Skip to level 2" << std::endl;
		std::cout << "(3) Skip to level 3" << std::endl;
		
		char selection = _getch();

		switch (selection)
		{
		
		case '1':
		{
			Game game;
			game.startInLevel(0);
			game.run();
			break;
		}
		case '2':
		{
			Game game;
			game.startInLevel(1);
			game.run();
			break;
		}
		case '3':
		{
			Game game;
			game.startInLevel(2);
			game.run();
			break;
		}
		case '8':
			gotoxy(MAX_X / 2, MAX_Y / 2);
			printInstructions();
			break;
		
		case '9':
		{
			
			system("cls");
			printCentered("Goodbye :3", 10);
			exitProgram = true;
			break;
		}
			
		}

	}
	
	return 0;
}