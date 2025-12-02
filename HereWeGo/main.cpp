#include <windows.h>
#include <conio.h>
#include <iostream>
#include "Game.h"
#include "Player.h"
#include "Placement.h"
#include "Room.h"
#include "Door.h"
#include "Key.h"
#include "Screen.h"
#include "Utils.h"
#include "CommandKeys.h"
#include "Switch.h" // <--- Added this

using namespace std;



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
		
		char selection = _getch();

		switch (selection)
		{
		case '1':
		{
			Game game;
			game.run();
			break;
		}
		case '8':
			
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