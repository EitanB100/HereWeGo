#include "Room.h"


// Door finders
Door* Room::isDoorThere(const Point& p) {
	for (auto& door : doors) {
		if (door.getPos().getPosition() == p) return &door;
	}
	return nullptr;
}

const Door* Room::isDoorThere(const Point& p) const {
	for (const auto& door : doors) {
		if (door.getPos().getPosition() == p) return &door;
	}
	return nullptr;
}

// Key finders
Key* Room::isKeyThere(const Point& p) {
	for (auto& key : keys) {
		if (key.getPos() == p) return &key;
	}
	return nullptr;
}

const Key* Room::isKeyThere(const Point& p) const {
	for (const auto& key : keys) {
		if (key.getPos() == p) return &key;
	}
	return nullptr;
}

// Switch finders
Switch* Room::isSwitchThere(const Point& p) {
	for (auto& sw : switches) {
		if (sw && sw->getPos() == p) return sw.get();
	}
	return nullptr;
}

const Switch* Room::isSwitchThere(const Point& p) const {
	for (const auto& sw : switches) {
		if (sw && sw->getPos() == p) return sw.get();
	}
	return nullptr;
}

//Torch finders
Torch* Room::isTorchThere(const Point& p) {
	for (auto& torch : torches) {
		if (torch.getPos() == p) return &torch;
	}
	return nullptr;
}

const Torch* Room::isTorchThere(const Point& p) const {
	for (const auto& torch : torches) {
		if (torch.getPos() == p) return &torch;
	}
	return nullptr;
}

//Obstacle finders
Obstacle* Room::isObstacleThere(const Point& p) {
	for (auto& ob : obstacles) {
		if (ob.isAt(p)) return &ob;
	}
	return nullptr;
}

const Obstacle* Room::isObstacleThere(const Point& p) const {
	for (const auto& ob : obstacles) {
		if (ob.isAt(p)) return &ob;
	}
	return nullptr;
}

// Spring finders
Spring* Room::isSpringThere(const Point& p) {
	for (auto& spring : springs) {
		if (spring.isSpringPart(p)) return &spring;
	}
	return nullptr;
}

const Spring* Room::isSpringThere(const Point& p) const {
	for (const auto& spring : springs) {
		if (spring.isSpringPart(p)) return &spring;
	}
	return nullptr;
}

// Bomb finders
Bomb* Room::isBombThere(const Point& p) {
	for (auto& bomb : bombs) {
		if (bomb.getPos() == p) return &bomb;
	}
	return nullptr;
}

const Bomb* Room::isBombThere(const Point& p) const {
	for (const auto& bomb : bombs) {
		if (bomb.getPos() == p) return &bomb;
	}
	return nullptr;
}


bool Room::isWallThere(Point p) {
	//world edge boundary check
	if (p.x < 0 || p.x >= MAX_X || p.y < 0 || p.y >= MAX_Y)
		return true;

	if (map[p.y][p.x] == WALL_TILE)
		return true;
	return false;
}

//determines what exists at a coordinate.
//the order of checks is sorted by visual priority
char Room::getObjectAt(const Point& p) const
{
	Color c = Color::WHITE;
	return getObjectAt(p, c);
}

char Room::getObjectAt(const Point& p, Color& color) const
{
	//boundary check
	if (p.x < 0 || p.x > MAX_X || p.y < 0 || p.y > MAX_Y) {
		color = Color::WHITE;
		return ' ';
	}

	//obstacles - movable object - top priority
	auto obstacle = isObstacleThere(p);
	if (obstacle != nullptr) {
		color = Color::WHITE;
		return OBSTACLE_TILE;

	}
	//interactables:

	//keys
	auto key = isKeyThere(p);
	if (key != nullptr) {

		if (key->getIsSeen())
		{
			color = key->getColor();
			return KEY_TILE;
		}
		color = Color::WHITE;
		return UNKNOWN_TILE;
	}

	//torches
	auto torch = isTorchThere(p);
	if (torch != nullptr) {
		color = torch->getColor();
		return TORCH_TILE;
	}

	//static:
	//doors:
	auto door = isDoorThere(p);
	if (door != nullptr) {
		if (door->getIsOpen()) color = door->getColor();

		return door->getIsOpen() ? ' ' : door->getPos().getTileChar();
	}
	//bombs
	auto bomb = isBombThere(p);
	if (bomb != nullptr) {
		if (bomb->getIsSeen()) {
			color = bomb->getColor();
			return BOMB_TILE;
		}
		else {
			color = Color::WHITE;
			return UNKNOWN_TILE;
		}
	}

	//switches:
	auto sw = isSwitchThere(p);
	if (sw != nullptr) { // Check if a switch is present
		if (sw->getIsSeen()) {
			// Switch is seen, return its state
			color = sw->getState() ? Color::GREEN : Color::RED;
			return sw->getState() ? SWITCH_ON : SWITCH_OFF;
		}
		else {
			// Switch is NOT seen, treat it as unknown/blocked for movement
			color = Color::WHITE;
			return UNKNOWN_TILE;
		}
	}



	return map[p.y][p.x];
}
