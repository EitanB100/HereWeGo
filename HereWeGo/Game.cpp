#include "Game.h"
#include <iostream>
#include <sstream> 
#include <iomanip>  
#include <string>
char p1Keys[NUM_KEYS] = { 'W','X','A','D','S','E' };
char p2Keys[NUM_KEYS] = { 'I','M','J','L','K','O' };




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
	
	else std::cout << "EMPTY ";
	setColor(Color::WHITE);

	std::cout <<" Hitpoints " << players[0].getHP() << "/15 ";
	
	std::cout << "| Player 2: ";
	const heldItem& item2 = players[1].getItemInHand();
	if (item2.type == KEY) {
		setColor(item2.color);
		std::cout << "KEY ";
	}

	else if (item2.type == TORCH) {
		setColor(item2.color);
		std::cout << "TORCH ";
	}

	else std::cout << "EMPTY ";
	setColor(Color::WHITE);
	std::cout << " Hitpoints " << players[1].getHP() << "/15 ";
}


void Game::printTimer() {
	auto currentTime = std::chrono::steady_clock::now();

	// Calculate Total Time from the very beginning of the game
	auto totalElapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

	// Calculate Level Time from the last reset (start of level)
	auto levelElapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - levelStartTime).count();

	// Lambda to format seconds into MM:SS
	auto formatTime = [](long long totalSeconds) -> std::string {
		int m = static_cast<int>((totalSeconds % 3600) / 60);
		int s = static_cast<int>(totalSeconds % 60);
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(2) << m << ":"
			<< std::setfill('0') << std::setw(2) << s;
		return ss.str();
		};

	gotoxy(0, 1);
	setColor(Color::YELLOW);
	std::cout << "Total Time: " << formatTime(totalElapsed)
		<< " | Level Time: " << formatTime(levelElapsed) << " ";

	printScore();
}

void Game::resetLevelTimer() {
	levelStartTime = std::chrono::steady_clock::now();
}
void Game::printScore() {
	setColor(Color::YELLOW);
	gotoxy(55, 1);
	std::cout << "Score: " << score;
}

Game::Game() : players{
	Player(Placement(10,20),'$',0,0,p1Keys),
	Player(Placement(9,15),'&',0,0,p2Keys)
}
{
	exitPoints[0] = exitPoints[1] = { 79,22 };
	exitPoints[2] = { -1,-1 }; //final room
	exitPoints[3] = { 79,23 }; //dummy for test room
	p1StartPoints[0] = { 6,3 };
	p1StartPoints[1] = { 2,2};
	p1StartPoints[2] = { 65,5 };
	
	p2StartPoints[0] = { 6,7 };
	p2StartPoints[1] = { 4,2 };
	p2StartPoints[2] = { 70,16 };
	// for test room
	p1StartPoints[3] = { 4, 3 };
	p2StartPoints[3] = { 4, 4 };
	startTime = std::chrono::steady_clock::now();
	init();
}

void Game::settingsMenu() {
	bool inSettings = true;
	while (inSettings) {
		system("cls");
		printCentered("Change Players Keys", 3);
		printCentered("(1) Change P1 keys", 5);
		printCentered("(2) Change P2 keys", 7);
		printCentered("(9) Back to menu", 9);

		char selection = _getch();

		switch (selection)
		{
		case '1':
		{
			updatePlayerKeys(p1Keys, 1);
			break;
		}
		case '2':
		{
			updatePlayerKeys(p2Keys, 2);
			break;
		}

		case '9':
		{
			inSettings = false;
			break;
		}
		}
	}
}

