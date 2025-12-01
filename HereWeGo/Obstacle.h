#pragma once
#include <vector>
#include "Utils.h"
#include "Placement.h"

class Obstacle {
	std::vector<Placement> parts;
	char symbol = '*';
public:
	Obstacle() {}

	void addPart(const Placement& part) { // Add a new part to the obstacle
		parts.push_back(part);
	}
	
	size_t getSize() const { // Return the number of parts in the obstacle
		return parts.size();
	}
	
	void move(int dirx, int diry);

	bool isAt(const Point& p) const;

	void draw();


	std::vector<Point> getFutureParts(int dirx, int diry) const;
};