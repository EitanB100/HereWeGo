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
	for (Door& door : doors) door.draw();
	for (Key& key : keys) key.draw();
	for (auto& switchOnOff : switches) switchOnOff->draw();
	for (Torch& torch : torches) torch.draw();
	for (Obstacle& obstacle : obstacles) obstacle.draw();
	for (Spring& spring : springs) spring.draw();
	for (Bomb& bomb : bombs) bomb.draw();
	getTorchesLineOfSight();
	
}


void Room::drawRoom(Screen& screen) {
	screen.loadMapFromRoom(this->map);
}

void Room::loadFromScreen(Screen& screen) // Load the room from the screen
{
	for (int y = 0; y < MAX_Y; y++)
	{
		for (int x = 0; x < MAX_X; x++)
		{
			map[y][x] = screen.getTile(x, y);
		}
	}

	//suggested by Gemini as a bug fix for obstacles not colliding with each other when moved
	for (const auto& obs : obstacles) {
		// Get current parts (0,0 direction means current position)
		std::vector<Point> parts = obs.getFutureParts(0, 0);
		for (const auto& p : parts) {
			if (p.x >= 0 && p.x < MAX_X && p.y >= 0 && p.y < MAX_Y) {
				map[p.y][p.x] = OBSTACLE_TILE;
			}
		}
	}

	for (Spring& spring : springs) {
		for (const auto& part : spring.getParts()) {
			map[part.gety()][part.getx()] = SPRING_TILE;
		}
	}
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


void Room::addObstacle(const Obstacle& obs)
{
	obstacles.push_back(obs);

	std::vector<Point> currParts = obs.getFutureParts(0, 0);
	for (const auto& part : currParts)
	{

		map[part.y][part.x] = OBSTACLE_TILE;
	}
}

void Room::addWall(const Point& p)
{
	map[p.y][p.x] = WALL_TILE;
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

		Point p{ x1, y1 };

		if (isObstacleThere(p) || isWallThere(p)) //  obstacle or wall in the way
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