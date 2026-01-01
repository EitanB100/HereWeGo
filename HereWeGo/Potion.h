#pragma once
#include "Placement.h"
#include "utils.h"
#include "Tile_Chars.h" 

class Potion {

	Placement pos;
	Color color = Color::BLUE;
	bool seen = false;
	

public:
	Potion(int x, int y) : pos(x, y, POTION_TILE) {}


	Color getColor() const {
		return color;
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
	void draw();


}; 
