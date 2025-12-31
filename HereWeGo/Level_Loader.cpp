#include "Level_Loader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Switch.h"
#include "Door.h"
#include "Key.h"
#include "Obstacle.h"

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

		std::stringstream ss(line);
	}
}
