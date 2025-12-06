#include "Game.h"

const char p1Keys[NUM_KEYS] = { 'W','S','A','D','E','Q' };
const char p2Keys[NUM_KEYS] = { 'I','K','J','L','U','O' };
void Game::initLevel1(Room& r)
{
    // ==========================================
    // 1. RAW MAP DATA
    // ==========================================
    const char* layout[] = {
        R"(WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW)",
        R"(W T             W          * W)",
        R"(W     P1        W     K2   * K5                W)",
        R"(W               W          W                                          W)",
        R"(WWWWWWWWWW1WWWWWWWWWWWWWWWWWWWWWWWWWWW    WWWWWWWWWWWWWWWWWWWWWWWWWWWWW)",
        R"(W                    * )",
        R"(W    P2   \D2        * )",
        R"(W                WWWWWWWWWWWWWWWWWWWWWWWW2WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW)",
        R"(WWWWWWWWWWWWWWWWWW                             W     /D5    \D4     \D4    W)",
        R"(W    K3          W                             W                           W)",
        R"(W                W     /D3                     W                           W)",
        R"(WWWWWWWWWWWWWWW  * \D2                     W                           W)",
        R"(W     W          W                             W                           W)",
        R"(W     W          W                             W                           W)",
        R"(W   /D3          W                             W    \D4             /D5    W)",
        R"(W                W                             W                           W)",
        R"(W                * W                           W)",
        R"(WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW3WWWWWWWWWWWWWWWWW4WWWWWWWWWWWWW)",
        R"(W   W       W         W   W                       W                        WWWW)",
        R"(W  WW     K3  WWWW    W                                                       W)",
        R"(W        WWWW         W                                                       5)",
        R"(W   W       W WW      W                                                       W)",
        R"(WW  W     W                                                   /F            WWW)",
        R"(WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW)"
    };

    int rows = sizeof(layout) / sizeof(layout[0]);
    int offY = 1;

    struct LinkInfo { Point p; int doorID; bool reqState; };
    std::vector<LinkInfo> pendingLinks;

    Point doorLocs[10];
    bool doorExists[10] = { false };

    int switchGridIDs[MAX_Y][MAX_X];
    for (int i = 0; i < MAX_Y; i++) for (int j = 0; j < MAX_X; j++) switchGridIDs[i][j] = -1;

    // ==========================================
    // 2. PARSE OBJECTS
    // ==========================================
    for (int y = 0; y < rows; y++) {
        for (int x = 0; layout[y][x] != '\0'; x++) {

            if (x >= MAX_X || (y + offY) >= MAX_Y) continue;

            char c = layout[y][x];
            int worldY = y + offY;

            // --- WALLS ---
            if (c == 'W') r.addWall(Point{ x, worldY });

            // --- OBSTACLES (Vertical Grouping) ---
            else if (c == '*') {
                bool topIsObs = (y > 0 && layout[y - 1][x] == '*');
                if (!topIsObs) {
                    Obstacle obs;
                    obs.addPart(Placement(x, worldY));
                    int nextY = y + 1;
                    while (nextY < rows && layout[nextY][x] == '*') {
                        obs.addPart(Placement(x, nextY + offY));
                        nextY++;
                    }
                    r.addObstacle(obs);
                }
            }

            // --- TORCH ---
            else if (c == 'T') r.addTorch(Torch(x, worldY, 6));

            // --- PLAYERS (P1/P2) ---
            else if (c == 'P') {
                char pid = layout[y][x + 1];
                if (pid == '1') players[0].setPos(x, worldY);
                if (pid == '2') players[1].setPos(x, worldY);
                x++; // Skip ID so it isn't drawn or parsed as a Door
            }

            // --- DOORS (1-9) ---
            else if (c >= '1' && c <= '9') {
                int id = c - '0';
                Color col = Color::WHITE;
                if (id == 2) col = Color::MAGENTA;
                if (id == 3) col = Color::RED;
                if (id == 4) col = Color::CYAN;
                if (id == 5) col = Color::GREEN;

                Door d(x, worldY, id, col);
                // Key Logic
                if (id == 2) d.addRequiredKey(2);
                if (id == 3) d.addRequiredKey(3);
                if (id == 5) d.addRequiredKey(5);

                r.addDoor(d);
                doorLocs[id] = { x, worldY };
                doorExists[id] = true;
            }

            // --- KEYS (K) ---
            else if (c == 'K') {
                char kid = layout[y][x + 1];
                if (isdigit(kid)) {
                    int id = kid - '0';
                    Color col = Color::WHITE;
                    if (id == 2) col = Color::MAGENTA;
                    if (id == 3) col = Color::RED;
                    if (id == 5) col = Color::GREEN;
                    r.addKey(Key(x, worldY, id, col));
                    x++; // CRITICAL FIX: Skip the digit so it isn't parsed as a Door!
                }
            }

            // --- SWITCHES (/ or \) ---
            else if (c == '/' || c == '\\') {
                bool isFake = (layout[y][x + 1] == 'F');

                // UNIFIED LOGIC: All switches must be ON to trigger their door.
                // This means you step on '\' -> it becomes '/' -> Door Opens.
                // This fixes the "Non-reversible / Starts Open" confusion.
                bool reqOn = true;

                int doorID = -1;
                if (layout[y][x + 1] == 'D') {
                    doorID = layout[y][x + 2] - '0';
                    x += 2; // CRITICAL FIX: Skip 'D' and ID
                }
                else if (isFake) {
                    x++; // Skip 'F'
                }

                // Vertical Grouping Logic
                int sid = -1;
                if (y > 0 && switchGridIDs[y - 1][x] != -1) {
                    sid = switchGridIDs[y - 1][x];
                }
                else {
                    sid = isFake ? 99 : (y * 100 + x);
                }
                switchGridIDs[y][x] = sid;

                Switch s(x, worldY, sid);
                r.addSwitch(s);

                if (doorID != -1) {
                    pendingLinks.push_back({ Point{x, worldY}, doorID, reqOn });
                }
            }
        }
    }

    // ==========================================
    // 3. PASS 2: LINKING
    // ==========================================
    for (const auto& link : pendingLinks) {
        if (!doorExists[link.doorID]) continue;

        Switch* sPtr = r.isSwitchThere(link.p);
        Door* dPtr = r.isDoorThere(doorLocs[link.doorID]);

        if (sPtr && dPtr) {
            dPtr->addRequiredSwitch(sPtr, link.reqState);
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
