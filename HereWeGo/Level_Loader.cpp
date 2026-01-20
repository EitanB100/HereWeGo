#include "Level_Loader.h"
#include <map>
static std::string trimWhiteSpaces(const std::string& str) {
	int first = str.find_first_not_of(" \t\r\n");
	if (first == std::string::npos) 
		return "";
	int last = str.find_last_not_of(" \t\r\n");
	return str.substr(first, (last - first + 1));
}

static std::string readCleanLine(std::stringstream& parser) {
	std::string txt;
	if (std::getline(parser, txt)) {
		return trimWhiteSpaces(txt);
	}
	return "";
}

//suggested by gemini to get obstacle and spring parts correctly from a file by adjacent parts in advance 
static void consumeConnectedParts(Room& room, int x, int y, char targetCh, std::vector<Point>& outParts, int axis = 0)
{
	// 1. Bounds Check
	if (x < 0 || x >= MAX_X || y < 0 || y >= MAX_Y) return;

	// 2. Stop if it's not the target (or if it's already been eaten/turned to space)
	if (room.getObjectAt({ x, y }) != targetCh) return;

	// 3. "Eat" the tile (Remove it from the map)
	// This is the trick! We modify the map in-place so the main loop skips this tile later.
	Point p = { x,y };
	room.clearTile(p);
	outParts.push_back(p);

	// 4. Recurse neighbors
	if (axis == 0 || axis == 1) {
		consumeConnectedParts(room, x + 1, y, targetCh, outParts, axis);
		consumeConnectedParts(room, x - 1, y, targetCh, outParts, axis);
	}
	if (axis ==0 || axis == 2) {
		consumeConnectedParts(room, x, y + 1, targetCh, outParts, axis);
		consumeConnectedParts(room, x, y - 1, targetCh, outParts, axis);
	}
}

