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
