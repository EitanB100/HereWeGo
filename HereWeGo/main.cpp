#include <windows.h>
#include <conio.h>
#include "Player.h"
#include "Placement.h"
#include "Room.h"
#include "Screen.h"
#include "Utils.h"
#include "CommandKeys.h"

int main() {

	constexpr char ESC = 27;
	hideCursor();
	cls();
	Screen screen;

	Placement start(10, 10);
	Placement start2(9, 15);
	Placement startR(7, 6);

	char player1Keys[NUM_KEYS] = { 'W','S','A','D','E','Q' };
	char player2Keys[NUM_KEYS] = { 'I','K','J','L','U','O' };  

	Room areaA(startR, 'W', 18, 50);
	Player players[] = {
		Player(start, '&', 1, 0, player1Keys,screen),
		Player(start2, '*', 0, 1, player2Keys, screen)
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
			player.move();
		}

		Sleep(50);       // frame delay
	}
	cls();
}

