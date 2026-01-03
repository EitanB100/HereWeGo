#pragma once
#include "Placement.h"
#include "utils.h"
#include "Tile_Chars.h"


// Bomb class representing a bomb in the game
class Bomb {
	Placement pos;
	int bombID;
	Color color = Color::RED;
	int timer = 5; // default timer value
	bool activated = false;
	int blastRadius = 3; // default blast radius
	bool seen = false;
	char explosionChar = EXPLOSION_TILE;

public:
	Bomb(int x, int y, int id) : pos(x, y, BOMB_TILE), bombID(id){}
	Bomb(int x, int y, int id, int t) : pos(x, y, BOMB_TILE), bombID(id), timer(t) {}

	int getBombID() const {
		return bombID;
	}

	Color getColor() const {
		return color;
	}

	bool getIsSeen() const {
		return seen;
	}
	void setSeen() {
		seen = true;
	}
	
	bool isActivated() { return activated; }

	Point getPos() const {
		return pos.getPosition();
	}
	char getExplosionChar() const {
		return explosionChar;
	}
	int getBlastRadius() const {
		return blastRadius;
	}
	void activate() {
		activated = true;
	}
	void gonnaExplode() { if (activated && timer > 0) timer--; }
	int getTimer() const { return timer; }

	void draw();
};
