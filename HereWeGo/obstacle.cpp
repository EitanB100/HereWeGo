#include "Obstacle.h"

void Obstacle::move(int dirx, int diry)
{
	for (auto& part : parts)
	{
		part.move(dirx, diry, OBSTACLE_TILE);
	}
}

void Obstacle::obstacleRoomTravel(int x, int y)
{
	if (parts.empty()) return;

	int diffX = x - parts[0].getx();
	int diffY = y - parts[0].gety();

	for (auto& part : parts) {
		part.move(diffX, diffY, OBSTACLE_TILE);
	}
}

bool Obstacle::isAt(const Point& p) const { // Check if any part of the obstacle is at the given point
	for (const auto& part : parts) {
		if (part.getPosition() == p) {
			return true; 
		}
	}
	return false; 
}

void Obstacle::draw() { // Draw the entire obstacle on screen
	for (auto& part : parts) {
		part.draw(OBSTACLE_TILE); // Draw each part of the obstacle
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
