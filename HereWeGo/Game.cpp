#include "Game.h"

const char p1Keys[NUM_KEYS] = { 'W','X','A','D','S','E' };
const char p2Keys[NUM_KEYS] = { 'I','M','J','L','K','O' };
void Game::initLevel1(Room& r)
{
    // ==========================================
    // 1. BOUNDARIES (Box 0-78, Y 1-23)
    // ==========================================
    for (int x = 0; x < 79; x++) { r.addWall(Point{ x, 1 }); r.addWall(Point{ x, 23 }); }
    for (int y = 1; y < 24; y++) { r.addWall(Point{ 0, y }); r.addWall(Point{ 78, y }); }

    // ==========================================
    // 2. INTERNAL WALLS
    // ==========================================
    // Vertical Separator for P1 area (x=16, y=2-4)
    for (int y = 2; y <= 4; y++) r.addWall(Point{ 16, y });

    // Horizontal Separator below P1 (y=5), Gap for Door 1 at x=10
    for (int x = 1; x <= 40; x++) {
        if (x != 10) r.addWall(Point{ x, 5 });
    }
    // Continue that wall to the right side if needed, or leave open as per map
    // Map shows wall continues to right side
    for (int x = 41; x < 78; x++) r.addWall(Point{ x, 5 });

    // Vertical Separator for P2 area (x=17, y=5-7)
    // Note: Starts at y=5 (wall) goes down
    for (int y = 5; y <= 7; y++) r.addWall(Point{ 17, y });

    // Horizontal Separator below P2 (y=8), Gap for Door 2 at x=45
    for (int x = 1; x < 78; x++) {
        if (x != 45) r.addWall(Point{ x, 8 });
    }

    // Horizontal Separator at bottom (y=18), Gaps for D3(45), D4(65)
    for (int x = 1; x < 78; x++) {
        if (x != 45 && x != 65) r.addWall(Point{ x, 18 });
    }

    // Bottom Maze Structures
    // Vertical segments around y=19-22
    for (int y = 19; y <= 22; y++) {
        r.addWall(Point{ 4, y });
        r.addWall(Point{ 12, y });
    }
    // Small horizontal bits in maze
    r.addWall(Point{ 8, 20 }); r.addWall(Point{ 9, 20 });
    r.addWall(Point{ 15, 20 }); r.addWall(Point{ 16, 20 }); r.addWall(Point{ 17, 20 });

    // ==========================================
    // 3. DOORS
    // ==========================================
    // Door 1 (White) at (10,5)
    Door d1(10, 5, 1, Color::WHITE);
    r.addDoor(d1);

    // Door 2 (Magenta) at (45,8) - Requires Key 2
    Door d2(45, 8, 2, Color::MAGENTA);
    d2.addRequiredKey(2);
    r.addDoor(d2);

    // Door 3 (Red) at (45,18) - Requires Key 3
    Door d3(45, 18, 3, Color::RED);
    d3.addRequiredKey(3);
    r.addDoor(d3);

    // Door 4 (Cyan) at (65,18)
    Door d4(65, 18, 4, Color::CYAN);
    r.addDoor(d4);

    // Door 5 (Green - Exit) at (78,21) or close to it
    Door d5(78, 21, 5, Color::GREEN);
    d5.addRequiredKey(5);
    r.addDoor(d5);

    // ==========================================
    // 4. SWITCHES
    // ==========================================

    // -- P2 Area Switches (The "Same Switch" Group) --
    // Top Part: /D1 at (10,6). Controls D1.
    Switch sD1(10, 6, 101);
    r.addSwitch(sD1);

    // Bottom Part: \D2 at (10,7). Controls D2.
    Switch sD2(10, 7, 102);
    r.addSwitch(sD2);

    // Link them!
    // Note: We must fetch the pointer from the room to link correctly
    r.isDoorThere(Point{ 10, 5 })->addRequiredSwitch(r.isSwitchThere(Point{ 10, 6 }), true);  // D1 needs S(10,6) ON
    r.isDoorThere(Point{ 45, 8 })->addRequiredSwitch(r.isSwitchThere(Point{ 10, 7 }), false); // D2 needs S(10,7) OFF

    // -- Middle Switches --
    // /D5 at (38,9)
    Switch sD5(38, 9, 103);
    r.addSwitch(sD5);
    r.isDoorThere(Point{ 78, 21 })->addRequiredSwitch(r.isSwitchThere(Point{ 38, 9 }), true); // D5 needs S ON

    // \D4 at (45,9)
    Switch sD4a(45, 9, 104);
    r.addSwitch(sD4a);
    r.isDoorThere(Point{ 65, 18 })->addRequiredSwitch(r.isSwitchThere(Point{ 45, 9 }), false); // D4 needs S OFF

    // \D4 at (52,9)
    Switch sD4b(52, 9, 105);
    r.addSwitch(sD4b);
    r.isDoorThere(Point{ 65, 18 })->addRequiredSwitch(r.isSwitchThere(Point{ 52, 9 }), false); // D4 needs S OFF

    // -- Bottom Switches --
    // /D3 at (4,15)
    Switch sD3(4, 15, 106);
    r.addSwitch(sD3);
    // Link D3 to this switch if required? Text implies /D3 relationship.
    r.isDoorThere(Point{ 45, 18 })->addRequiredSwitch(r.isSwitchThere(Point{ 4, 15 }), true);

    // Fake Switch /F at (60,22)
    r.addSwitch(Switch(60, 22, 99)); // ID 99 triggers "Gotcha"

    // ==========================================
    // 5. OBSTACLES
    // ==========================================
    // Group 1: Top (y=2,3) at x=27
    Obstacle obs1;
    obs1.addPart(Placement(27, 2));
    obs1.addPart(Placement(27, 3));
    r.addObstacle(obs1);

    // Group 2: Middle (y=6,7) at x=27
    Obstacle obs2;
    obs2.addPart(Placement(27, 6));
    obs2.addPart(Placement(27, 7));
    r.addObstacle(obs2);

    // Group 3: Lower (y=12) at x=27
    Obstacle obs3;
    obs3.addPart(Placement(27, 12));
    r.addObstacle(obs3);

    // Group 4: Bottom (y=17) at x=27
    Obstacle obs4;
    obs4.addPart(Placement(27, 17));
    r.addObstacle(obs4);

    // ==========================================
    // 6. KEYS & ITEMS
    // ==========================================
    r.addTorch(Torch(2, 2, 6));

    r.addKey(Key(22, 3, 2, Color::MAGENTA)); // K2
    r.addKey(Key(37, 3, 5, Color::GREEN));   // K5
    r.addKey(Key(5, 10, 3, Color::RED));     // K3 (Left)
    r.addKey(Key(10, 20, 3, Color::RED));    // K3 (Bottom maze)

    // ==========================================
    // 7. PLAYERS
    // ==========================================
    //players[0].setPos(6, 3);
    //players[1].setPos(5, 7);

	players[0].setPos(6, 3);
	players[1].setPos(6, 7);

	   


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
	Player(Placement(10,20),'$',0,0,p1Keys),
	Player(Placement(9,15),'&',0,0,p2Keys)
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
