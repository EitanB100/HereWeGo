#include "Key.h"

void Key::draw() {
	if (isActive) {

		if (seen) {
			setColor(color);
			pos.draw();
		}
		else
			pos.draw(UNKNOWN_TILE); // Draw as random item (R) and player cant interfene with it as not seen
		setColor(Color::WHITE); // Reset to default color
	}
}
