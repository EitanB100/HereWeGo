#include "Spring.h"

bool Spring::isSpringPart(const Point& p) const
{
	for (const auto& part : parts) {
		if (part.getx() == p.x && part.gety() == p.y) return true;
	}
	return false;
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
