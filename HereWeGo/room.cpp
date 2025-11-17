#include "Room.h"



void Room::MakeWalls() {
	int x = pos.getx();
	int y = pos.gety();
	for (int i = 0; i <= getWidth(); i++) {
		Placement walls(x + i, y, 'W');
		walls.draw();
	}

	for (int i = 1; i < getHeight(); i++) {
		Placement walls(x, y + i, 'W');
		walls.draw();
	}

	for (int i = getWidth() - 1; i > 0; i--) {
		Placement walls(x + i, y + getHeight(), 'W');
		walls.draw();
	}

	for (int i = getHeight() - 1; i > 0; i--) {
		Placement walls(x + getWidth(), y + i, 'W');
		walls.draw();
	}

};