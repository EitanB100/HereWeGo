#pragma once
#include "Placement.h"
#include "utils.h"
// Key class representing a key in the game

class Key {
	
	Placement pos;
	
	int keyID;
	Color color = Color::WHITE;
	bool isActive = true;

public:
	Key(int x, int y, int id, Color c) : pos(x, y, KEY_TILE), keyID(id), color(c) {}

	int getKeyID() const {
		return keyID;
	}

	Color getColor() const {
		return color;
	}

	bool getIsActive() const {
		return isActive;
	}

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

 };