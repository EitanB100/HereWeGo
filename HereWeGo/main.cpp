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

		cout << "Welcome to the game!" << endl;
		cout << "(1) Start a new game" << endl;
		cout << "(8) Instructions" << endl;
		cout << "(9) EXIT" << endl;
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
























	/*constexpr char ESC = 27;
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

	// --- SETUP SWITCH (Restored from Conflict) ---
	Switch SW1(50, 5, 20);
	areaA.addSwitch(SW1);

	Point swPos = SW1.getPos();
	Switch* activeSwitch = areaA.isSwitchThere(swPos);

	// --- SETUP DOOR 1 (Green) ---
	Door d1(12, 0, 1, Color::GREEN);
	d1.addRequiredKey(10);
	d1.addRequiredSwitch(activeSwitch, false); // Restored: Requires Switch OFF
	areaA.addDoor(d1);

	// --- SETUP DOOR 2 (Red) ---
	Door d2(79, 12, 2, Color::RED);
	d2.addRequiredKey(11);
	d2.addRequiredKey(12);
	d2.addRequiredKey(13);
	d2.addRequiredSwitch(activeSwitch, true); // Restored: Requires Switch ON
	areaA.addDoor(d2); // Note: Added d2 AFTER configuring requirements

	// --- SETUP KEYS ---
	areaA.addKey(Key(40, 5, 10, Color::GREEN));
	areaA.addKey(Key(20, 15, 11, Color::RED));
	areaA.addKey(Key(20, 10, 12, Color::RED));
	areaA.addKey(Key(20, 5, 13, Color::RED));

	// --- SETUP WALLS ---
	areaA.addWall(Point{ 30,10 });
	areaA.addWall(Point{ 31,10 });


	// --- DRAWING ---
	areaA.drawRoom(screen);
	screen.draw();
	areaA.drawTopLayer();

	for (auto& player : players)
	{
		player.draw();
	}

	// --- GAME LOOP ---
	while (true) {
		char key = 0;

		if (_kbhit()) {
			key = _getch();
			if (key == ESC) {
				key = _getch();
				if (key == 'h' || key == 'H') {
					screen.clearScreen();
					break;
				}
			}
		}

		for (auto& player : players) {
			setColor(Color::WHITE);
			player.changeDirection(key);
			player.move(areaA);
		}

		Sleep(50); // Fixed the garbage characters here
	}
	
	
	screen.clearScreen();*/
}