#include "Potion.h"

void Potion::draw() {
	setColor(color);
	pos.draw();
	setColor(Color::WHITE);
}
