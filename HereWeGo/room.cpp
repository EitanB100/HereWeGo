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
	for (Switch* switchOnOff : switches) switchOnOff->draw();
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

void Room::addBomb(const Bomb& bomb){
	Point bombPos = bomb.getPos();
	if (bomb.getIsSeen())
		map[bombPos.y][bombPos.x] = BOMB_TILE;
	else
		map[bombPos.y][bombPos.x] = UNKNOWN_TILE; // Draw as unknown if not seen
	bombs.push_back(bomb);
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

void Room::addSwitch(std::unique_ptr<Switch> s) {
	Point switchPos = s->getPos();
	if (switchPos.x >= 0 && switchPos.x < MAX_X && switchPos.y >= 0 && switchPos.y < MAX_Y) {
		map[switchPos.y][switchPos.x] = SWITCH_OFF; // Ensure SWITCH_OFF is defined in Tile_Chars.h
		switches.push_back(std::move(s)); 
	}
}

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

void Room::CompleteLineOfSight(const Torch& torch) {
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
			if (isBombThere(p)) // if bomb there , make it seen
			{
				Bomb* bomb = isBombThere(p);
				if (bomb && !(bomb->getIsSeen())) {
					bomb->setSeen();
					map[p.y][p.x] = BOMB_TILE; // update map tile
					bomb->draw();
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

//logic for handling obstacle movement
bool Room::moveObstacle(Point p, int dirx, int diry, int force)
{
	Obstacle* obs = isObstacleThere(p);
	if (!obs) return false;
	
	//if already moved this frame
	if (obs->getHasMoved()) return true;

	//early force check
	if (force < obs->getSize()) return false;
	
	//get all parts of obstacle for validity check
	std::vector<Point> currentParts = obs->getFutureParts(0, 0);
	std::vector<Point> futureParts = obs->getFutureParts(dirx, diry); // complete

	//temporarily clear the map at current position to avoid self collision
	for (const auto& part : currentParts) {
		map[part.y][part.x] = ' ';
	}

	bool canMove = true;

	//check collision for every part in the new position
	for (auto& futurePart : futureParts)
	{
		//boundaries
		if (futurePart.x < 0 || futurePart.x > MAX_X - 1 || futurePart.y < 0 || futurePart.y > MAX_Y - 1)
		{
			canMove = false;
			break;
		}

		//objects
		char tile = map[futurePart.y][futurePart.x];
		if (tile != ' ') 
		{
			//can move through doors
			Door* door = isDoorThere(futurePart);
			if (door != nullptr && door->getIsOpen()) continue; // move obstacle part through a door

			canMove = false;
			break;
		}
	}

	if (canMove) {
		//clear the parts from old position
		for (const auto& part : currentParts)
		{
			bool staysCovered = false;
			for (const auto& newPart : futureParts) {
				if (part == newPart)
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

		//prevent double moving in a frame
		obs->markAsMoved();
		return true;
	}
	else {
		//movement failed, restore for the current position
		for (const auto& part : currentParts)
		{
			map[part.y][part.x] = OBSTACLE_TILE;
		}
		obs->draw();
		return false;
	}

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
	Obstacle* obstacle = isObstacleThere(p);
	if (obstacle != nullptr) {
		color = Color::WHITE;
		return OBSTACLE_TILE;

	}
	//interactables:
	
	//keys
	Key* key = isKeyThere(p);
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
	Torch* torch = isTorchThere(p);
	if (torch != nullptr) {
		color = torch->getColor();
		return TORCH_TILE;
	}

	//static:
	//doors:
	Door* door = isDoorThere(p);
	if (door != nullptr) {
		if (door->getIsOpen()) color = door->getColor();

		return door->getIsOpen() ? ' ' : door->getPos().getTileChar();
	}
	//bombs
	Bomb* bomb = isBombThere(p);
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
	Switch* sw = isSwitchThere(p);
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



void Room::bombExplode(Bomb* bomb, Player* players, int playerCount, Screen& screen) {
	Point blastCenter = bomb->getPos();
	int blastRadius = bomb->getBlastRadius();

	//Apply damage to players
	for (int i = 0; i < playerCount; ++i) {
		Point pPos = players[i].getPos();
		int dx = abs(pPos.x - blastCenter.x);
		int dy = abs(pPos.y - blastCenter.y);
		int distance = (dx > dy) ? dx : dy;

		if (distance <= blastRadius && PointhasLineOfSight(blastCenter.x, blastCenter.y, pPos.x, pPos.y)) {
			int damage = (blastRadius - distance + 1) * 5;
			players[i].takeDamage(damage);
		}
	}

	for (int d = blastRadius; d >= 0; d--) { 	// Loop from the outside (blastRadius) down to the center , needed so walls and obstacles which block clearned last
		for (int y = blastCenter.y - d; y <= blastCenter.y + d; y++) { 		// Iterate through the square shell at distance 'd'
			for (int x = blastCenter.x - d; x <= blastCenter.x + d; x++) {
				if (abs(x - blastCenter.x) == d || abs(y - blastCenter.y) == d) { // We only want to process the "border" of the current square shell this ensures we go ring-by-ring
					if (x < 0 || y < 0 || x >= MAX_X || y >= MAX_Y) continue;
					Point p{ x, y };
					if (PointhasLineOfSight(blastCenter.x, blastCenter.y, x, y)) {
						// Clear items and obstacles 
						if (isKeyThere(p)) removeKey(p);
						if (isSwitchThere(p)) removeSwitch(p);
						if (isObstacleThere(p)) removeObstacle(p);
						if (isWallThere(p)) map[p.y][p.x] = ' ';
						if (isTorchThere(p)) removeTorch(p);
						if (isSpringThere(p)) removeSpring(p);
						if (isBombThere(p)) {
							Bomb* otherBomb = isBombThere(p);
							if (otherBomb != nullptr && otherBomb != bomb) {
								otherBomb->activate(); // Chain reaction
							}
						}

						// Draw the explosion char onto the room map buffer
						map[y][x] = bomb->getExplosionChar();
					}
				}
			}
		}
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