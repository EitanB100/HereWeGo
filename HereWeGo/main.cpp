#include <windows.h>
#include <conio.h>
#include "Player.h"
#include "Placement.h"
#include "Room.h"
#include "Door.h"
#include "Key.h"
#include "Screen.h"
#include "Utils.h"
#include "CommandKeys.h"

int main() {

	constexpr char ESC = 27;
	hideCursor();
	Screen screen;
	screen.clearScreen();

	Placement start = Placement(10,10);
	Placement start2 = Placement(9,15);


	char player1Keys[NUM_KEYS] = { 'W','S','A','D','E','Q' };
	char player2Keys[NUM_KEYS] = { 'I','K','J','L','U','O' };  

	Room areaA;
	



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
	d2.addRequiredKey(12);
	d2.addRequiredKey(13);
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

