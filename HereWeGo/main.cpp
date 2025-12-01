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
			game.init();
			game.run();
			break;
		}
		case '8':
			printInstructions();
			break;
		}

	}
	return 0;
























	/*
	hideCursor();
	Screen screen;
	screen.clearScreen();

	Placement start = Placement(10, 10);
	Placement start2 = Placement(9, 15);

	char player1Keys[NUM_KEYS] = { 'W','S','A','D','E','Q' };
	char player2Keys[NUM_KEYS] = { 'I','K','J','L','U','O' };

	Room areaA;

	Player players[] = {
		Player(start, '&', 1, 0, player1Keys),
		Player(start2, '@', 0, 1, player2Keys)
	};


	// --- GAME LOOP ---
	
	
	screen.clearScreen();*/
}