void Game::updatePlayerKeys(char keys[], int playerNum) {
	const char* commandNames[] = { "UP", "DOWN", "LEFT", "RIGHT", "STAY", "DISPOSE" , ""};
	bool finishing = false;

	while (!finishing) {
		system("cls");
		printCentered("PLAYER " + std::to_string(playerNum) + " CONTROLS", 2);

		// Display current mapping
		for (int i = 0; i < NUM_KEYS; i++) {
			std::string row = "(" + std::to_string(i + 1) + ") " + commandNames[i] + ": [" + keys[i] + "]";
			printCentered(row, 4 + i);
		}

		printCentered("(ESC or Enter) Save and Exit", 12);
		printCentered("Select a number to rebind:", 14);

		char choice = _getch();

		if (choice == ESC || choice == ENTER) {
			finishing = true;
		}
		else {
			int index = -1;
			// Check if choice is a number 1-6
			if (choice >= '1' && choice <= '6') {
				index = choice - '1';
			}
			// Check if choice is the actual command key
			else {
				for (int i = 0; i < NUM_KEYS; i++) {
					if (toupper(choice) == keys[i]) {
						index = i;
						break;
					}
				}
			}
			if (index != -1) {
				system("cls");
				printCentered("Changing " + std::string(commandNames[index]), 10);
				printCentered("Press new key...", 12);

				char newKey = _getch();

				// Validation: Check if newKey is ESC or ENTER
				if (newKey == ESC || newKey == ENTER) {
					printCentered("ERROR: ESC and ENTER are reserved!", 14);
					Sleep(250);
				}
				else {
					keys[index] = toupper(newKey);
					printCentered("SAVED!", 14);
					Sleep(150);
				}
			}
		}
	}
}

void Game::init()
{
    // Initialize ALL levels
    initLevel1Props(levels[0]);
    initLevel2Props(levels[1]);
    initLevel3Props(levels[2]);
	initLevel4Props(levels[3]);

	currentLevelID = 0;// Start at Level 1
	setGame(currentLevelID , true);
}

void Game::setGame(int level , bool firstSettings) {
	screen.clearScreen();

	switch (level) {
	case 0:
		screen.Lvl1Screen();
		break;
	case 1:
		screen.Lvl2Screen();
		break;
	case 2:
		screen.Lvl3Screen();
		break;
	case 3:
		screen.Lvl4Screen();
		break;
	default:
		screen.Lvl1Screen();
		break;
	}


	if (level < 0 || level >= ROOM_AMOUNT) level = 0;

	currentLevelID = level;

	levels[currentLevelID].loadFromScreen(screen);
	levels[currentLevelID].drawRoom(screen);
	if (!(firstSettings)) {
		screen.draw();
		levels[currentLevelID].drawTopLayer();
	}

	players[0].setPos(p1StartPoints[currentLevelID]);
	players[1].setPos(p2StartPoints[currentLevelID]);

	for (auto& player : players) {
		player.setDirection(0, 0);
		player.setFinished(false);
		player.draw();
	}
}


void Game::run()
{
	resetLevelTimer();
	bool boomDustCleaningNeeded = false;
	while (true) {
		Room& currRoom = levels[currentLevelID]; 
		char key = 0;
		
		
		if (_kbhit()) {
			key = _getch();
			if (key == ESC) {
				key = _getch();
				if (key == 'h' || key == 'H') {
					setColor(Color::WHITE);
					screen.clearScreen();
					break; //main menu exit
				}
			}
		}
		
		currRoom.resetObstacles();
		Point currentExitPoint = exitPoints[currentLevelID];

		//update loop
		for (int i = 0; i < PLAYER_AMOUNT; i++) {
			Player& p = players[i];
			Player& other = players[1 - i];

			p.inputManager(key, currRoom);
			setColor(Color::WHITE);

			p.updateSpringPhysics(currRoom, &other);
			p.move(currRoom, &other);
			//check level completion for a player
			if (currentExitPoint.x != -1 && p.getPos() == currentExitPoint) {
				if (!p.isFinished()) {
					p.setFinished(true);
					gotoxy(50, 0);
					std::cout << "Player " << p.getSymbol() << " Is waiting...";
				}
			}
		}

		checkLevelTransition(currentLevelID, players[0].getPos(), players[1].getPos());
		
		if (currentLevelID == 2) {
			Point p1 = players[0].getPos();
			Point p2 = players[1].getPos();

			Point winA = { 37,1 };
			Point winB = { 37,2 };
			if ((p1 == winA && p2 == winB) || (p1 == winB && p2 == winA)){

				setColor(Color::GREEN);
				printCentered("THANKS FOR PLAYING!", 12);

				levels[currentLevelID].drawRoom(screen);
			}
			gotoxy(45, 0);
			setColor(Color::GREEN);
			std::cout << "Go through the top door to finish!";
			setColor(Color::WHITE);
		}
		if (boomDustCleaningNeeded) {
			currRoom.clearExplosions();
			currRoom.drawRoom(screen);
			
			screen.draw(); 
			currRoom.drawTopLayer();
			boomDustCleaningNeeded = false;
		}
		
		currRoom.updateBombs(players, PLAYER_AMOUNT , screen); // update bombs
		
		if (currRoom.hasExplosions()) {
			boomDustCleaningNeeded = true;
			
		}

		bool gameOver = false;
		for (int i = 0; i < PLAYER_AMOUNT; i++) {
			players[i].draw();
			if (players[i].isDead()) {
				gameOver = true;
				break;
			}
		}

		if (gameOver) {
			screen.clearScreen();
			gotoxy(35, 10);
			//setColor(Color::RED);
			//std::cout << "G A M E   O V E R";
			printCentered("GAME OVER", 10);
			//gotoxy(30, 12);
			setColor(Color::WHITE);
			//std::cout << "Press 'H' to Exit to Menu";
			printCentered("LOL U SUCK", 12);
			while (true) {
				if (_kbhit()) {
					char choice = _getch();
					if (choice == 'h' || choice == 'H') {
						// This exits Game::run and goes back to main()
						setColor(Color::WHITE);
						return;
					}
				}
			}
		}
		//HUD renderer
		printHUD();
		printTimer();
		Sleep(100);
	}

}

