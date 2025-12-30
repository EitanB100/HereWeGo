#include "Room.h"


// Door finders
Door* Room::isDoorThere(const Point& p) {
	return findBy(doors, [&](Door& door) {return door.getPos().getPosition() == p; });
}

const Door* Room::isDoorThere(const Point& p) const {
	return findBy(doors, [&](const Door& door) {return door.getPos().getPosition() == p; });
}

// Key finders
Key* Room::isKeyThere(const Point& p) {
	return findBy(keys, [&](Key& key) {return key.getPos() == p; });
}

const Key* Room::isKeyThere(const Point& p) const {
	return findBy(keys, [&](const Key& key) {return key.getPos() == p; });
}

// Switch finders
Switch* Room::isSwitchThere(const Point& p) {
	auto sw = findBy(switches, [&](const std::unique_ptr<Switch>& s) {return s && s->getPos() == p; });

	return sw ? sw->get() : nullptr;
}

const Switch* Room::isSwitchThere(const Point& p) const {
	auto sw = findBy(switches, [&](const std::unique_ptr<Switch>& s) {return s && s->getPos() == p; });

	return sw ? sw->get() : nullptr;
}

//Torch finders
Torch* Room::isTorchThere(const Point& p) {
	return findBy(torches, [&](Torch& torch) {return torch.getPos() == p; });
}

const Torch* Room::isTorchThere(const Point& p) const {
	return findBy(torches, [&](const Torch& torch) {return torch.getPos() == p; });
}

//Obstacle finders
Obstacle* Room::isObstacleThere(const Point& p) {
	return findBy(obstacles, [&](Obstacle& obstacle) {return obstacle.isAt(p); });
}

const Obstacle* Room::isObstacleThere(const Point& p) const {
	return findBy(obstacles, [&](const Obstacle& obstacle) {return obstacle.isAt(p); });
}

// Spring finders
Spring* Room::isSpringThere(const Point& p) {
	return findBy(springs, [&](Spring& spring) {return spring.isSpringPart(p); });
}

const Spring* Room::isSpringThere(const Point& p) const {
	return findBy(springs, [&](const Spring& spring) {return spring.isSpringPart(p); });
}

// Bomb finders
Bomb* Room::isBombThere(const Point& p) {
	return findBy(bombs, [&](Bomb& bomb) {return bomb.getPos() == p; });
}

const Bomb* Room::isBombThere(const Point& p) const {
	return findBy(bombs, [&](const Bomb& bomb) {return bomb.getPos() == p; });
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