bool Level_Loader::loadLevel(Room& room, const std::string& fileName, std::string& errorMessage)
{
	bool isDiscrepancy = false;
	std::ifstream file(fileName);
	
	if (!file.is_open()) {
		errorMessage = "Error: Could not open level file " + fileName;
		return false;
	}

	room.resetRoom();
	std::string line;
	std::string section = "";

	std::vector<Point> foundKeys;
	std::vector<Point> foundBombs;
	std::vector<Point> foundTorches;
	std::vector<Point> foundSwitches;
	std::vector<Point> foundPotions;
	std::vector<Point> foundRiddles;
	std::map<int, Point> foundDoors; // Maps DoorChar '1' to Point(x,y)


	int keyInd = 0;
	int bombInd = 0;
	int torchInd = 0;
	int potionInd = 0;
	int switchInd = 0;
	int riddleInd = 0;
	int mapRow = 0;

	while (std::getline(file, line)) {
		line = trimWhiteSpaces(line);

		if (section != "MAP") {
			if (line.empty() || line[0] == '#') continue;
		}
		if (line == "[MAP]") { 
			section = "MAP";
			continue;
		}

		if (line == "[/MAP]") {
			section = "";
			std::vector<Obstacle> tempObstacles;

			for (int y = 0; y < MAX_Y; y++) {
				for (int x = 0; x < MAX_X; x++) {
					Point curr = { x,y };
					char c = room.map[y][x];
					if (c == OBSTACLE_TILE) {
						Obstacle obstacle;
						std::vector<Point> parts;

						consumeConnectedParts(room, x, y, OBSTACLE_TILE, parts);
						for (Point part : parts) {
							obstacle.addPart(Placement(part.x,part.y,OBSTACLE_TILE));
						}

						tempObstacles.push_back(obstacle);
					}
					else if (c == '^' || c =='>' || c == 'v' || c == 'V' || c == '<') {
						Point dir;
						int axis = 0;
						if (c == '^') {
							dir = Directions::UP;
							axis = 2;
							
						}
						else if (c == 'v' || c == 'V') {
							dir = Directions::DOWN;
							axis = 2;
						}
						else if (c == '>') {
							dir = Directions::RIGHT;
							axis = 1;
						}
						else if (c == '<') {
							dir = Directions::LEFT;
							axis = 1;
						}

						std::vector<Point> parts;
						consumeConnectedParts(room, x, y, c, parts, axis);

						Spring spring(dir);
						for (const auto& part : parts) {
							spring.addPart(part.x, part.y);
						}

						spring.sortParts();
						room.addSpring(spring);
					}

					switch (c) {

					case KEY_TILE:
						foundKeys.push_back(curr);
						break;
					case BOMB_TILE:
						foundBombs.push_back(curr);
						break;
					case TORCH_TILE:
						foundTorches.push_back(curr);
						break;
					case POTION_TILE:
						foundPotions.push_back(curr);
						break;
					case RIDDLE_TILE:
						foundRiddles.push_back(curr);
						break;
					case SWITCH_ON:
					case SWITCH_OFF:
						foundSwitches.push_back(curr);
						break;
					case EXIT_TILE:
						room.setExitPos(curr);
						room.clearTile(curr);
					}


					if (c >= '1' && c <= '9') {
						foundDoors[c - '0'] = curr;
					}
				}
			}
			for (const auto& obstacle : tempObstacles) {
				room.addObstacle(obstacle);
			}
			continue;
		}
		
		if (line == "[SWITCHES]") {
			section = "SWITCHES";
			continue;
		}
		
		if (line == "[DOORS]") {
			section = "DOORS";
			continue;
		}

		if (line == "[KEYS]") {
			section = "KEYS";
			continue;
		}

		if (line == "[TORCHES]") {
			section = "TORCHES";
			continue;
		}
		
		if (line == "[BOMBS]") {
			section = "BOMBS";
			continue;
		}

		if (line == "[POTIONS]") {
			section = "POTIONS";
			continue;
		}

		if (line == "[RIDDLES]") {
			section = "RIDDLES";
			continue;
		}

		if (line == "[EXIT]") {
			section = "EXIT";
			continue;
		}

		std::stringstream parser(line);


		if (section == "MAP") {
			if (mapRow < MAX_Y) {
				for (int x = 0; x < MAX_X && x < line.length(); x++) {
					char c = line[x];

					if (c == 'L') {
						room.legendLocation = { x, mapRow };
						room.map[mapRow][x] = ' ';
					}
					else if (c == '$' ) {//change to constexpr player sprites later 
						room.setP1Start({ x,mapRow });
						room.map[mapRow][x] = ' ';
					}
					else if (c == '&') {
						room.setP2Start({ x,mapRow });
						room.map[mapRow][x] = ' ';
					}
					else 
						room.map[mapRow][x] = c;
					}
				}

				for (int x = line.length(); x < MAX_X; x++) {
					room.map[mapRow][x] = ' ';
				}
				mapRow++;
			}
		
		
		else if (section == "SWITCHES") {
			int id, savedState, savedSeen;

			if (parser >> id >> savedState >> savedSeen) {
				if (switchInd < foundSwitches.size()) {
					Point p = foundSwitches[switchInd];
					auto sw = std::make_unique<Switch>(p.x, p.y, id);

					// Restore the logical toggle state
					if (savedState == 1) {
						sw->toggleState();
					}

					// Restore the visibility status
					if (savedSeen == 1) {
						sw->setSeen();
					}

					room.addSwitch(std::move(sw));
					switchInd++;
				}
			}
		}

		else if (section == "KEYS") {
			int id, color, seen;
			if (parser >> id >> color >> seen) {
				if (keyInd < foundKeys.size()) {
					Point p = foundKeys[keyInd];
					Key key(p.x, p.y, id, static_cast<Color>(color));
					if (seen) key.setSeen();
					room.addKey(key);
					keyInd++;
				}
				else {
					errorMessage = "Error - More key entries than keys on grid!";
					isDiscrepancy = true;
				}
			}
		}

		else if (section == "DOORS") {
			int id, color, keyCount, switchCount;
			
			if (parser >> id >> color >> keyCount) {
				if (foundDoors.find(id) != foundDoors.end()) {
					Point p = foundDoors[id];
					Door door(p.x, p.y, id, static_cast<Color>(color));

					while (parser.peek() == ' ' || parser.peek() == '[')
						parser.ignore();

					for (int i = 0; i < keyCount; i++) {
						int keyID;
						if (parser >> keyID) {
							door.addRequiredKey(keyID);
						}
					}
					while (parser.peek() == ' ' || parser.peek() == ']')
						parser.ignore();

					if (parser >> switchCount) {

						while (parser.peek() == ' ' || parser.peek() == '[')
							parser.ignore();

						for (int i = 0; i < switchCount; i++) {
							int switchID, switchState;
							if (parser >> switchID >> switchState) {
								Switch* sw = room.getSwitchByID(switchID);
								if (sw) {
									door.addRequiredSwitch(sw, (bool)switchState);
								}
								else {
									errorMessage = "Error - door " + std::to_string(id) + "missing switch " + std::to_string(switchID);
									isDiscrepancy = true;
								}
							}
						}
						while (parser.peek() == ' ' || parser.peek() == ']')
							parser.ignore();
					}
					door.UpdatedFromSwitch();
					room.addDoor(door);
				}
				else {
					errorMessage = "Error - door " + std::to_string(id) + " defined in props but not on grid!";
					isDiscrepancy = true;
				}
			}
		}

		else if (section == "TORCHES") {
			int  radius, color;
			if (parser >> radius) {
				if (torchInd < foundTorches.size()) {
					Point p = foundTorches[torchInd];
					Torch torch(p.x, p.y, radius);
					room.addTorch(torch);
					torchInd++;
				}
				else {
					errorMessage = "Error - More torch entries than torches on grid!";
					isDiscrepancy = true;
				}
			}
			
		}

		else if (section == "BOMBS") {
			int id, timer, seen;
			
			if (parser >> id >> timer >> seen) {
				if (bombInd < foundBombs.size()) {
					Point p = foundBombs[bombInd];
					Bomb bomb(p.x, p.y, id, timer);
					if (seen) bomb.setSeen();
					room.addBomb(bomb);
					bombInd++;
				}
				else {
					errorMessage = "Error - More bomb entries than bombs on grid!";
					isDiscrepancy = true;
				}
			}
			
		}

		else if (section == "POTIONS") {
			int seen;
			if (parser >> seen) {
				if (potionInd < foundPotions.size()) {
					Point& p = foundPotions[potionInd];
					room.addPotion(Potion(p.x, p.y, seen));
					potionInd++;
				}
				else {
					errorMessage = "Error - More potion entries than potions on grid!";
					isDiscrepancy = true;
				}
			}
			
		}

		else if (section == "RIDDLES") {
			int id;
			if (parser >> id ) {
				if (riddleInd < foundRiddles.size()) {
					Point p = foundRiddles[riddleInd];
					room.addRiddle(p.x, p.y, id);
					riddleInd++;
				}
				else {
					errorMessage = "Error - More riddle entries than riddles on grid!";
				}
			}
		}

		else if (section == "EXIT") {
			int dest;
			if (parser >> dest) {
				room.setExitDestination(dest);
			}
		}
	}

	if (room.doors.size() < foundDoors.size()) {
		isDiscrepancy = true;
		errorMessage = "Error: Map has " + std::to_string(foundDoors.size()) + " Doors, but file defines only " + std::to_string(room.doors.size()) + "!";
	}
	//Discrepency checks between tilemap and definitions
	if (keyInd < foundKeys.size()) {
		isDiscrepancy = true;
		errorMessage = "Error: Map has " + std::to_string(foundKeys.size()) + " Keys, but file defines only " + std::to_string(keyInd) + "!";
	}
	
	if (switchInd < foundSwitches.size()) {
		isDiscrepancy = true;
		errorMessage = "Error: Map has " + std::to_string(foundSwitches.size()) + " Switches, but file defines only " + std::to_string(switchInd) + "!";
	}
	if (riddleInd < foundRiddles.size()) {
		isDiscrepancy = true;
		errorMessage = "Error: Map has " + std::to_string(foundRiddles.size()) + " Riddles, but file defines only " + std::to_string(riddleInd) + "!";
	}
	
	if (bombInd < foundBombs.size()) {
		isDiscrepancy = true;
		errorMessage = "Error: Map has " + std::to_string(foundBombs.size()) + " Bombs, but file defines only " + std::to_string(bombInd) + "!";
		
	}
	
	if (torchInd < foundTorches.size()) {
		isDiscrepancy = true;
		errorMessage = "Error: Map has " + std::to_string(foundTorches.size()) + " Torches, but file defines only " + std::to_string(torchInd) +"!" ;
		
	}

	file.close();

	if (isDiscrepancy) {
		return false;
	}
	return true;
}