void Game::checkLevelTransition(int& currentLevel, Point p1, Point p2)
{
	Point exit = exitPoints[currentLevel];
	if (exit.x == -1) return; // No standard exit

	if (p1 == exit && p2 == exit)
	{
		// 1. Calculate Score BEFORE resetting the level timer
		// This ensures the player is rewarded based on the time spent on the level they just finished
		auto now = std::chrono::steady_clock::now();
		int levelSeconds = std::chrono::duration_cast<std::chrono::seconds>(now - levelStartTime).count();
		score += (100000 / (levelSeconds + 1));

		// 2. Handle Special Case (Obstacle carry over)
		if (currentLevel == 0) {
			Obstacle* obs = levels[0].isObstacleThere({ 58, 18 });
			if (obs) {
				Obstacle newObs = *obs;
				newObs.obstacleRoomTravel(3, 3);
				levels[1].addObstacle(newObs);
				levels[0].removeObstacle({ 58, 18 });
			}
		}

		// 3. Advance Level and Reset Timer
		if (currentLevel < ROOM_AMOUNT - 1) {
			currentLevel++;
		}

		// Reset the game state for the new currentLevel
		setGame(currentLevel, false );

		// RESET TIMER HERE: This sets levelStartTime to 'now', making the HUD show 00:00
		resetLevelTimer();

		// Update HUD
		printTimer(); // Force a timer update immediately so it doesn't show the old time for 75ms
	}
}

