#include "Game.h"

const char p1Keys[NUM_KEYS] = { 'W','S','A','D','E','Q' };
const char p2Keys[NUM_KEYS] = { 'I','K','J','L','U','O' };

Game::Game() : players{
	Player(Placement(10,20),'&',1,0,p1Keys),
	Player(Placement(9,15),'@',0,1,p2Keys)
}
{
	init();
}
void Game::init()
{

	// --- SETUP SWITCH (Restored from Conflict) ---
	Switch SW1(50, 5, 20);
	room.addSwitch(SW1);

	Point swPos = SW1.getPos();
	Switch* activeSwitch = room.isSwitchThere(swPos);

	// --- SETUP DOOR 1 (Green) ---
	Door d1(12, 0, 1, Color::GREEN);
	d1.addRequiredKey(10);
	d1.addRequiredSwitch(activeSwitch, false); // Restored: Requires Switch OFF
	room.addDoor(d1);

	// --- SETUP DOOR 2 (Red) ---
	Door d2(79, 12, 2, Color::RED);
	d2.addRequiredKey(11);
	d2.addRequiredKey(12);
	d2.addRequiredKey(13);
	d2.addRequiredSwitch(activeSwitch, true); // Restored: Requires Switch ON
	room.addDoor(d2); // Note: Added d2 AFTER configuring requirements

	// --- SETUP KEYS ---
	room.addKey(Key(40, 5, 10, Color::GREEN));
	room.addKey(Key(20, 15, 11, Color::RED));
	room.addKey(Key(20, 10, 12, Color::RED));
	room.addKey(Key(20, 5, 13, Color::RED));

	// --- SETUP WALLS ---
	room.addWall(Point{ 30,10 });
	room.addWall(Point{ 31,10 });

	// --- SETUP OBSTACLES ---
	Obstacle rock;
	rock.addPart(Placement(35, 10));
	rock.addPart(Placement(35, 11));
	Obstacle rock1;
	rock1.addPart(Placement(21, 15));
	room.addObstacle(rock);
	room.addObstacle(rock1);

	// --- DRAWING ---
	room.drawRoom(screen);
	screen.draw();
	room.drawTopLayer();

	for (auto& player : players)
	{
		player.draw();
	}
}

void Game::run()
{
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
		room.resetObstacles();

		for (int i = 0; i < 2; i++) {
			players[i].changeDirection(key);
		}
		
		for (int i = 0; i < 2; i++) {
			setColor(Color::WHITE);
			players[i].move(room, &players[1 - i]);
		}

		Sleep(75); // Fixed the garbage characters here
	}

}
