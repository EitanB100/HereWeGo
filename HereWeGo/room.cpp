#include "Room.h"
#include "Key.h"
#include "Door.h"
<<<<<<< Updated upstream
#include "Wall.h"
#include "Obstacle.h"

void Room::drawRoom(Screen& screen)
{
	drawPerimiterWalls(screen);
	drawInternalWalls(screen);
	drawDoors(screen);
	drawKeys(screen);
	//drawObstacles(screen); // to be implemented later
}

//perimeter walls
void Room::drawPerimiterWalls(Screen& screen)
{
	int rows = getHeight();
	int cols = getWidth();
	int startX = pos.getx();
	int startY = pos.gety();

	
	for (int i = 0; i < cols; i++) {	// Top edge
		screen.setTile(startX + i, startY, symbol);
	}

	for (int i = 1; i < rows - 1; i++) {	// Sides
		screen.setTile(startX, startY + i, symbol);
		screen.setTile(startX + cols - 1, startY + i, symbol);
	}

	for (int i = 0; i < cols; i++) {	// Bottom edge
		screen.setTile(startX + i, startY + rows - 1, symbol);
	}
}

// Draw internal walls
void Room::drawInternalWalls(Screen& screen)
{
	for (int i = 0; i < numWalls; i++)
	{
		if (walls[i].isActiveWall())
		{
			Placement wallPos = walls[i].getPosition();
			screen.setTile(wallPos.getx(), wallPos.gety(), wallPos.getTileChar());
=======
#include "Switch.h"
#include "Obstacle.h"

void Room::drawTopLayer()
{
	for (Door& door : doors) door.draw();
	for (Key& key : keys) key.draw();
	for (Switch& switchOnOff : switches) switchOnOff.draw();
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

	if (item.type == KEY) 
	{
		if (door->tryUnlock(item.id))
		{
			item = { NONE, 0, Color::WHITE};
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
>>>>>>> Stashed changes
		}
	}
}

<<<<<<< Updated upstream
//Draw doors
void Room::drawDoors(Screen& screen)
{
	for (int i = 0; i < numDoors; i++)
	{
		Placement p = doors[i].getPosition();

		char doorChar = doors[i].getIsOpen() ? ' ' : p.getTileChar();
		screen.setTile(p.getx(), p.gety(), doorChar);
	}
}

//Draw keys
void Room::drawKeys(Screen& screen)
{

	for (int i = 0; i < numKeys; i++)
	{
		if (keys[i].getIsActive())
		{
			Placement p = keys[i].getPosition();
			screen.setTile(p.getx(), p.gety(), p.getTileChar());
		}
	}
}

char Room::getObjectAt(Point& p) const
{
	Door* doorAtPos = isDoorThere(p);
	if (doorAtPos != nullptr)
		return (*doorAtPos).getIsOpen() ? ' ' : 'D'; // Return 'D' for closed door, ' ' for open door

	Key* keyAtPos = isKeyThere(p);
	if (keyAtPos != nullptr)
		return 'K'; // Return 'K' for key

	if (!inBounds(p)) 
		return 'W'; // Wall symbol if out of bounds
	
	Wall* wallAtPos = isWallThere(p);
	if (wallAtPos != nullptr)
		return 'W'; // Return 'W' for wall

	Obstacle* obstacleAtPos = isObstacleThere(p);
	if (obstacleAtPos != nullptr)
		return '*'; // Return '*' for obstacle

	return ' '; // Empty space if no object found
}

void Room::initializeArrays(int doorCount, int keyCount, int wallCount, int ObstacleCount) { // Initialize doors,keys and internal walls arrays

	delete[] doors; // Free existing memory if any
	delete[] keys;
	delete[] walls;
	delete[] obstacles;

	if (keyCount > 0 && doorCount > 0) {
		doors = new Door[doorCount];
		keys = new Key[keyCount];

		numDoors = doorCount;
		numKeys = keyCount;
	}

	if (wallCount > 0)
	{
		walls = new Wall[wallCount];
		numWalls = wallCount;
	}

	if (ObstacleCount > 0)
	{
		obstacles = new Obstacle[ObstacleCount];
		numObstacles = ObstacleCount;
	}


}

void Room:: addDoor(int index, const Door& door) {
	if (doors != nullptr && index < numDoors) {
		doors[index] = door;
	}
}

void Room::addKey(int index, const Key& key) {
	if (keys != nullptr && index < numKeys) {
		keys[index] = key;
	}
}

void Room::addWall(int index, const Wall& wall)
{
	if (walls != nullptr && index < numWalls) {
		walls[index] = wall;
	}
}

void Room::addObstacle(int index, int x, int y)
=======
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

void Room::addSwitch(const Switch& Switch) {
	Point SwitchPos = Switch.getPos();
	if (SwitchPos.x >= 0 && SwitchPos.x < MAX_X && SwitchPos.y >= 0 && SwitchPos.y < MAX_Y) {
		map[SwitchPos.y][SwitchPos.x] = SWITCH_OFF;
		switches.push_back(Switch);
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

	return map[p.y][p.x];
}



/*void Room::addObstacle(int index, int x, int y)
>>>>>>> Stashed changes
{
	if (obstacles != nullptr && index < numObstacles) {
		Placement part(x, y, '*');
		obstacles[index].addPart(part);
	}
}

<<<<<<< Updated upstream
Door* Room::isDoorThere(Point& p) const
{

	for (int i = 0; i < numDoors; i++)
	{
		if (doors[i].isAtPosition(p)) return &doors[i];
	}
	return nullptr; // Return default door if none found
}

bool Room::checkDoor(Point p, const heldItem& item)
{
	Door* door = isDoorThere(p);
	if (door == nullptr) return false; // No door at this position
	if (door->getIsOpen()) return true; // Door already open

	if (item.type == KEY && item.id == door->getRequiredKeyID())
	{
		door->openDoor(true); // Open the door
		return true;
	}
	return false;
}
	

Key* Room::isKeyThere(Point& p) const
{

	for (int i = 0; i < numKeys; i++)
	{
		if (keys[i].isAtPosition(p)) return &keys[i];
	}
	return nullptr; // Return default key if none found
}

Wall* Room::isWallThere(Point& p) const
{

	for (int i = 0; i < numWalls; i++) // internal walls
	{
		if (walls[i].isAt(p)) return &walls[i];
	}

	return nullptr; // No wall found (perimiter walls are not included here)
}

Obstacle* Room::isObstacleThere(Point& p) const
{
	for (int i = 0; i < numObstacles; i++)
	{
		if (obstacles[i].isAt(p)) return &obstacles[i];
	}

		return nullptr;
}

=======

	

>>>>>>> Stashed changes
bool Room::moveObstacle(Obstacle* obstacle, int dirx, int diry, int playerForce)
{
	if (playerForce < obstacle->getSize()) return false; // Not enough force to move the obstacle
	


	return false;
<<<<<<< Updated upstream
}
=======
}*/
>>>>>>> Stashed changes
