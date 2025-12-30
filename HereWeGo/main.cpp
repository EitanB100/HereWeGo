#include <windows.h>
#include <conio.h>
#include <iostream>
#include "Game.h"
//removed all the inclusions!


int main() {

	constexpr char MENU_LVL1 = '1';
	constexpr char MENU_LVL2 = '2';
	constexpr char MENU_LVL3 = '3';
	constexpr char MENU_TEST = '4';
	constexpr char MENU_SETTINGS = '7';
	constexpr char MENU_INSTRUCTIONS = '8';
	constexpr char MENU_EXIT = '9';

	bool exitProgram = false;
	hideCursor();
	while (!exitProgram)
	{
		system("cls");

		printCentered("Welcome to the game!", 4);
		printCentered("(1) Start new game", 7);
		printCentered("(7) Settings", 9);
		printCentered("(8) Instructions", 11);
		printCentered("(9) EXIT", 13);
		gotoxy(0, 16);
		std::cout << "for easy playtesting:" << std::endl;
		std::cout << "(2) Skip to level 2" << std::endl;
		std::cout << "(3) Skip to level 3" << std::endl;
		std::cout << "(4) Test Space" << std::endl;

		char selection = _getch();

		switch (selection)
		{

		case MENU_LVL1:	
		{
			Game game;
			game.startInLevel(Level::ONE);
			game.run();
			break;
		}
		case MENU_LVL2:
		{
			Game game;
			game.startInLevel(Level::TWO);
			game.run();
			break;
		}
		case MENU_LVL3:
		{
			Game game;
			game.startInLevel(Level::THREE);
			game.run();
			break;
		}
		case MENU_TEST:
		{
			Game game;
			game.startInLevel(Level::TEST);
			game.run();
			break;
		}
		case MENU_SETTINGS:
		{
			Game game;
			game.settingsMenu();
			break;
		}
		case MENU_INSTRUCTIONS: {
			gotoxy(MAX_X / 2, MAX_Y / 2);
			printInstructions();
			break;
		}
		case MENU_EXIT:
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