#include "Bomb.h"

void Bomb::draw() {
	if (seen) {
		setColor(color);
		pos.draw();
	}
	else
		pos.draw(UNKNOWN_TILE);
	setColor(Color::WHITE);

}
