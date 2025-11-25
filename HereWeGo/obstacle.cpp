#include "Obstacle.h"

void Obstacle::move(int dirx, int diry)
{

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