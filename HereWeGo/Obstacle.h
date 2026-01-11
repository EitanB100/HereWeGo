#pragma once
#include <vector>
#include "Utils.h"
#include "Placement.h"
#include "Tile_Chars.h"

class Obstacle {
	std::vector<Placement> parts;
	bool hasMoved = false;
	Color color = Color::WHITE;

public:
	Obstacle() {}

	void addPart(const Placement& part) { // Add a new part to the obstacle
		parts.push_back(part);
	}
	
	size_t getSize() const { // Return the number of parts in the obstacle
		return parts.size();
	}
	
	void move(int dirx, int diry);
	bool getHasMoved() { return hasMoved; }
	void resetMove() { hasMoved = false; }
	void markAsMoved() { hasMoved = true; }
	void getParts(std::vector<Point>& outParts) const {
		for (const auto& part : parts) {
			outParts.push_back(part.getPosition());
		}
	}
	void setColor(Color c) { color = c; }
	Color getColor() const { return color; }

	void obstacleRoomTravel(int x, int y);
	

	bool isAt(const Point& p) const;
	bool overLap(Point p) { return isAt(p); }

	void draw();


	std::vector<Point> getFutureParts(int dirx, int diry) const;
};