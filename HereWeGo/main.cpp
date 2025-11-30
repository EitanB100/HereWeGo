#include <windows.h>
#include <conio.h>
#include "Player.h"
#include "Placement.h"
#include "Room.h"
#include "Door.h"
#include "Key.h"
<<<<<<< Updated upstream
#include "Wall.h"
=======
>>>>>>> Stashed changes
#include "Screen.h"
#include "Utils.h"
#include "CommandKeys.h"

int main() {

	constexpr char ESC = 27;
	hideCursor();
	cls();
	Screen screen;
	screen.clearScreen();

	Placement start = Placement(10,10);
	Placement start2 = Placement(9,15);

<<<<<<< Updated upstream
	Placement start = Placement(10,10);
	Placement start2 = Placement(9,15);


	char player1Keys[NUM_KEYS] = { 'W','S','A','D','E','Q' };
	char player2Keys[NUM_KEYS] = { 'I','K','J','L','U','O' };  

	Room areaA = Room();

	areaA.initializeArrays(2, 2, 4, 3);

	areaA.addDoor(0, Door(0, 0, 12, 1, 0)); // doorNum, x, y, destID, keyID
	areaA.addDoor(1, Door(1, 78, 12, 2, 1));
	areaA.addKey(0, Key(40, 5, 0)); // x, y, keyID
	areaA.addKey(1, Key(20, 15, 1));
	areaA.addWall(0, Wall(30, 10));
	areaA.addWall(1, Wall(31, 10));
	areaA.addWall(2, Wall(32, 10));
	areaA.addWall(3, Wall(33, 10));
=======

	char player1Keys[NUM_KEYS] = { 'W','S','A','D','E','Q' };
	char player2Keys[NUM_KEYS] = { 'I','K','J','L','U','O' };  

	Room areaA;
	



>>>>>>> Stashed changes
	Player players[] = {
		Player(start, '&', 1, 0, player1Keys),
		Player(start2, '@', 0, 1, player2Keys)
	};

	Door d1(12, 0, 1, Color::GREEN);
	d1.addRequiredKey(10);
	areaA.addDoor(d1);

	Door d2(79, 12, 2, Color::RED);
	d2.addRequiredKey(11);
	
	
	areaA.addKey(Key(40, 5, 10, Color::GREEN));
	areaA.addKey(Key(20, 15, 11, Color::RED));
	areaA.addKey(Key(20, 10, 12, Color::RED));
	areaA.addKey(Key(20, 5, 13, Color::RED));

	Switch* SW1 = new Switch(50, 5, 20);
	areaA.addSwitch(*SW1);


	d2.addRequiredKey(12);
	d2.addRequiredKey(13);
	d1.addRequiredSwitch(SW1, false);
	d2.addRequiredSwitch(SW1, true);
	areaA.addWall(Point{30,10});
	areaA.addWall(Point{ 31,10 });
	areaA.addDoor(d2);


	areaA.drawRoom(screen);
	screen.draw();

	areaA.drawTopLayer();

	for (auto& player : players)
	{
		player.draw();
	}
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

		Sleep(50);       // frame delay
	}
	screen.clearScreen();
}

