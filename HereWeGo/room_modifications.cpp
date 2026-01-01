#include "Room.h"

void Room::addDoor(const Door& door) {
	Placement doorPos = door.getPos();
	map[doorPos.gety()][doorPos.getx()] = doorPos.getTileChar();
	doors.push_back(door);
}


void Room::addKey(const Key& key) {
	Point keyPos = key.getPos();
	if (key.getIsSeen())
		map[keyPos.y][keyPos.x] = KEY_TILE;
	else
		map[keyPos.y][keyPos.x] = UNKNOWN_TILE; // Draw as unknown if not seen
	keys.push_back(key);
}

void Room::addTorch(const Torch& torch) {
	Point TorchPos = torch.getPos();
	map[TorchPos.y][TorchPos.x] = TORCH_TILE;
	torches.push_back(torch);
}

void Room::addSpring(const Spring& spring)
{
	springs.push_back(spring);
	for (const auto& part : spring.getParts()) {
		map[part.gety()][part.getx()] = SPRING_TILE;
	}
}

void Room::addSwitch(std::unique_ptr<Switch> s) {
	Point switchPos = s->getPos();
	if (switchPos.x >= 0 && switchPos.x < MAX_X && switchPos.y >= 0 && switchPos.y < MAX_Y) {
		map[switchPos.y][switchPos.x] = SWITCH_OFF; // Ensure SWITCH_OFF is defined in Tile_Chars.h
		switches.push_back(std::move(s));
	}
}

void Room::addBomb(const Bomb& bomb) {
	Point bombPos = bomb.getPos();
	if (bomb.getIsSeen())
		map[bombPos.y][bombPos.x] = BOMB_TILE;
	else
		map[bombPos.y][bombPos.x] = UNKNOWN_TILE; // Draw as unknown if not seen
	bombs.push_back(bomb);
}

void Room::addObstacle(const Obstacle& obs)
{
	obstacles.push_back(obs);

	std::vector<Point> currParts = obs.getFutureParts(0, 0);
	for (const auto& part : currParts)
	{

		map[part.y][part.x] = OBSTACLE_TILE;
	}
}

void Room::addPotion(const Potion& potion)
{
	Point potionPos = potion.getPos();
	if (potion.getIsSeen())
		map[potionPos.y][potionPos.x] = POTION_TILE;
	else
		map[potionPos.y][potionPos.x] = UNKNOWN_TILE;

	potions.push_back(potion);
}

void Room::addRiddle(const Riddle& riddle)
{

}

void Room::addWall(const Point& p)
{
	map[p.y][p.x] = WALL_TILE;
}

void Room::removeKey(const Point& p)
{
	for (auto key = keys.begin(); key != keys.end(); key++) {
		if (key->getPos() == p) {
			keys.erase(key);
			map[p.y][p.x] = ' ';
			return;
		}
	}
}

void Room::removeTorch(const Point& p)
{
	for (auto torch = torches.begin(); torch != torches.end(); torch++) {
		if (torch->getPos() == p) {
			torches.erase(torch);
			map[p.y][p.x] = ' ';
			return;
		}
	}
}


void Room::removeObstacle(const Point& p)
{
	for (auto obstacle = obstacles.begin(); obstacle != obstacles.end(); obstacle++) {
		if (obstacle->isAt(p)) {
			obstacles.erase(obstacle);
			return;
		}
	}
}

void Room::removeSpring(const Point& p)
{
	for (auto spring = springs.begin(); spring != springs.end(); spring++) {
		if (spring->isSpringPart(p)) {
			springs.erase(spring);
			return;
		}
	}
} // ask if remove all or just one part of spring


void Room::removeSwitch(const Point& p)
{
	for (auto sw = switches.begin(); sw != switches.end(); sw++) {
		if ((*sw)->getPos() == p) {
			switches.erase(sw);
			map[p.y][p.x] = ' ';
			return;
		}
	}
}

void Room::removeBomb(const Point& p) {
	for (auto bomb = bombs.begin(); bomb != bombs.end(); bomb++) {
		if (bomb->getPos() == p) {
			bombs.erase(bomb);
			map[p.y][p.x] = ' ';
			return;
		}
	}
}

void Room::removePotion(const Point& p)
{
	for (auto potion = potions.begin(); potion != potions.end(); potion++) {
		if (potion->getPos() == p) {
			potions.erase(potion);
			map[p.y][p.x] = ' ';
			return;
		}
	}
}
