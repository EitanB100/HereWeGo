#include <windows.h>
#include <conio.h>
#include "Player.h"
#include "Placement.h"
#include "Room.h"
#include "Door.h"
#include "Key.h"
#include "Wall.h"
#include "Screen.h"
#include "Utils.h"
#include "CommandKeys.h"

int main() {

	constexpr char ESC = 27;
	hideCursor();
	cls();
	Screen screen;

	Placement start = Placement(10,10,'&');
	Placement start2 = Placement(9,15,'*');

	Placement startR(7, 6);

	char player1Keys[NUM_KEYS] = { 'W','S','A','D','E','Q' };
	char player2Keys[NUM_KEYS] = { 'I','K','J','L','U','O' };  

	Room areaA = Room();

	areaA.initializeArrays(2, 2, 4);

	areaA.addDoor(0, Door(0, 0, 12, 1, 0)); // doorNum, x, y, destID, keyID
	areaA.addDoor(1, Door(1, 78, 12, 2, 1));
	areaA.addKey(0, Key(40, 5, 0)); // x, y, keyID
	areaA.addKey(1, Key(20, 15, 1));
	areaA.addWall(0, Wall(30, 10));
	areaA.addWall(1, Wall(31, 10));
	areaA.addWall(2, Wall(32, 10));
	areaA.addWall(3, Wall(33, 10));
	Player players[] = {
		Player(start, '&', 1, 0, player1Keys),
		Player(start2, '*', 0, 1, player2Keys)
	};

	areaA.drawRoom(screen);
	screen.draw();

	while (true) {
		char key = 0;

		if (_kbhit()) {
			key = _getch();
			if (key == ESC) {
				key = _getch();
				if (key == 'h' || key == 'H') {
					cls();
					break;
				}
			}
		}

		for (auto& player : players) {
			player.changeDirection(key);
			player.move(areaA);
		}

		Sleep(50);       // frame delay
	}
	cls();
}

