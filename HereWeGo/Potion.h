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
	Potion(int x, int y, bool _seen) : pos(x,y,POTION_TILE),seen(_seen) {}


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
