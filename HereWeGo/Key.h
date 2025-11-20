#pragma once
#include "Placement.h"
#include "utils.h"
// Key class representing a key in the game

class Key {
	Placement pos;
	int keyID;
	bool isActive = true; // tracking if key has been picked up

public:

	Key() : pos(0, 0, 'K'), keyID(0) {}  // Default constructor for array initialization

	Key(int _x, int _y, int _keyID) : pos(_x,_y,'K'), keyID(_keyID) {}

	int getKeyID() const {
		return keyID;
	}

	Placement getPosition() const {
		return pos;
	}

	void draw() {
		pos.draw();
	}

	bool getIsActive() const {
		return isActive;
	}

	void takeKey() { // called when key is picked up
		isActive = false;
		pos.draw(' '); // Erase key from screen
	}

	bool isAtPosition(const Point& p) const {
		return (pos.getx() == p.x && pos.gety() == p.y);
	}
 };