#pragma once
#include "Placement.h"
#include "utils.h"
#include "Tile_Chars.h" 

// Torch class representing a torch in the game
class Torch {

	Placement pos;
	int LineOfSight = 5;
	Color color = Color::MAGENTA;

public:
	Torch(int x, int y) : pos(x, y, TORCH_TILE) {}

	Torch(int x, int y, int sight) : pos(x, y, TORCH_TILE), LineOfSight(sight) {}

	int getLineOfSight() const {
		return LineOfSight;
	}

	void setLineOfSight(int sight) {
		LineOfSight = sight;
	}

	Color getColor() const {
		return color;
	}
	Point getPos() const {
		return pos.getPosition();
	}

	void draw() {
		setColor(Color::MAGENTA);
		pos.draw();
		setColor(Color::WHITE);
	}


};