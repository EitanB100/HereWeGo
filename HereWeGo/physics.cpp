#include "Room.h"
#include "Player.h"

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
				auto key = isKeyThere(p);
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
