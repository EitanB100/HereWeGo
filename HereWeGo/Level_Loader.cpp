#include "Level_Loader.h"

static std::string trimWhiteSpaces(const std::string& str) {
	int first = str.find_first_not_of(" \t");
	if (first == std::string::npos) return "";
	return str.substr(first);
}

static std::string readCleanLine(std::stringstream& parser) {
	std::string txt;
	if (std::getline(parser, txt)) {
		return trimWhiteSpaces(txt);
	}
	else c
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

	int mapRow = 0;

	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#') continue;

		if (line == "[MAP]") { 
			section = "MAP";
			continue;
		}

		if (line == "[/MAP]") {
			section = "";
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
			int id, x, y, seen;
			if (parser >> id >> x >> y >> seen) {
				auto sw = std::make_unique<Switch>(x, y, id);
				if (seen) sw->setSeen();
				room.addSwitch(std::move(sw));
			}
		}

		else if (section == "KEYS") {
			int id, x, y, color, seen;
			if (parser >> id >> x >> y >> color >> seen) {
				Key key (x, y, id, static_cast<Color>(color));
				if (seen) key.setSeen();
				room.addKey(key);
			}
		}

		else if (section == "DOORS") {
			int id, x, y, color, keyCount, switchCount;

			if (parser >> id >> x >> y >> color >> keyCount) {
				Door door(x, y, id, static_cast<Color>(color));

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
					
					while (parser.peek() == ' ' || parser.peek() == ']')
						parser.ignore();

					for (int i = 0; i < switchCount; i++) {
						int switchID, switchState;
						if (parser >> switchID >> switchState) {
							Switch* sw = room.getSwitchByID(switchID);
							if (sw) {
								door.addRequiredSwitch(sw, (bool)switchState);
							} 
							else {
								std::cerr << "Warning - door " << id << "missing switch " << switchID << std::endl;
							}
						}
					}
					while (parser.peek() == ' ' || parser.peek() == ']')
						parser.ignore();
				}
				door.UpdatedFromSwitch();
				room.addDoor(door);
			}
		}

		else if (section == "OBSTACLES") {
			int partCount;
			if (parser >> partCount) {
				Obstacle obstacle;

				for (int i = 0; i < partCount; i++) {
					while (parser.peek() == ' ' || parser.peek() == '[')
						parser.ignore();

					int px, py;

					if (parser >> px >> py)
						obstacle.addPart(Placement(px, py, OBSTACLE_TILE));
					
					while (parser.peek() == ' ' || parser.peek() == ']')
						parser.ignore();
				}
				room.addObstacle(obstacle);
			}
		}

		else if (section == "TORCHES") {
			int x, y, radius;
			if (parser >> x >> y >> radius) {
				Torch torch(x, y, radius);
				room.addTorch(torch);
			}
		}

		else if (section == "BOMBS") {
			int id, x, y, timer;
			
			if (parser >> id >> x >> y >> timer) {
				Bomb bomb(x, y, id, timer);
				room.addBomb(bomb);
			}
		}

		else if (section == "POTIONS") {
			int x, y;
			if (parser >> x >> y) {
				Potion potion(x, y);
				room.addPotion(potion);
			}
		}

		else if (section == "SPRINGS") {
			int dirx, diry, count;
			
			if (parser >> dirx >> diry >> count) {
				Spring spring({ dirx,diry });
				for (int i = 0; i < count; i++) {
					int px, py;
					if (parser >> px >> py)
						spring.addPart(px, py);
				}
				room.addSpring(spring);
			}
		}

		else if (section == "RIDDLES") {
			int id, x, y;
			if (parser >> id >> x >> y) {
				room.addRiddle(x, y, id);
			}
		}
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
			std::string question;
			if (std::getline(parser, question)) {
				int first = question.find
			}
		}
	}
	
}