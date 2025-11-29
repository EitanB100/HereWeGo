#include "Room.h"
#include "Key.h"
#include "Door.h"
#include "Obstacle.h"

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
		Point doorPos = door.getPos();
		if (doorPos.x == p.x && doorPos.y == p.y)
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
	if (door->getIsOpen()) return true;
	
	if (item.type == KEY) 
	{
		if (door->tryUnlock(item.id))
			item = { NONE, 0 };
	}

	return (door->getIsOpen());
}

void Room::addDoor(Door& door) {

	Point doorPos = door.getPos();

	map[doorPos.y][doorPos.x] = door.getDoorID();

	doors.push_back(door);
}

void Room::addWall(int x, int y)
{
	map[y][x] = WALL;
}


void Room::addKey(int x, int y, int keyID) {

	map[y][x] = KEY;


}


char Room::getObjectAt(Point& p) const
{
	return map[p.y][p.x];
}



void Room::addWall(int index, const Wall& wall)
{
	if (walls != nullptr && index < numWalls) {
		walls[index] = wall;
	}
}

void Room::addObstacle(int index, int x, int y)
{
	if (obstacles != nullptr && index < numObstacles) {
		Placement part(x, y, '*');
		obstacles[index].addPart(part);
	}
}


	

bool Room::moveObstacle(Obstacle* obstacle, int dirx, int diry, int playerForce)
{
	if (playerForce < obstacle->getSize()) return false; // Not enough force to move the obstacle
	


	return false;
}