void Game::initLevel1Props(Room& r) {

	// 1. DOORS
	// ==========================================
	// Door 1 (White) at (12,6) //requiers swtich
	Door d1(12, 6, 1, Color::WHITE);
	r.addDoor(d1);

	// Door 2 (Magenta) at (44,9) - Requires Key 2
	Door d2(44, 9, 2, Color::YELLOW);
	d2.addRequiredKey(2);
	r.addDoor(d2);

	// Door 3 (Red) at (45,17) - Requires Key 3
	Door d3(44, 17, 3, Color::RED);
	d3.addRequiredKey(3);
	r.addDoor(d3);

	// Door 4 (Cyan) at (63,17)
	Door d4(63, 17, 4, Color::CYAN);
	r.addDoor(d4);

	// Door 5 (Green - Exit) at (79,22) or close to it
	Door d5(79, 22, 5, Color::GREEN);
	d5.addRequiredKey(4);
	d5.addRequiredKey(5);
	r.addDoor(d5);

	// ==========================================
	// 2. SWITCHES
	// ==========================================

	// -- Area1-2 Switches (The "Same Switch" Group) --
	Switch* sD1 = new Switch(12, 8, 101);
	r.addSwitch(sD1);
	r.isDoorThere(Point{ 12, 6 })->addRequiredSwitch(r.getSwitchByID(101), true);  // D1 needs S(12,8) ON
	r.isDoorThere(Point{ 44, 9 })->addRequiredSwitch(r.getSwitchByID(101), false); // D2 needs S(12,8) OFF
	Switch* sD2 = new Switch(23, 11, 102);
	r.addSwitch(sD2);
	r.isDoorThere(Point{ 44, 9 })->addRequiredSwitch(r.getSwitchByID(102), false); // D2 needs S(23,11) OFF
	r.isDoorThere(Point{ 44, 17 })->addRequiredSwitch(r.getSwitchByID(102), true); // D3 needs S(23,11) ON
	Switch* sD3 = new Switch(10, 14, 103);
	r.addSwitch(sD3);
	r.isDoorThere(Point{ 44, 17 })->addRequiredSwitch(r.getSwitchByID(103), true); // D3 needs S(10,14) ON
	//-- Area3 Switches and 4--
	Switch* sD4 = new Switch(50, 14, 104);
	Switch* sD5 = new Switch(57, 10, 105);
	Switch* sD6 = new Switch(64, 10, 106);
	Switch* sD7 = new Switch(71, 10, 107);
	Switch* sD8 = new Switch(78, 14, 108);
	r.addSwitch(sD4);
	r.addSwitch(sD5);
	r.addSwitch(sD6);
	r.addSwitch(sD7);
	r.addSwitch(sD8);
	r.isDoorThere(Point{ 63, 17 })->addRequiredSwitch(r.getSwitchByID(104), false); // D4 needs S(49,14) OFF
	r.isDoorThere(Point{ 63, 17 })->addRequiredSwitch(r.getSwitchByID(106), false); // D4 needs S(64,10) OFF
	r.isDoorThere(Point{ 63, 17 })->addRequiredSwitch(r.getSwitchByID(107), false); // D4 needs S(71,10) OFF
	r.isDoorThere(Point{ 79, 22 })->addRequiredSwitch(r.getSwitchByID(105), true);  // D5 needs S(56,10) ON
	r.isDoorThere(Point{ 79, 22 })->addRequiredSwitch(r.getSwitchByID(108), true); // D5 needs S(78,14) On

	// ==========================================
	// 3. OBSTACLES
	// ==========================================
	// Group 1: Top (y=2,3) at x=28
	Obstacle obs1;
	obs1.addPart(Placement(28, 2));
	obs1.addPart(Placement(28, 3));
	r.addObstacle(obs1);

	// Group 2: Middle (y=7,8) at x=21
	Obstacle obs2;
	obs2.addPart(Placement(21, 7));
	obs2.addPart(Placement(21, 8));
	r.addObstacle(obs2);

	// Group 3: Lower (y=13) at x=22
	Obstacle obs3;
	obs3.addPart(Placement(22, 13));
	r.addObstacle(obs3);

	// Group 4: Bottom (y=16) at x=22
	Obstacle obs4;
	obs4.addPart(Placement(22, 16));
	r.addObstacle(obs4);

	// ==========================================
	// 4. KEYS & ITEMS
	// ==========================================
	r.addTorch(Torch(2, 2, 6));

	r.addKey(Key(22, 4, 2, Color::YELLOW)); // K2
	r.addKey(Key(37, 3, 4, Color::GREEN));   // K5
	r.addKey(Key(5, 10, 3, Color::RED));     // K3 (Left)
	r.addKey(Key(12, 19, 5, Color::GREEN));    // K5 (Bottom maze)

	// ==========================================
	// 5. PLAYERS
	// ==========================================
	players[0].setPos(p1StartPoints[0]);
	players[1].setPos(p2StartPoints[0]);

}

