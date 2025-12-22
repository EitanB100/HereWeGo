#include "Spring.h"

void Spring::draw() {
	
	for (int i = 0; i < parts.size(); i++) {
		if (i < compressedCount) {
			parts[i].draw(' ');
		}
		else {
			parts[i].draw(SPRING_TILE);
		}
	}
}
