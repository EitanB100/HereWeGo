#include "Spring.h"

bool Spring::isSpringPart(const Point& p) const
{
	for (const auto& part : parts) {
		if (part.getx() == p.x && part.gety() == p.y) return true;
	}
	return false;
}

void Spring::setDirection(Point dir)
{
	direction = dir;
	sortParts();
}

void Spring::sortParts() //ensuring parts[0] is the tip of the spring
{
	if (parts.size() < 2) return;

	bool swapped = true;
	bool needSwap = false;
	while (swapped) {
		swapped = false;
		for (int i = 0; i < parts.size() - 1; i++) { 
			if (direction == Directions::RIGHT){
				if (parts[i].getx() < parts[i + 1].getx()) needSwap = true;
			}
			else if (direction == Directions:: LEFT) {
				if (parts[i].getx() > parts[i + 1].getx()) needSwap = true;
			}
			else if (direction == Directions::DOWN) {
				if (parts[i].gety() < parts[i + 1].gety()) needSwap = true;
			}
			else if (direction == Directions::UP) {
				if (parts[i].gety() > parts[i + 1].gety()) needSwap = true;
			}
			if (needSwap)
				std::swap(parts[i], parts[i + 1]);
		}
	}
}

void Spring::draw() {
	setColor(color);
	for (int i = 0; i < parts.size(); i++) {
		if (i < compressedCount) {
			parts[i].draw(' ');
		}
		else {
			parts[i].draw(SPRING_TILE);
		}
	}
	setColor(Color::WHITE);
}
