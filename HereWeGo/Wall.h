#pragma once
#include "Placement.h"
#include "Utils.h"

// Represents internal walls within a room!
// decided to make the perimiter indestructible, because it represents the screen boundaries
class Wall {
	Placement pos;
	bool isActive = true; // could be false if bombed!

public:
	Wall() {}
	Wall(int _x, int _y) : pos(_x, _y, 'W') {}
	Wall(int _x, int _y, bool active) : pos(_x, _y, 'W'), isActive(active) {}

	bool isAt(const Point& p) const {
		return (isActive && pos.getx() == p.x && pos.gety() == p.y);
	}

	Placement getPosition() const {
		return pos;
	}

	bool isActiveWall() const {
		return isActive;
	}
	void destroy() { // called when bombed
		isActive = false;
		pos.draw(' '); // Erase wall from screen
	}

	void draw() {
		if (isActive) pos.draw();
		
		else pos.draw(' ');
	}
};