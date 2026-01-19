#include "Bomb.h"
#include "Game.h"

void Bomb::draw() {
	if (Game::s_silentMode) return;
	if (seen) {
		setColor(color);
		pos.draw();
	}
	else
		pos.draw(UNKNOWN_TILE);
	setColor(Color::WHITE);

}