void Game::initLevel2Props(Room& r){
	// 1. DOORS
	Door d1(79, 22, 1, Color::GREEN);
	
	// ==========================================
	// ==========================================
	// 2. SWITCHES
	// ==========================================
	for (int i = 1; i <= 16; i++) {
		Switch* temp = new Switch(i, 23, i);
		r.addSwitch(temp);
		if (i % 2 == 0)
			temp->toggleState();
	}
	for (int i = 50; i <= 65; i++) {
		Switch* realS = new Switch(i, 18, i);
		r.addSwitch(realS);
		if (i % 2 == 1)
			d1.addRequiredSwitch(r.getSwitchByID(i), true);
		else
			d1.addRequiredSwitch(r.getSwitchByID(i), false);
	}
	d1.UpdatedFromSwitch();
	r.addDoor(d1);
	// ==========================================
	// 3. OBSTACLES
	// ==========================================
	for (int i = 20; i < 80; i += 20) {
		Obstacle top;
		top.addPart(Placement(i, 2));
		top.addPart(Placement(i, 3));
		r.addObstacle(top);
	}
	for (int i = 11; i < 70; i += 20) {
		Obstacle mid1;
		mid1.addPart(Placement(i, 5));
		mid1.addPart(Placement(i, 6));
		r.addObstacle(mid1);
	}
	for (int i = 34; i < 80; i += 20) {
		Obstacle mid2;
		mid2.addPart(Placement(i, 8));
		mid2.addPart(Placement(i, 9));
		r.addObstacle(mid2);
	}
	for (int i = 24; i < 80; i += 20) {
		Obstacle lower;
		lower.addPart(Placement(i, 11));
		lower.addPart(Placement(i, 12));
		r.addObstacle(lower);
	}
	// ==========================================
	// 4. KEYS & ITEMS
		r.addTorch(Torch(31, 20, 2));
	}

void Game::initLevel3Props(Room& r) {
	//  1.DOORS
	Door d1(64, 16, 4, Color::GREEN);
	Door d2(52, 22, 3, Color::RED);
	Door d3(36, 22, 6, Color::MAGENTA);
	Door d4(22, 22, 7, Color::YELLOW);
	Door d5(9, 20, 5, Color::CYAN);
	Door d6(9, 8, 2, Color::GREEN);
	Door d7(22, 5, 1, Color::RED);
	// ==========================================
	// 2. SWITCHES
	Switch* sD1 = new Switch(64, 2, 201);
	r.addSwitch(sD1);
	sD1->setSeen();
	Switch* sD2 = new Switch(65, 2, 202);
	r.addSwitch(sD2);
	sD2->setSeen();
	Switch* sD3 = new Switch(66, 2, 203);
	r.addSwitch(sD3);
	sD3->setSeen();


	//Door 1
	d1.addRequiredSwitch(r.getSwitchByID(201), true);  // Left switch must be TRUE
	d1.addRequiredSwitch(r.getSwitchByID(202), false);  // Middle switch must be TRUE
	d1.addRequiredSwitch(r.getSwitchByID(203), false); // Right switch must be FALSE

	// Door 2
	d2.addRequiredSwitch(r.getSwitchByID(201), false);
	d2.addRequiredSwitch(r.getSwitchByID(202), true);
	d2.addRequiredSwitch(r.getSwitchByID(203), true);

	// Door 3
	d3.addRequiredSwitch(r.getSwitchByID(201), true);
	d3.addRequiredSwitch(r.getSwitchByID(202), true);
	d3.addRequiredSwitch(r.getSwitchByID(203), false);

	// Door 4
	d4.addRequiredSwitch(r.getSwitchByID(201), true);
	d4.addRequiredSwitch(r.getSwitchByID(202), true);
	d4.addRequiredSwitch(r.getSwitchByID(203), true);

	// Door 5
	d5.addRequiredSwitch(r.getSwitchByID(201), true);
	d5.addRequiredSwitch(r.getSwitchByID(202), false);
	d5.addRequiredSwitch(r.getSwitchByID(203), true);

	// Door 6
	d6.addRequiredSwitch(r.getSwitchByID(201), false);
	d6.addRequiredSwitch(r.getSwitchByID(202), true);
	d6.addRequiredSwitch(r.getSwitchByID(203), false);

	// Door 7
	d7.addRequiredSwitch(r.getSwitchByID(201), false);
	d7.addRequiredSwitch(r.getSwitchByID(202), false);
	d7.addRequiredSwitch(r.getSwitchByID(203), true);

	// ==========================================
	r.addDoor(d1);
	r.addDoor(d2);
	r.addDoor(d3);
	r.addDoor(d4);
	r.addDoor(d5);
	r.addDoor(d6);
	r.addDoor(d7);


	
}

