#pragma once
#include "Placement.h"
#include "utils.h"
#include "Tile_Chars.h"
// Key class representing a key in the game

class Switch {

	Placement pos;
	int SwitchID;
	Color color = Color::RED; //defult color for Off
	bool state = false;
	bool seen = false; 

public:
	Switch(int x, int y, int id) : pos(x, y, SWITCH_OFF), SwitchID(id) {}

	int getSwitchID() const {
		return SwitchID;
	}

	Color getColor() const {
		return color;
	}

	bool getState() const {
		return state;
	}

	Point getPos() const {
		return pos.getPosition();
	}

	bool getIsSeen() const {
		return seen;
	}
	void setSeen() {
		seen = true;
	}
	char stateChar() const {
		return state ? SWITCH_ON : SWITCH_OFF;
	}

	void toggleState();
	void draw();

};
