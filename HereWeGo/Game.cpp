#include "Game.h"

const char p1Keys[NUM_KEYS] = { 'W','S','A','D','E','Q' };
const char p2Keys[NUM_KEYS] = { 'I','K','J','L','U','O' };


void Game::initLevel1(Room& r)
{
	// ============================
	// 1. PLAYERS (Top Left Start)
	// ============================
	// Based on the 'P1' and 'P2' marks in your sketch
	players[0].setPos(2, 2);
	players[1].setPos(2, 6);

	// ============================
	// 2. WALLS (The Map Layout)
	// ============================

	// A. Outer Borders
	for (int x = 0; x <= 60; x++) { r.addWall(Point{ x, 0 }); r.addWall(Point{ x, 20 }); } // Top & Bottom
	for (int y = 0; y <= 20; y++) { r.addWall(Point{ 0, y }); r.addWall(Point{ 60, y }); } // Left & Right

	// B. The Horizontal Divider (Separating Top/Bottom)
	// In sketch: Goes from left wall to the center
	for (int x = 1; x <= 25; x++) r.addWall(Point{ x, 8 });

	// C. The Vertical Divider (Separating Top-Left from Top-Right)
	// In sketch: Goes from top wall down to the horizontal line
	for (int y = 1; y <= 8; y++) r.addWall(Point{ 25, y });

	// D. The "Switch Cage" Wall (Bottom Right)
	// In sketch: Encloses the switches, separates them from the left side
	for (int y = 8; y < 20; y++) r.addWall(Point{ 45, y });


	// ============================
	// 3. OBSTACLES (The '*' marks)
	// ============================
	// In sketch: Blocking the path in the top corridor
	Obstacle column;
	column.addPart(Placement(15, 2));
	column.addPart(Placement(15, 3));
	column.addPart(Placement(15, 4));
	r.addObstacle(column);

	// ============================
	// 4. KEYS (The 'K' marks)
	// ============================

	// Purple Key (Top Middle - needed for Purple Door)
	r.addKey(Key(20, 2, 10, Color::MAGENTA));

	// Green Key (Top Right - needed for Exit)
	r.addKey(Key(35, 2, 11, Color::GREEN));

	// Orange/Red Key (Bottom Left - needed for Cage Door)
	r.addKey(Key(5, 12, 12, Color::RED));


	// ============================
	// 5. SWITCHES (The '/' marks)
	// ============================
	// In sketch: Located in the bottom-right "Cage" area
	Switch s1(50, 10, 1);
	Switch s2(55, 12, 2);
	Switch s3(50, 14, 3);

	r.addSwitch(s1);
	r.addSwitch(s2);
	r.addSwitch(s3);

	// *IMPORTANT*: Get pointers to the switches inside the Room 
	// so we can link them to the door logic.
	Switch* pS1 = r.isSwitchThere(s1.getPos());
	Switch* pS2 = r.isSwitchThere(s2.getPos());
	Switch* pS3 = r.isSwitchThere(s3.getPos());


	// ============================
	// 6. DOORS (The 'D' marks)
	// ============================

	// Purple Door (Top, allows access to Green Key)
	// In sketch: Located on the vertical divider
	Door dPurple(25, 4, 1, Color::MAGENTA);
	dPurple.addRequiredKey(10); // Requires Purple Key
	r.addDoor(dPurple);

	// Red Door (Bottom, allows access to Switches)
	// In sketch: Located on the cage wall
	Door dRed(45, 12, 2, Color::RED);
	dRed.addRequiredKey(12); // Requires Orange/Red Key
	r.addDoor(dRed);

	// EXIT DOOR (Blue/Cyan) - Leads to Room 2
	// In sketch: Bottom right corner, arrows pointing to "Room 2"
	Door dExit(58, 18, 9, Color::CYAN);

	// Logic: Requires Green Key AND correct Switch combination
	dExit.addRequiredKey(11);
	dExit.addRequiredSwitch(pS1, true);  // Switch 1 must be ON
	dExit.addRequiredSwitch(pS2, false); // Switch 2 must be OFF
	dExit.addRequiredSwitch(pS3, true);  // Switch 3 must be ON

	r.addDoor(dExit);
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

	// --- SETUP SWITCH (Restored from Conflict) ---
	Switch SW1(50, 5, 20);
	room.addSwitch(SW1);

	Point swPos = SW1.getPos();
	Switch* activeSwitch = room.isSwitchThere(swPos);

	// --- SETUP DOOR 1 (Green) ---
	Door d1(12, 1, 1, Color::GREEN);
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
	room.addKey(Key(20, 5, 10, Color::GREEN));
	room.addKey(Key(20, 15, 11, Color::RED));
	room.addKey(Key(20, 10, 12, Color::RED));
	room.addKey(Key(40, 5, 13, Color::RED));

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

	// --- SETUP TORCHES ---
	room.addTorch(Torch(50, 20));

	// --- DRAWING ---
	room.drawRoom(screen);
	screen.draw();
	room.drawTopLayer();

	for (auto& player : players)
	{
		player.draw();
		player.setDirection(0, 0);
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
			players[i].inputManager(key,room);
		}
		
		for (int i = 0; i < 2; i++) {
			setColor(Color::WHITE);
			players[i].move(room, &players[1 - i]);
		}
		printHUD();
		Sleep(75); // Fixed the garbage characters here
	}

}
