#include "Game.h"

const char p1Keys[NUM_KEYS] = { 'W','X','A','D','S','E' };
const char p2Keys[NUM_KEYS] = { 'I','M','J','L','K','O' };




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
	init();
}

void Game::init()
{
    // Initialize ALL levels
    initLevel1Props(levels[0]);
    initLevel2Props(levels[1]);
    initLevel3Props(levels[2]);

	currentLevelID = 0;// Start at Level 1
	setGame(currentLevelID);
}

void Game::setGame(int level) {
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
	screen.draw();
	levels[currentLevelID].drawTopLayer();

	for (auto& player : players) {
		player.setDirection(0, 0);
		player.setFinished(false);
		player.draw();
	}
}


void Game::run()
{
	while (true) {
		Room& currRoom = levels[currentLevelID]; 
		char key = 0;
		
		
		if (_kbhit()) {
			key = _getch();
			if (key == ESC) {
				key = _getch();
				if (key == 'h' || key == 'H') {
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
					std::cout << "Player" << p.getSymbol() << " Waiting...";
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
		
		//HUD renderer
		printHUD();
		Sleep(75);
	}

}

void Game::checkLevelTransition(int& currentLevel, Point p1, Point p2)
{
	Point exit = exitPoints[currentLevel];
	if (exit.x == -1) return; // No standard exit

	if (p1 == exit && p2 == exit)
	{
		// Level 1 -> Level 2 Special Case (Obstacle carry over)
		if (currentLevel == 0) {
			Obstacle* obs = levels[0].isObstacleThere({ 58, 18 });
			if (obs) {
				Obstacle newObs = *obs;
				newObs.obstacleRoomTravel(3, 3);
				levels[1].addObstacle(newObs);
				levels[0].removeObstacle({ 58, 18 });
			}
		}

		// Advance level
		if (currentLevel < ROOM_AMOUNT - 1) {
			currentLevel++;
			setGame(currentLevel);
		}
		else {
			// Looping back or end game state could go here
			// For now, just reset the current level to simulate endless play
			setGame(currentLevel);
		}
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
	// Simple Key
	r.addKey(Key(40, 3, 1, Color::RED));
	r.addDoor(Door(42, 3, 1, Color::RED));

	// Switch Door
	Switch* sw1 = new Switch(60, 3, 401); // ID 401
	r.addSwitch(sw1);
	Door dSwitch(65, 3, 0, Color::CYAN);
	dSwitch.addRequiredSwitch(r.getSwitchByID(401), true); // Open when ON
	r.addDoor(dSwitch);


	// 2. Spring Chaining (Middle Left)
	// Launch RIGHT -> Hit UP Spring -> Fly UP
	Spring s1({ 1,0 }); // Push Right
	s1.addPart(5, 10);
	s1.addPart(6, 10); // Tip
	r.addSpring(s1);

	Spring s2({ 0,-1 }); // Push Up
	s2.addPart(15, 8); // Tip
	s2.addPart(15, 9);
	r.addSpring(s2);

	// Wall to catch the player
	r.addWall({ 15, 4 });

	// 3. Obstacles (Bottom)
	Obstacle box;
	box.addPart(Placement(10, 18));
	box.addPart(Placement(11, 18)); // 2-wide box
	r.addObstacle(box);

	// 4. Torch
	r.addTorch(Torch(35, 18, 5));

	// 5. Switches controlling logic
	Switch* lightSwitch = new Switch(70, 20, 402);
	r.addSwitch(lightSwitch);
}