#include "Switch.h"

void Switch::toggleState()
{
	state = !state;
	if (state) {
		setColor(Color::GREEN);
		pos.set(pos.getx(), pos.gety(), SWITCH_ON);
		setColor(Color::WHITE);
	}
	else {
		setColor(Color::RED);
		pos.set(pos.getx(), pos.gety(), SWITCH_OFF);
		setColor(Color::WHITE);
	}
}

void Switch::draw() {
	if (seen) {
		if (state) {
			setColor(Color::GREEN);
			pos.draw();
			setColor(Color::WHITE);
		}
		else {
			setColor(Color::RED);
			pos.draw();
			setColor(Color::WHITE);
		}
	}
	else {
		pos.draw(UNKNOWN_TILE); // Draw as unknown if not seen
		setColor(Color::WHITE);
	}
}
