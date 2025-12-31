#include "Torch.h"

void Torch::draw() {
	setColor(color);
	pos.draw();
	setColor(Color::WHITE);
}