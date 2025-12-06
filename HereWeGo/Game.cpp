#include "Game.h"

const char p1Keys[NUM_KEYS] = { 'W','S','A','D','E','Q' };
const char p2Keys[NUM_KEYS] = { 'I','K','J','L','U','O' };


void Game::initLevel1(Room& r)
{
	// ============================
	// 0. MAP SETTINGS
	// ============================
	int top = 1;  // Leave y=0 for HUD
	int bot = 22; // Bottom wall y-coord

	// ============================
	// 1. PLAYERS & TORCH
	// ============================
	players[0].setPos(4, top + 2); // P1 (Top Left Room)
	players[1].setPos(4, top + 6); // P2 (Room below P1)

	r.addTorch(Torch(2, top + 1, 6)); // T (Top Left Corner)

	// ============================
	// 2. WALLS
	// ============================
	// A. Outer Borders
	for (int x = 0; x <= 60; x++) { r.addWall(Point{ x, top }); r.addWall(Point{ x, bot }); }
	for (int y = top; y <= bot; y++) { r.addWall(Point{ 0, y }); r.addWall(Point{ 60, y }); }

	// B. Internal Vertical Walls
	// Wall x=12 (Separates Left Rooms)
	for (int y = top; y <= bot; y++) if (y != top + 4 && y != top + 7 && y != 14 && y != 15) r.addWall(Point{ 12, y });

	// Wall x=24 (Separates Middle/Right)
	for (int y = top; y <= 17; y++) if (y != top + 1 && y != top + 2 && y != top + 3) r.addWall(Point{ 24, y });

	// Wall x=36 (Partial Right Wall)
	for (int y = top + 8; y <= 17; y++) r.addWall(Point{ 36, y });

	// C. Internal Horizontal Walls
	// Wall y=5 (Separates P1/P2)
	for (int x = 0; x <= 12; x++) if (x != 8) r.addWall(Point{ x, top + 4 });

	// Wall y=8 (Long horizontal)
	for (int x = 12; x <= 60; x++) r.addWall(Point{ x, top + 7 });

	// Wall y=17 (Bottom Corridor)
	for (int x = 0; x <= 60; x++) if (x != 30 && x != 45) r.addWall(Point{ x, 17 });

	// Bottom Left Maze Walls
	r.addWall(Point{ 6, 19 }); r.addWall(Point{ 6, 20 });
	r.addWall(Point{ 12, 19 }); r.addWall(Point{ 12, 20 });
	for (int x = 8; x <= 16; x++) r.addWall(Point{ x, 21 }); // Small maze wall

	// ============================
	// 3. OBSTACLES ('*')
	// ============================
	Obstacle obsTop; // Blocking Top Corridor (x=24)
	obsTop.addPart(Placement(24, top + 1));
	obsTop.addPart(Placement(24, top + 2));
	obsTop.addPart(Placement(24, top + 3));
	r.addObstacle(obsTop);

	Obstacle obsLeft; // Blocking Left Vertical (x=12)
	obsLeft.addPart(Placement(12, 14));
	obsLeft.addPart(Placement(12, 15));
	r.addObstacle(obsLeft);

	// ============================
	// 4. KEYS ('K')
	// ============================
	r.addKey(Key(16, top + 2, 10, Color::MAGENTA)); // Magenta Key (Top Middle)
	r.addKey(Key(45, top + 2, 11, Color::GREEN));   // Green Key (Top Right)
	r.addKey(Key(4, 13, 12, Color::RED));           // Orange Key (Left Room) - Using RED for Orange
	r.addKey(Key(14, 19, 13, Color::GREEN));        // Green Key 2 (Bot Left Maze)

	// ============================
	// 5. SWITCHES ('/' and '\')
	// ============================
	Switch sGreen1(40, top + 9, 1);  // Top Right Area
	Switch sBlue1(45, top + 9, 2);
	Switch sBlue2(50, top + 9, 3);

	Switch sOrange(5, 16, 4);        // Bottom Left

	// THE FAKE SWITCH (Bottom Right) - ID 99
	Switch sFake(45, 21, 99);

	r.addSwitch(sGreen1);
	r.addSwitch(sBlue1);
	r.addSwitch(sBlue2);
	r.addSwitch(sOrange);
	r.addSwitch(sFake);

	// Pointers for Door Logic
	Switch* pS_Green1 = r.isSwitchThere(sGreen1.getPos());
	Switch* pS_Blue1 = r.isSwitchThere(sBlue1.getPos());
	Switch* pS_Blue2 = r.isSwitchThere(sBlue2.getPos());
	Switch* pS_Orange = r.isSwitchThere(sOrange.getPos());

	// ============================
	// 6. DOORS ('D')
	// ============================

	// 1. White Door (Start P1)
	Door dWhite(8, top + 4, 1, Color::WHITE);
	r.addDoor(dWhite);

	// 2. Magenta Door (Left Vertical)
	Door dMag(12, top + 7, 2, Color::MAGENTA);
	dMag.addRequiredKey(10);
	r.addDoor(dMag);

	// 3. Orange Door (Opens maze or switch path)
	// Located at (16, 10) roughly based on image
	Door dOrange(16, 10, 3, Color::RED);
	dOrange.addRequiredKey(12); // Needs Orange Key
	dOrange.addRequiredSwitch(pS_Orange, true); // Needs Switch ON
	r.addDoor(dOrange);

	// 4. Blue Doors (Top Right Room)
	Door dBlue1(42, top + 9, 4, Color::CYAN);
	dBlue1.addRequiredSwitch(pS_Blue1, false); // Switch OFF opens it
	r.addDoor(dBlue1);

	Door dBlue2(47, top + 9, 5, Color::CYAN);
	dBlue2.addRequiredSwitch(pS_Blue2, false); // Switch OFF opens it
	r.addDoor(dBlue2);

	// 5. EXIT DOOR (Green) - Bottom Right
	Door dExit(60, 20, 9, Color::GREEN);
	dExit.addRequiredKey(11); // Needs Green Key
	dExit.addRequiredKey(13); // Needs Green Key 2
	dExit.addRequiredSwitch(pS_Green1, true); // Needs Green Switch ON
	r.addDoor(dExit);
}

