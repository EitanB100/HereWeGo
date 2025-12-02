#include "Room.h"
#include "Key.h"
#include "Door.h"
#include "Switch.h"  
#include "Obstacle.h"

void Room::drawTopLayer()
{
	for (Door& door : doors) door.draw();
	for (Key& key : keys) key.draw();
	for (Switch& switchOnOff : switches) switchOnOff.draw(); // <--- Added
}

void Room::drawRoom(Screen& screen) // Draw the room on the screen
{
	for (int y = 0; y < MAX_Y; y++)
	{
		for (int x = 0; x < MAX_X; x++)
		{
			screen.setTile(x, y, map[y][x]);
		}
	}
}

Door* Room::isDoorThere(Point& p)
{
	for (Door& door : doors)
	{
		Placement doorPos = door.getPos();
		
		if (doorPos.getx() == p.x && doorPos.gety() == p.y)
		{
			return &door;
		}
	}
	return nullptr;
}

bool Room::checkDoor(Point p, heldItem& item)
{
	Door* door = isDoorThere(p);
	if (door == nullptr) return false;

	// If door is already open, we can pass (optional check)
	if (door->getIsOpen()) return true;

	if (item.type == KEY) 
	{
		if (door->tryUnlock(item.id))
		{
			item = { NONE, 0, Color::WHITE}; // Consume key
			door->draw();
		}
	}

	return (door->getIsOpen());
}


void Room::checkSwitch(Point p) {
	Switch* switchOnOff = isSwitchThere(p);
	if (switchOnOff != nullptr) {
		for (Door& door : doors) {
			door.UpdatedFromSwitch();
		}
	}
}

void Room::addDoor(Door door) {
	Placement doorPos = door.getPos();
	map[doorPos.gety()][doorPos.getx()] = doorPos.getTileChar();
	doors.push_back(door);
}

Key* Room::isKeyThere(Point& p)
{
	for (Key& key : keys)
	{
		Point keyPos = key.getPos();
		if (keyPos.x == p.x && keyPos.y == p.y)
		{
			return &key;
		}
	}
	return nullptr;
}

void Room::addKey(Key key) {
	Point keyPos = key.getPos();
	map[keyPos.y][keyPos.x] = KEY_TILE;
	keys.push_back(key);
}


void Room::addSwitch(const Switch& s) {
	Point SwitchPos = s.getPos();
	if (SwitchPos.x >= 0 && SwitchPos.x < MAX_X && SwitchPos.y >= 0 && SwitchPos.y < MAX_Y) {
		map[SwitchPos.y][SwitchPos.x] = SWITCH_OFF; // Ensure SWITCH_OFF is defined in Tile_Chars.h
		switches.push_back(s);
	}
}

void Room::addObstacle(Obstacle obs)
{
	obstacles.push_back(obs);

	std::vector<Point> currParts = obs.getFutureParts(0, 0);
	for (const auto& part : currParts)
	{

		map[part.y][part.x] = OBSTACLE_TILE;
	}
}

void Room::addWall(Point p)
{
	map[p.y][p.x] = WALL_TILE;
}

// --- NEW SWITCH GETTER (Restored from Conflict) ---
Switch* Room::isSwitchThere(Point& p){
	for (Switch& switchOnOff : switches){
		Point SwitchPoint = switchOnOff.getPos();

		if (SwitchPoint.x == p.x && SwitchPoint.y == p.y){
			return &switchOnOff;
		}
	}
	return nullptr;
}

Obstacle* Room::getObstacleAt(Point p)
{
	for (auto& ob : obstacles)
	{
		if (ob.isAt(p))
			return &ob;
	}
	return nullptr;
}


bool Room::moveObstacle(Point p, int dirx, int diry, int force)
{
	Obstacle* obs = nullptr;
	for (auto& o : obstacles)
	{
		if (o.isAt(p)) {
			obs = &o;
			break;
		}
	}

	if (!obs) return false;
	
	if (obs->getHasMoved()) return true;

	if (force < obs->getSize()) return false;

	std::vector<Point> currentParts = obs->getFutureParts(0, 0);

	for (const auto& part : currentParts) {
		map[part.y][part.x] = ' ';
		gotoxy(part.x, part.y);
		std::cout << ' ';
	}

	std::vector<Point> futureParts = obs->getFutureParts(dirx, diry); // complete
	bool canMove = true;

	for (auto& futurePart : futureParts)
	{
		if (futurePart.x < 0 || futurePart.x > MAX_X - 1 || futurePart.y < 0 || futurePart.y > MAX_Y - 1)
		{
			canMove = false;
			break;
		}

		if (map[futurePart.y][futurePart.x] != ' ')
		{
			canMove = false;
			break;
		}
	}

	if (canMove) {
		obs->move(dirx, diry);

		for (const auto& part : futureParts)
		{
			map[part.y][part.x] = OBSTACLE_TILE;	
		}
		obs->draw();
		return true;
	}
	else {
		for (const auto& part : currentParts)
		{
			map[part.y][part.x] = OBSTACLE_TILE;
		}
		obs->draw();
		return false;
	}

}

char Room::getObjectAt(Point& p)
{
	Door* door = isDoorThere(p);
	if (door != nullptr) {
		return door->getIsOpen() ? ' ' : door->getPos().getTileChar();
	}

	Key* key = isKeyThere(p);
	if (key != nullptr) {
		return key->getIsActive() ? KEY_TILE : ' ';
	}
	
	// Note: You might want to add a check for Switch here too if you want to avoid walking on it
	// But usually, switches are walkable 'floor' tiles.

	return map[p.y][p.x];
}

/*bool Room::moveObstacle(Obstacle* obstacle, int dirx, int diry, int playerForce)
{
	if (playerForce < obstacle->getSize()) return false; // Not enough force
	
	// Add your actual moving logic here later.
	// For now, return false is fine to compile.
	return false;
}*/