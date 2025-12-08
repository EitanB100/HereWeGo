#include "Obstacle.h"

void Obstacle::move(int dirx, int diry)
{
	for (auto& part : parts)
	{
		part.move(dirx, diry, symbol);
	}
}

void Obstacle::obstacleRoomTravel(int x, int y)
{
	if (parts.empty()) return;

	int diffX = x - parts[0].getx();
	int diffY = y - parts[0].gety();

	for (auto& part : parts) {
		part.move(diffX, diffY, symbol);
	}
}

bool Obstacle::isAt(const Point& p) const { // Check if any part of the obstacle is at the given point
	for (const auto& part : parts) {
		if (part.getx() == p.x && part.gety() == p.y) {
			return true; 
		}
	}
	return false; 
}

void Obstacle::draw() { // Draw the entire obstacle on screen
	for (auto& part : parts) {
		part.draw(symbol); // Draw each part of the obstacle
	}
}

std::vector<Point> Obstacle::getFutureParts(int dirx, int diry) const
{
	std::vector<Point> newPositions;
	for (const auto& part : parts)
	{
		newPositions.push_back({ part.getx() + dirx, part.gety() + diry });
	}
	return newPositions;
}
