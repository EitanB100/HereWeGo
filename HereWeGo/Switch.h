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
	bool seen = false; // if player has seen the switch yet  (for future use)

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

	bool getIsSeen() const {
		return seen;
	}
	void setSeen() {
		seen = true;
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

	void draw() {
		if (seen) {
			if (state) {
				setColor(Color::GREEN);
				pos.draw();
				setColor(Color::WHITE);
			}
			else {
				setColor(Color::RED);
				pos.draw();
				setColor(Color::WHITE);
			}
		}
		else {
			pos.draw(UNKNOWN_TILE); // Draw as unknown if not seen
			setColor(Color::WHITE);
		}
	}

};
