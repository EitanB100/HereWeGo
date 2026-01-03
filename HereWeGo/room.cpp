#include "Room.h"
#include "Key.h"
#include "Door.h"
#include "Switch.h"  
#include "Obstacle.h"
#include "Bomb.h"
#include "Player.h"
#include "Tile_Chars.h"

//grid initialization
Room::Room() { 
	for (int y = 1; y < MAX_Y; y++)
	{
		for (int x = 0; x < MAX_X; x++)
		{
			if (y == 1 || y == MAX_Y - 1 || x == 0 || x == MAX_X - 1)
				map[y][x] = WALL_TILE;
			else
				map[y][x] = ' ';
		}
	}
}

//redraws all non wall objects on top
void Room::drawTopLayer()
{
	for (auto& switchOnOff : switches) switchOnOff->draw(); 
	for (Key& key : keys) key.draw();
	for (Torch& torch : torches) torch.draw();	
	for (Potion& potion : potions) potion.draw();
	for (Bomb& bomb : bombs) bomb.draw();
	for (Spring& spring : springs) spring.draw();
	for (Door& door : doors) door.draw();
	for (Obstacle& obstacle : obstacles) obstacle.draw();

	getTorchesLineOfSight();
	
}


void Room::drawRoom(Screen& screen) {
	screen.loadMapFromRoom(this->map);
}

void Room::resetRoom()
{
	doors.clear();
	keys.clear();
	switches.clear(); 
	obstacles.clear();
	torches.clear();
	springs.clear();
	bombs.clear();
}

bool Room::checkDoor(Point p, heldItem& item)
{
	Door* door = isDoorThere(p);
	if (door == nullptr) return false;

	// If door is already open, we can pass (optional check)
	if (door->getIsOpen()) return true;

	if (item.type == ItemType::KEY) 
	{
		if (door->tryUnlock(item.id))
		{
			item = { ItemType::NONE, 0, Color::WHITE}; // Consume key
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

Switch* Room::getSwitchByID(int id) {
	// Iterate through all switches in the list
	for (const auto& sw : switches) {
		if (sw && sw->getSwitchID() == id) {
			return sw.get();
		}
	}

	return nullptr;
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

		if (map[y1][x1] == WALL_TILE || map[y1][x1] == OBSTACLE_TILE) //  obstacle or wall in the way
			return false;

		int comperedError = 2 * balance; // go back one tile in x or y direction close to the torch
		if (comperedError > -distanceY) { balance -= distanceY; x1 += directionX; }
		if (comperedError < distanceX) { balance += distanceX; y1 += directionY; }
	}
}


void Room::getTorchesLineOfSight() {
	for (auto& torch : torches) {
		CompleteLineOfSight(torch);
	}
}





void Room::updateBombs(Player* players, int playerCount, Screen& screen) {
	for (int i = bombs.size() - 1; i >= 0; i--) {
		bombs[i].gonnaExplode();

		if (bombs[i].getTimer() <= 0) {
			bombExplode(&bombs[i], players, playerCount, screen); // Pass screen
			bombs.erase(bombs.begin() + i);
			drawRoom(screen);     
			screen.draw();        
			drawTopLayer();       
		}
	}
}

void Room::clearExplosions() {
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			// Check if the tile is an explosion character
			if (map[y][x] == EXPLOSION_TILE) {
				map[y][x] = ' '; // Revert to empty floor
			}
		}
	}
}

bool Room::hasExplosions() {
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			if (map[y][x] == EXPLOSION_TILE) return true;
		}
	}
	return false;
}

Point Room::getRiddlePos(int id) const
{
	for (const auto& riddle : riddleLocations) {
		if (riddle.id == id) return riddle.p;
	}
	return { -1,-1 };
}

int Room::getRiddleID(const Point& p) const
{
	const RiddlePos* riddle = isRiddleThere(p);
	if (riddle) return riddle->id;
	return 0;
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
	if (p.x < 0 || p.x >= MAX_X || p.y < 0 || p.y >= MAX_Y) {
		color = Color::WHITE;
		return ' ';
	}
	
	char mapChar = map[p.y][p.x];

	if (mapChar == ' ' || mapChar == WALL_TILE || mapChar == SPRING_TILE || mapChar == GLASS_TILE || mapChar == OBSTACLE_TILE) {
		color = Color::WHITE;

		return mapChar;
	}

	
	//interactables:

	//keys
	if (mapChar == KEY_TILE) {
		auto key = isKeyThere(p);
		if (key != nullptr) {
			color = key->getColor();
			return KEY_TILE;
		}
		return ' ';
	}

	//torches
	if (mapChar == TORCH_TILE) {
		auto torch = isTorchThere(p);
		if (torch != nullptr) {
			color = torch->getColor();
			return TORCH_TILE;
		}
		return ' ';
	}
	//doors
	if (isDoorTile(mapChar)) {
		auto door = isDoorThere(p);
		if (door != nullptr) {
			// Note: Map keeps the '1' even if open, so we check state here
			if (door->getIsOpen()) {
				color = door->getColor();
				return ' '; // Draw space if open
			}
			color = door->getColor();
			return mapChar;
		}
		return ' ';
	}
	//bombs
	if (mapChar == BOMB_TILE) {
		auto bomb = isBombThere(p);
		if (bomb != nullptr) {
			color = bomb->getColor();
			return BOMB_TILE;
		}
		return ' ';
	}

	//switches:
	if (isSwitchTile(mapChar)) {
		auto sw = isSwitchThere(p);
		if (sw != nullptr) {
			// Map might say OFF ('\') but switch is ON ('/'), so we trust the object
			if (sw->getIsSeen()) {
				color = sw->getState() ? Color::GREEN : Color::RED;
				return sw->getState() ? SWITCH_ON : SWITCH_OFF;
			}
			// If not seen, it falls through to UNKNOWN_TILE logic usually, 
			// but if map has the switch char, we treat it as visible.
		}
		return ' ';
	}
	
	if (mapChar == POTION_TILE) {
		auto potion = isPotionThere(p);
		if (potion != nullptr) {
			color = potion->getColor();
			return POTION_TILE;
		}
		return ' ';
	}

	if (mapChar == UNKNOWN_TILE) {
		color = Color::WHITE;
		return UNKNOWN_TILE;
	}

	color = Color::WHITE;
	return mapChar;
}
