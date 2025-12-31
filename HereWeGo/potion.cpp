#include "Potion.h"

void Potion::draw() {
	if (seen) {
		setColor(color);
		pos.draw();
	}
	else pos.draw(UNKNOWN_TILE);
	setColor(Color::WHITE);
}
