#include "Game.h"

const char p1Keys[NUM_KEYS] = { 'W','S','A','D','E','Q' };
const char p2Keys[NUM_KEYS] = { 'I','K','J','L','U','O' };


void Game::initLevel1(Room& r)
{
    // ==========================================
    // 1. THE EXACT MAP DATA
    // ==========================================
    const char* layout[] = {
        "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
        "W T             W          * W",
        "W     P1        W     K2   * K5                W",
        "W               W          W                                          W",
        "WWWWWWWWWW1WWWWWWWWWWWWWWWWWWWWWWWWWWW    WWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
        "W         /D1        * ",
        "W    P2   \\D2        * ",
        "W                WWWWWWWWWWWWWWWWWWWWWWWW2WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
        "WWWWWWWWWWWWWWWWWW                             W     /D5    \\D4     \\D4    W",
        "W    K3          W                             W                           W",
        "W                W     /D3                     W                           W",
        "WWWWWWWWWWWWWWW  * \\D2                     W                           W",
        "W     W          W                             W                           W",
        "W     W          W                             W                           W",
        "W   /D3          W                             W    \\D4             /D5    W",
        "W                W                             W                           W",
        "W                * W                           W",
        "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW3WWWWWWWWWWWWWWWWW4WWWWWWWWWWWWW",
        "W   W       W         W   W                       W                        WWWW",
        "W  WW     K3  WWWW    W                                                       W",
        "W        WWWW         W                                                       5",
        "W   W       W WW      W                                                       W",
        "WW  W     W                                                   /F            WWW",
        "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
    };

    int rows = sizeof(layout) / sizeof(layout[0]);
    int offY = 1; // Start map at y=1 to leave room for HUD

    // Helper to store links safely (using Coordinates instead of Pointers)
    struct LinkInfo { Point p; int doorID; bool reqState; };
    std::vector<LinkInfo> pendingLinks;

    // Helper to store Doors by ID so we can find them later
    // (Map allows IDs 0-9)
    Door* doorRegistry[10] = { nullptr };

    // ==========================================
    // 2. PARSING LOOP
    // ==========================================
    for (int y = 0; y < rows; y++) {
        for (int x = 0; layout[y][x] != '\0'; x++) {

            // Safety Check (Prevent Out-of-Bounds Crash)
            if (x >= MAX_X || (y + offY) >= MAX_Y) continue;

            char c = layout[y][x];
            int worldY = y + offY;

            // --- WALLS ---
            if (c == 'W') {
                r.addWall(Point{ x, worldY });
            }

            // --- OBSTACLES (With Vertical Grouping) ---
            else if (c == '*') {
                // If the tile above is also '*', this is part of that obstacle.
                // We assume the top one created the object, we just add this part.
                // But since we can't easily access the previous obstacle object,
                // we will only create a NEW obstacle if the one above is NOT '*'.
                // If the one above IS '*', we skip (it was handled by the row above).

                // Wait! simpler approach for your class:
                // Create a new obstacle for every *, BUT check if we should merge?
                // Your Obstacle class doesn't support merging easily.
                // Let's stick to the strategy: 
                // "If top is *, do nothing (assume top handled it). If top is NOT *, create."

                bool topIsObs = (y > 0 && layout[y - 1][x] == '*');

                if (!topIsObs) {
                    Obstacle obs;
                    obs.addPart(Placement(x, worldY));

                    // Look ahead: Is there a * below?
                    int nextY = y + 1;
                    while (nextY < rows && layout[nextY][x] == '*') {
                        obs.addPart(Placement(x, nextY + offY));
                        nextY++;
                    }
                    r.addObstacle(obs);
                }
            }

            // --- TORCH ---
            else if (c == 'T') {
                r.addTorch(Torch(x, worldY, 6));
            }

            // --- PLAYERS ---
            else if (c == 'P') {
                char id = layout[y][x + 1];
                if (id == '1') players[0].setPos(x, worldY);
                if (id == '2') players[1].setPos(x, worldY);
            }

            // --- DOORS (1-9) ---
            else if (c >= '1' && c <= '9') {
                int id = c - '0';
                Color color = Color::WHITE;
                if (id == 2) color = Color::MAGENTA;
                if (id == 3) color = Color::RED;
                if (id == 4) color = Color::CYAN;
                if (id == 5) color = Color::GREEN; // Exit

                Door d(x, worldY, id, color);

                // Add Key Requirements
                if (id == 2) d.addRequiredKey(2);
                if (id == 3) d.addRequiredKey(3); // Based on K3 existing
                if (id == 5) d.addRequiredKey(5);

                r.addDoor(d);

                // Save pointer for linking. 
                // We search for it immediately because addDoor makes a copy.
                doorRegistry[id] = r.isDoorThere(Point{ x, worldY });
            }

            // --- KEYS (Kn) ---
            else if (c == 'K') {
                char dChar = layout[y][x + 1];
                if (dChar >= '0' && dChar <= '9') {
                    int id = dChar - '0';
                    Color color = Color::WHITE;
                    if (id == 2) color = Color::MAGENTA;
                    if (id == 3) color = Color::RED;
                    if (id == 5) color = Color::GREEN;

                    r.addKey(Key(x, worldY, id, color));
                }
            }

            // --- SWITCHES (/ or \) ---
            else if (c == '/' || c == '\\') {
                bool isFake = (layout[y][x + 1] == 'F');
                bool reqOn = (c == '/');
                int doorID = -1;

                // Check for label like D1, D2
                if (layout[y][x + 1] == 'D') {
                    doorID = layout[y][x + 2] - '0';
                }

                // Create the switch
                int sid = isFake ? 99 : (y * 100 + x); // Unique ID based on position
                Switch s(x, worldY, sid);
                r.addSwitch(s);

                // If it controls a door, save the coordinate to link later
                if (doorID != -1) {
                    pendingLinks.push_back({ Point{x, worldY}, doorID, reqOn });
                }
            }
        }
    }

    // ==========================================
    // 3. APPLY LINKS (Safe Phase)
    // ==========================================
    // Now that all switches are created and the vector is stable, 
    // we can safely get their pointers.
    for (const auto& link : pendingLinks) {
        Switch* sPtr = r.isSwitchThere(link.p);

        if (sPtr != nullptr && doorRegistry[link.doorID] != nullptr) {
            doorRegistry[link.doorID]->addRequiredSwitch(sPtr, link.reqState);
        }
    }
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
