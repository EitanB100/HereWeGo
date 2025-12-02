#pragma once
#include "Placement.h"
#include "utils.h"
#include "Tile_Chars.h"
// Key class representing a key in the game

class Switch {

	Placement pos;
	int SwtichID;
	Color color = Color::RED; //defult color of Off
	bool state = false;

public:
	Switch(int x, int y, int id) : pos(x, y, SWITCH_OFF), SwtichID(id) {}

	int getSwitchID() const {
		return SwtichID;
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

	void toggleState() {
		state = !state;
		if (state) {
			setColor(Color::GREEN);
			pos.set(pos.getx(), pos.gety(), SWITCH_ON);
		}
		else {
			setColor(Color::RED);
			pos.set(pos.getx(), pos.gety(), SWITCH_OFF);
		}
	}

	void draw()
	{
		if (state) {
			setColor(Color::GREEN);
			pos.draw();
			setColor(Color::WHITE); // Ask Ethan about resetting color
		}
		else {
			setColor(Color::RED);
			pos.draw();
			setColor(Color::WHITE);
		}
	}

};