void Game::initLevel4Props(Room& r) {
	// === TEST LAB SETUP ===

	// 1. Keys & Doors (Top Right)
	r.addKey(Key(40, 3, 1, Color::RED));
	r.addDoor(Door(42, 3, 1, Color::RED));

	// Switch Door
	Switch* sw1 = new Switch(60, 3, 401);
	r.addSwitch(sw1);
	Door dSwitch(65, 3, 0, Color::CYAN);
	dSwitch.addRequiredSwitch(r.getSwitchByID(401), true);
	r.addDoor(dSwitch);

	// 2. Spring Chaining (Middle Left)
	Spring s1({ 1,0 }); 
	s1.addPart(7, 10); 
	s1.addPart(6, 10); 
	s1.addPart(5, 10);
	r.addSpring(s1);

	Spring s2({ 0,-1 });
	s2.addPart(12, 10);
	s2.addPart(12, 11);
	s2.addPart(12, 12);
	r.addSpring(s2);

	r.addWall({ 15, 4 });

	// 3. Obstacles (Bottom)
	Obstacle box;
	box.addPart(Placement(10, 18));
	box.addPart(Placement(11, 18));
	r.addObstacle(box);

	// 4. Torch
	r.addTorch(Torch(35, 18, 5));

	// === NEW OBJECTS ADDED ===

	// A cage area defined by walls
	for (int x = 50; x < 60; x++) { r.addWall({ x, 15 }); r.addWall({ x, 22 }); }
	for (int y = 15; y <= 22; y++) { r.addWall({ 50, y }); r.addWall({ 60, y }); }

	// A door protecting the cage entrance
	Door cageDoor(55, 15, 9, Color::MAGENTA);
	cageDoor.addRequiredKey(9);
	r.addDoor(cageDoor);

	// The key for the cage (hidden behind obstacles)
	r.addKey(Key(70, 20, 9, Color::MAGENTA));

	// Obstacles blocking the key
	Obstacle block;
	block.addPart(Placement(68, 20));
	block.addPart(Placement(68, 21));
	r.addObstacle(block);

	// Extra switch for atmosphere
	Switch* lightSwitch = new Switch(55, 18, 402);
	r.addSwitch(lightSwitch);
	// === NEW TRICKY OBJECTS ===

	// 1. "Ping-Pong" Springs (Bottom Right)
	// Spring Facing Left at (75, 12)
	Spring sPing({ -1, 0 });
	sPing.addPart(75, 12); // Tip
	sPing.addPart(76, 12); // Base
	r.addSpring(sPing);

	// Spring Facing Right at (70, 12)
	Spring sPong({ 1, 0 });
	sPong.addPart(70, 12); // Tip
	sPong.addPart(69, 12); // Base
	r.addSpring(sPong);

	// 2. The Wall Slam (Top Left)
	// Spring pointing UP into a wall with only 1 space gap
	Spring sSlam({ 0, -1 });
	sSlam.addPart(5, 5); // Tip
	sSlam.addPart(5, 6); // Base
	r.addSpring(sSlam);
	r.addWall({ 5, 3 }); // The wall you will hit

	// 3. Locked Door Launch (Bottom Left)
	// Spring pointing DOWN into a door
	Spring sDoor({ 0, 1 });
	sDoor.addPart(20, 20); // Tip
	sDoor.addPart(20, 19); // Base
	r.addSpring(sDoor);

	// A red locked door at (20, 23)
	Door dTrap(20, 23, 8, Color::RED);
	dTrap.addRequiredKey(99); // Impossible key ID
	r.addDoor(dTrap);

	Bomb b1(40, 18, 1);
	Bomb b2(42, 22, 2);
	Bomb b3(42, 20, 2);
	r.addBomb(b1);
	r.addBomb(b2);
	r.addBomb(b3);
}