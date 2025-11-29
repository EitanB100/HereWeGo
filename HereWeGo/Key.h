#pragma once
#include "Placement.h"
#include "utils.h"
// Key class representing a key in the game

class Key {
	
	int  x, y;
	int keyID;

public:
	Key(int _x, int _y, int _id) : x(_x), y(_y), keyID(_id) {}

	bool isAt (int checkX, int checkY) const {
		return (x == checkX && y == checkY);
	}

	int getKeyID() const {
		return keyID;
	}
 };