#include "Room.h"



void Room::drawRoom(Screen& screen)
{
	int rows = getHeight();
	int cols = getWidth();
	int startX = pos.getx();
	int startY = pos.gety();

	for (int i = 0; i < cols; i++) { // Top edge
		screen.setTile(startX + i, startY, symbol);
	}

	for (int i = 1; i < rows - 1; i++) { // Sides
		screen.setTile(startX, startY + i, symbol);
		screen.setTile(startX + cols - 1, startY + i, symbol);
	}
	for (int i = 0; i < cols; i++) { // Bottom edge
		screen.setTile(startX + i, startY + rows - 1, symbol);
	}
}