void Level_Loader::loadRiddles(const std::string& fileName, std::vector<Riddle>& outRiddles) {
	std::ifstream file(fileName);
	if (!file.is_open()) return;

	std::string line;
	Riddle currentRiddle;
	bool isReading = false;

	while (std::getline(file, line)) {
		std::stringstream parser(line);
		std::string section;
		
		if (!(parser >> section)) continue;
			
		if (section == "[RIDDLE]") {
				if (isReading) 
					outRiddles.push_back(currentRiddle);
				
				currentRiddle = Riddle();
				isReading = true;
			}

		else if (section == "ID") {
			int tempID;
			if (parser >> tempID) 
				currentRiddle.id = tempID;
		}

		else if (section == "Q:") {
			currentRiddle.question = readCleanLine(parser);
		}

		else if (section == "A:") {
			std::string option = readCleanLine(parser);

			if (!option.empty())
				currentRiddle.options.push_back(option);
		}

		else if (section == "CORRECT:") {
			int val;
			
			if (parser >> val) {
				currentRiddle.correctAnswer = val;
			}
			else {
				std::cerr << "Warning - invalid index for correct answer for riddle" << currentRiddle.id << std::endl;
				currentRiddle.correctAnswer = -1;
			}
		}
	}
	
	if (isReading) {
		outRiddles.push_back(currentRiddle);
	}
	file.close();
}