void Game::initLevel2(Room& r)
{
	// 1. Walls (Simple Box)
	for (int x = 0; x < 79; x++) { r.addWall(Point{ x, 0 }); r.addWall(Point{ x, 24 }); }
	for (int y = 0; y < 25; y++) { r.addWall(Point{ 0, y }); r.addWall(Point{ 78, y }); }

	// 2. Torch (Required for vision)
	r.addTorch(Torch(5, 5, 8));

	// 3. Hidden Key
	r.addKey(Key(70, 20, 20, Color::GREEN));

	// 4. Exit Door
	Door dExit(77, 22, 9, Color::CYAN);
	dExit.addRequiredKey(20);
	r.addDoor(dExit);
}

void Game::initLevel3(Room& r)
{
	for (int x = 0; x < 40; x++) { r.addWall(Point{ x, 0 }); r.addWall(Point{ x, 20 }); }
	for (int y = 0; y <= 20; y++) { r.addWall(Point{ 0, y }); r.addWall(Point{ 40, y }); }
}


void Game::printHUD()
{
	gotoxy(0, 0);
	setColor(Color::WHITE);
	std::cout << "Player 1: ";
	const heldItem& item1 = players[0].getItemInHand();
	if (item1.type == KEY) {
		setColor(item1.color);
		std::cout << "KEY ";
	}
	else if (item1.type == TORCH) {
		setColor(item1.color);
		std::cout << "TORCH ";
	}
	//remove the comment brackets if you implemented torch!
	/*else if (item1.type == TORCH) {
	setColor(item1.color) -> if it has a unique color lol
	std::cout << "TORCH";
	*/
	else std::cout << "EMPTY ";
	
	setColor(Color::WHITE);
	
	std::cout << "| Player 2: ";
	const heldItem& item2 = players[1].getItemInHand();
	if (item2.type == KEY) {
		setColor(item2.color);
		std::cout << "KEY  ";
	}

	else if (item2.type == TORCH) {
		setColor(item2.color);
		std::cout << "TORCH ";
	}

	else std::cout << "EMPTY  ";
	setColor(Color::WHITE);
}

Game::Game() : players{
	Player(Placement(10,20),'$',1,0,p1Keys),
	Player(Placement(9,15),'&',0,1,p2Keys)
}
{
	init();
}

void Game::init()
{
	// Initialize ALL levels
	initLevel1(levels[0]);
	initLevel2(levels[1]);
	initLevel3(levels[2]);

	currentLevelID = 0; // Start at Level 1

	// Draw the initial room
	levels[currentLevelID].drawRoom(screen);
	screen.draw();
	levels[currentLevelID].drawTopLayer();

	for (auto& player : players) player.draw();
}

void Game::run()
{
	while (true) {
		Room& currRoom = levels[currentLevelID]; // Use CURRENT room

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

		currRoom.resetObstacles();

		for (int i = 0; i < 2; i++) {
			players[i].inputManager(key, currRoom);
		}

		for (int i = 0; i < 2; i++) {
			setColor(Color::WHITE);
			players[i].move(currRoom, &players[1 - i]);
		}

		// --- LEVEL TRANSITIONS ---
		Point p1 = players[0].getPos();
		Point p2 = players[1].getPos();

		// Level 1 -> Level 2
		if (currentLevelID == 0 && p1.x == 58 && p1.y == 18 && p2.x == 58 && p2.y == 18)
		{
			// Check for obstacle push
			Obstacle* obs = currRoom.isObstacleThere({ 58, 18 });
			if (obs) {
				Obstacle newObs = *obs;
				newObs.obstacleRoomTravel(3, 3); // Teleport to new room entrance
				levels[1].addObstacle(newObs);
				currRoom.removeObstacle({ 58, 18 });
			}

			currentLevelID = 1;
			screen.clearScreen();

			// Set players at start of Level 2
			players[0].setPos(2, 2);
			players[1].setPos(2, 3);

			levels[currentLevelID].drawRoom(screen);
			levels[currentLevelID].drawTopLayer();
		}
		// Level 2 -> Level 3
		else if (currentLevelID == 1 && p1.x == 77 && p1.y == 22 && p2.x == 77 && p2.y == 22)
		{
			currentLevelID = 2;
			screen.clearScreen();
			players[0].setPos(5, 5);
			players[1].setPos(5, 6);

			printCentered("YOU WIN!", 12);
			levels[currentLevelID].drawRoom(screen);
		}

		printHUD();
		Sleep(75);
	}

}
