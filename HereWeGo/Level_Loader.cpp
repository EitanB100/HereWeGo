#include "Level_Loader.h"
#include <map>
static std::string trimWhiteSpaces(const std::string& str) {
	int first = str.find_first_not_of(" \t");
	if (first == std::string::npos) 
		return "";

	return str.substr(first);
}

static std::string readCleanLine(std::stringstream& parser) {
	std::string txt;
	if (std::getline(parser, txt)) {
		return trimWhiteSpaces(txt);
	}
	return "";
}

//suggested by gemini to get obstacle and spring parts correctly from a file by adjacent parts in advance 
// Helper: Finds connected parts and REMOVES them from the map so they aren't processed twice.
static void consumeConnectedParts(Room& room, int x, int y, char targetCh, std::vector<Point>& outParts)
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
	consumeConnectedParts(room, x + 1, y, targetCh, outParts);
	consumeConnectedParts(room, x - 1, y, targetCh, outParts);
	consumeConnectedParts(room, x, y + 1, targetCh, outParts);
	consumeConnectedParts(room, x, y - 1, targetCh, outParts);
}

void Level_Loader::loadLevel(Room& room, const std::string& fileName)
{
	std::ifstream file(fileName);
	
	if (!file.is_open()) {
		std::cerr << "Error: Could not open level file " << fileName << std::endl;
		return;
	}

	room.resetRoom();
	std::string line;
	std::string section = "";

	std::vector<Point> foundKeys;
	std::vector<Point> foundBombs;
	std::vector<Point> foundTorches;
	std::vector<Point> foundPotions;
	std::vector<Point> foundSwitches;
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
		if (line.empty() || line[0] == '#') continue;

		if (line == "[MAP]") { 
			section = "MAP";
			continue;
		}

		if (line == "[/MAP]") {
			section = "";
			
			for (int y = 0; y < MAX_Y; y++) {
				for (int x = 0; x < MAX_X; x++) {
					Point curr = { x,y };
					char c = room.getObjectAt({x,y});
					if (c == OBSTACLE_TILE) {
						Obstacle obstacle;
						std::vector<Point> parts;

						consumeConnectedParts(room, x, y, OBSTACLE_TILE, parts);
						for (Point part : parts) {
							obstacle.addPart(Placement(part.x,part.y,OBSTACLE_TILE));
						}

						room.addObstacle(obstacle);
					}
					else if (c == SPRING_TILE) {
						std::vector<Point> parts;
						consumeConnectedParts(room, x, y, SPRING_TILE, parts);

						int dirX = 0, dirY = 0;
						bool dirFound = false;

						for (Point part : parts) {
							if (part.y > 0 && room.getObjectAt(Point{ part.x, part.y - 1 }) == 'W') { dirX = 0; dirY = 1; dirFound = true; break; }
							if (part.y < MAX_Y - 1 && room.getObjectAt(Point{ part.x, part.y + 1 }) == 'W') { dirX = 0; dirY = -1; dirFound = true; break; }
							if (part.x > 0 && room.getObjectAt(Point{ part.x - 1, part.y }) == 'W') { dirX = 1; dirY = 0; dirFound = true; break; }
							if (part.x < MAX_X - 1 && room.getObjectAt(Point{ part.x + 1, part.y }) == 'W') { dirX = -1; dirY = 0; dirFound = true; break; }
						}
						if (!dirFound) {
							dirX = 0;
							dirY = -1;
						}
						Spring spring({ dirX,dirY });
						for (Point part : parts) {
							spring.addPart(part.x,part.y);
						}
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
					}
					
					if (c >= '1' && c <= '9') {
						foundDoors[c - '0'] = curr;
					}
				}
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

		if (line == "[OBSTACLES]") {
			section = "OBSTACLES";
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

		std::stringstream parser(line);


		if (section == "MAP") {
			if (mapRow < MAX_Y) {
				for (int x = 0; x < MAX_X && x < line.length(); x++) {
					char c = line[x];

					if (c == 'L') {
						room.legendLocation = { x, mapRow };
						room.map[mapRow][x] = ' ';
					}
					else {
						room.map[mapRow][x] = c;
					}
				}

				for (int x = line.length(); x < MAX_X; x++) {
					room.map[mapRow][x] = ' ';
				}
				mapRow++;
			}
		}
		
		else if (section == "SWITCHES") {
			int id, seen;
			if (parser >> id >> seen) {
				if (switchInd < foundSwitches.size()) {
					Point p = foundSwitches[switchInd];
					auto sw = std::make_unique<Switch>(p.x, p.y, id);
					if (seen) sw->setSeen();
					room.addSwitch(std::move(sw));
					switchInd++;
				}
				else {
					std::cerr << "Error - More switch entries than switches on grid!" << std::endl;
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
					std::cerr << "Error - More key entries than keys on grid!" << std::endl;
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
									std::cerr << "Error - door " << id << "missing switch " << switchID << std::endl;
								}
							}
						}
						while (parser.peek() == ' ' || parser.peek() == ']')
							parser.ignore();
					}
					door.UpdatedFromSwitch();
					room.addDoor(door);
				}
				else
					std::cerr << "Error - door "<< id << " defined in props but not on grid!" << std::endl;
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
			}
		}

		else if (section == "BOMBS") {
			int id, timer;
			
			if (parser >> id >> timer) {
				if (bombInd < foundBombs.size()) {
					Point p = foundBombs[bombInd];
					Bomb bomb(p.x, p.y, id, timer);
					room.addBomb(bomb);
					bombInd++;
				}
			}
		}

		else if (section == "POTIONS") {
			if (potionInd < foundPotions.size()) {
				Point p = foundPotions[potionInd];
				Potion potion(p.x, p.y);
				room.addPotion(potion);
				potionInd++;
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
					std::cerr << "Error - More riddle entries than riddles on grid!" << std::endl;
				}
			}
		}
	}

	while (keyInd < foundKeys.size()) {
		room.clearTile(foundKeys[keyInd++]);
	}
	while (switchInd < foundSwitches.size()) {
		room.clearTile(foundSwitches[switchInd++]);
	}
	while (riddleInd < foundRiddles.size()) {
		room.clearTile(foundRiddles[riddleInd++]);
	}
	while (bombInd < foundBombs.size()) {
		room.clearTile(foundBombs[bombInd++]);
	}
	file.close();
}

void Level_Loader::loadRiddles(const std::string& fileName, std::vector<Riddle>& outRiddles) {
	std::ifstream file(fileName);
	if (!file.is_open()) return;

	std::string line;
	Riddle currentRiddle;
	bool isReading = false;

	while (std::getline(file, line)) {
		if (line.empty()) continue;

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