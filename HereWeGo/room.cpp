#include "Room.h"
#include "Key.h"
#include "Door.h"
#include "Switch.h"  
#include "Obstacle.h"
#include "Tile_Chars.h"

Room::Room() { 
	for (int y = 1; y < MAX_Y; y++)
	{
		for (int x = 0; x < MAX_X; x++)
		{
			// Initialize Map Data
			// I set the boundaries to WALL_TILE and the inside to Empty Space.
			// PREVIOUS BUG: The map was initialized to all ' ', so the walls
			// existed in the physics check but were invisible on screen.
			if (y == 1 || y == MAX_Y - 1 || x == 0 || x == MAX_X - 1)
				map[y][x] = WALL_TILE;
			else
				map[y][x] = ' ';
		}
	}
}

void Room::drawTopLayer()
{
	for (Door& door : doors) door.draw();
	for (Key& key : keys) key.draw();
	for (Switch& switchOnOff : switches) switchOnOff.draw(); // <--- Added
	for (Torch& torch : torches) torch.draw();
	getTorchesLineOfSight();
	
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
	if (key.getIsSeen())
		map[keyPos.y][keyPos.x] = KEY_TILE;
	else
		map[keyPos.y][keyPos.x] = UNKNOWN_TILE; // Draw as unknown if not seen
	keys.push_back(key);
}

void Room::addTorch(Torch torch) {
	Point TorchPos = torch.getPos();
	map[TorchPos.y][TorchPos.x] = TORCH_TILE;
	torches.push_back(torch);
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


Switch* Room::isSwitchThere(Point& p){
	for (Switch& switchOnOff : switches){
		Point SwitchPoint = switchOnOff.getPos();

		if (SwitchPoint.x == p.x && SwitchPoint.y == p.y){
			return &switchOnOff;
		}
	}
	return nullptr;
}
bool Room::isWallThere(Point p) {
	if (map[p.y][p.x] == WALL_TILE)
		return true;
	return false;
}
bool Room::PointhasLineOfSight(int x1, int y1, int x2, int y2) //using Bresenham's Line Algorithm
{
	int distanceX = abs(x2 - x1);
	int distanceY = abs(y2 - y1);
	int directionX = (x1 < x2) ? 1 : -1; // step in x direction, left or right
	int directionY = (y1 < y2) ? 1 : -1; // step in y direction, up or down
	int balance = distanceX - distanceY; // error value , difference between distances

	while (true)
	{
		if (x1 == x2 && y1 == y2)
			return true;

		Point p{ x1, y1 };

		if (isObstacleThere(p) || isWallThere(p)) //  obstacle or wall in the way
			return false;

		int comperedError = 2 * balance; // go back one tile in x or y direction close to the torch
		if (comperedError > -distanceY) { balance -= distanceY; x1 += directionX; }
		if (comperedError < distanceX) { balance += distanceX; y1 += directionY; }
	}
}

void Room::CompleteLineOfSight(Torch torch) {
	Point torchPoint = torch.getPos();
	int Dist = torch.getLineOfSight();

	for (int y = torchPoint.y - Dist; y <= torchPoint.y + Dist; y++) // running on possable points of LOS
	{
		for (int x = torchPoint.x - Dist; x <= torchPoint.x + Dist; x++)
		{
			if (x < 0 || y < 0 || x >= MAX_X || y >= MAX_Y) // out of bounds
				continue;
			Point p{ x, y };

			int distanceFromTorchX = x - torchPoint.x;
			int distanceFromTorchY = y - torchPoint.y;
			
			if (distanceFromTorchX * distanceFromTorchX + distanceFromTorchY * distanceFromTorchY > Dist * Dist) // out of circle , distance formula
				continue;
			
			if (!(PointhasLineOfSight(torchPoint.x, torchPoint.y, x, y))) // check if torch can see that object
				continue;
			
			if (isKeyThere(p)) // if key there , make it seen
			{
				Key* key = isKeyThere(p);
				if (key && !(key->getIsSeen())) {
					key->setSeen();
					map[p.y][p.x] = KEY_TILE; // update map tile
					key->draw();
				}
			}
			if (isSwitchThere(p)) // if switch there , make it seen
			{
				Switch* sw = isSwitchThere(p);
				if (sw && !(sw->getIsSeen())) {
					sw->setSeen();
					map[p.y][p.x] = sw->getState() ? SWITCH_ON : SWITCH_OFF; // update map tile
					sw->draw();
				}
			}		
		}
	}
}

void Room::getTorchesLineOfSight() {
	for (auto& torch : torches) {
		CompleteLineOfSight(torch);
	}
}

Torch* Room::isTorchThere(Point p)
{
	for (auto& torch : torches)
	{
		Point torchPos = torch.getPos();
		if (torchPos.x == p.x && torchPos.y == p.y)
		{
			return &torch;
		}
	}
	return nullptr;
}

Obstacle* Room::isObstacleThere(Point p)
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
	Obstacle* obs = isObstacleThere(p);
	if (!obs) return false;
	
	if (obs->getHasMoved()) return true;

	if (force < obs->getSize()) return false;

	std::vector<Point> currentParts = obs->getFutureParts(0, 0);
	std::vector<Point> futureParts = obs->getFutureParts(dirx, diry); // complete

	for (const auto& part : currentParts) {
		map[part.y][part.x] = ' ';
	}

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
		for (const auto& part : currentParts)
		{
			bool staysCovered = false;
			for (const auto& newPart : futureParts) {
				if (part.x == newPart.x && part.y == newPart.y)
				{
					staysCovered = true; 
					break;
				}
			}
			if (!staysCovered)
			{
				gotoxy(part.x, part.y);
				std::cout << ' ';
			}
		}

		obs->move(dirx, diry);
		for (const auto& part : futureParts) {
			map[part.y][part.x] = OBSTACLE_TILE;
		}
		obs->draw();
		obs->markAsMoved();
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

char Room::getObjectAt(Point& p){
	Door* door = isDoorThere(p);
	if (door != nullptr) {
		return door->getIsOpen() ? ' ' : door->getPos().getTileChar();
	}

	Key* key = isKeyThere(p);
	if (key != nullptr) {
		if (!(key->getIsActive())) 
			return ' ';
		if (key->getIsSeen())
			return KEY_TILE;
		return UNKNOWN_TILE;
	}
	
	Switch* sw = isSwitchThere(p);
	if (sw != nullptr) {
		return sw->getState() ? SWITCH_ON : SWITCH_OFF;
	}
	//work in progress - instead of obstacleMove function above!
	Obstacle* obstacle = isObstacleThere(p);
	if (obstacle != nullptr) {
		
		// handle move check function
		//move parts function

	}

	return map[p.y][p.x];
}
