#pragma once
#include "Placement.h"
#include "utils.h"
#include "Tile_Chars.h" 

// Key class representing a key in the game
class Key {

	Placement pos;

	int keyID;
	Color color = Color::WHITE;
	bool seen = false;

public:
	// Use the version that supports Color and the global KEY_TILE constant
	Key(int x, int y, int id, Color c) : pos(x, y, KEY_TILE), keyID(id), color(c) {}

	int getKeyID() const {
		return keyID;
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



	// Note: The new version returns a Point (struct), not the Placement object.
	Point getPos() const {
		return pos.getPosition();
	}


	void draw();

};