#pragma once
#include "Placement.h"
#include "utils.h"
<<<<<<< Updated upstream
// Key class representing a key in the game

class Key {
	Placement pos;
	int keyID;
	bool isActive = true; // tracking if key has been picked up

public:

	Key() : pos(0, 0, 'K'), keyID(0) {}  // Default constructor for array initialization

	Key(int _x, int _y, int _keyID) : pos(_x,_y,'K'), keyID(_keyID) {}
=======
#include "Tile_Chars.h"
// Key class representing a key in the game

class Key {
	
	Placement pos;
	
	int keyID;
	Color color = Color::WHITE;
	bool isActive = true;

public:
	Key(int x, int y, int id, Color c) : pos(x, y, KEY_TILE), keyID(id), color(c) {}
>>>>>>> Stashed changes

	int getKeyID() const {
		return keyID;
	}

<<<<<<< Updated upstream
	Placement getPosition() const {
		return pos;
	}

	void draw() {
		pos.draw();
=======
	Color getColor() const {
		return color;
>>>>>>> Stashed changes
	}

	bool getIsActive() const {
		return isActive;
	}

<<<<<<< Updated upstream
	void takeKey() { // called when key is picked up
		isActive = false;
		pos.draw(' '); // Erase key from screen
	}

	bool isAtPosition(const Point& p) const {
		return (isActive && pos.getx() == p.x && pos.gety() == p.y);
	}
=======
	Point getPos() const {
		return pos.getPosition();
	}
	
	void takeKey() { isActive = false; }
	
	void draw()
	{
		if (isActive) {
			setColor(color);
			pos.draw();
			setColor(Color::WHITE); // Reset to default color
		}
	}

>>>>>>> Stashed changes
 };