void Level_Loader::saveLevel(const Room& room, const std::string& fileName, Point p1Pos, Point p2Pos) {
	std::ofstream file(fileName);
	if (!file.is_open()) return;

	std::vector<std::string> tempMap(MAX_Y, std::string(MAX_X, ' '));
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			// Only keep permanent structures like Walls 'W'
			if (room.map[y][x] == 'W') {
				tempMap[y][x] = room.map[y][x];
			}
		}
	}

	//Overwrite positions using ACTUAL object data
	for (const Key& key : room.keys) tempMap[key.getPos().y][key.getPos().x] = KEY_TILE;
	for (const Bomb& bomb : room.bombs) tempMap[bomb.getPos().y][bomb.getPos().x] = BOMB_TILE;
	for (const Potion& potion : room.potions) tempMap[potion.getPos().y][potion.getPos().x] = POTION_TILE;
	for (const auto& r : room.riddleLocations) tempMap[r.p.y][r.p.x] = RIDDLE_TILE;
	for (const Torch& t : room.torches) tempMap[t.getPos().y][t.getPos().x] = TORCH_TILE;

	// Restore Obstacles and Springs using stored parts
	for (const auto& obs : room.obstacles) {
		std::vector<Point> obsParts;
		obs.getParts(obsParts);
		for (const auto& p : obsParts) {
			if (p.x >= 0 && p.x < MAX_X && p.y >= 0 && p.y < MAX_Y)
				tempMap[p.y][p.x] = OBSTACLE_TILE;
		}
	}

	for (const auto& spr : room.springs) {
		char sprChar = ' ';
		if (spr.getDirection() == Directions::UP) sprChar = '^';
		else if (spr.getDirection() == Directions::DOWN) sprChar = 'v';
		else if (spr.getDirection() == Directions::LEFT) sprChar = '<';
		else if (spr.getDirection() == Directions::RIGHT) sprChar = '>';

		std::vector<Point> sprParts;
		spr.getParts(sprParts);
		for (const auto& p : sprParts) {
			if (p.x >= 0 && p.x < MAX_X && p.y >= 0 && p.y < MAX_Y)
				tempMap[p.y][p.x] = sprChar;
		}
	}

	// Write the reconstructed [MAP]
	file << "[MAP]\n";
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			Point current = { x, y };

			// Players, Legend, Exit
			if (current == p1Pos) file << '$';
			else if (current == p2Pos) file << '&';
			else if (current == room.legendLocation) file << 'L';
			else if (current == room.getExitPos()) file << EXIT_TILE;
			else {
				// Doors (MUST be saved as their ID '1'-'9' for the loader)
				bool isDoor = false;
				for (const auto& door : room.doors) {
					if (door.getPos().getx() == x && door.getPos().gety() == y) {
						file << (char)('0' + door.getDoorID());
						isDoor = true;
						break;
					}
				}
				// Switches (MUST be saved as SWITCH_OFF in map section)
				if (!isDoor) {
					bool isSwitch = false;
					for (const auto& sw : room.switches) {
						if (sw->getPos().x == x && sw->getPos().y == y) {
							file << sw->stateChar();
							isSwitch = true;
							break;
						}
					}
					//Everything else
					if (!isSwitch) file << tempMap[y][x];
				}

			}
		}
		file << "\n";
	}
	file << "[/MAP]\n\n";

	file << "\n[KEYS]\n"; // save Keys
	for (const auto& key : room.keys) {
		file << key.getKeyID() << " " << static_cast<int>(key.getColor()) << " " << (key.getIsSeen() ? 1 : 0) << "\n";
	}

	file << "\n[SWITCHES]\n"; // save Switches
	for (const auto& sw : room.switches) {
		file << sw->getSwitchID() << " " << (sw->getState() ? 1 : 0) << " " << (sw->getIsSeen() ? 1 : 0) << "\n";
	}
	
	file << "\n[DOORS]\n";
	for (const auto& door : room.doors) {
		file << door.getDoorID() << " " << static_cast<int>(door.getColor()) << " " << door.getRequiredKeys().size();
		for (int kid : door.getRequiredKeys()) {
			file << " " << kid;
		}
		auto swReqs = door.getRequiredSwitches();
		file << " " << swReqs.size(); // This is the switchCount for the loader
		for (auto const& [swPtr, state] : swReqs) {
			file << " " << swPtr->getSwitchID() << " " << (state ? 1 : 0);
		}
		file << " " << 1 << "\n"; // doors always seen 
	}

	file << "\n[BOMBS]\n"; // save Bombs
	for (const auto& b : room.bombs) file << b.getBombID() << " " << b.getTimer() << " " << (b.getIsSeen() ? 1 : 0) << "\n";

	file << "\n[TORCHES]\n"; // save Torches
	for (const auto& t : room.torches) file << t.getLineOfSight() << "\n";

	file << "\n[POTIONS]\n"; // save Potions
	for (const auto& p : room.potions) file << (p.getIsSeen() ? 1 : 0) << "\n";

	file << "\n[RIDDLES]\n"; // save Riddles
	for (const auto& r : room.riddleLocations) file << r.id << "\n";

	file.close();
}