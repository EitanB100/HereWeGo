#include "Bomb.h"
#include "Game.h"

void Bomb::draw() {
	if (isGlobalSilent()) return;
	if (seen) {
		setColor(color);
		pos.draw();
	}
	else
		pos.draw(UNKNOWN_TILE);
	setColor(Color::WHITE);

}
