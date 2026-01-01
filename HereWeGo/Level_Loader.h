#pragma once
#include <string>
#include "Room.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Switch.h"
#include "Door.h"
#include "Key.h"
#include "Obstacle.h"
#include "Bomb.h"  
#include "Potion.h"
#include "Torch.h"
#include "Riddle.h"

class Level_Loader {
public:
	void collectConnectedParts(Room& room, int x, int y, char targetCh, std::vector<Point>& outParts, std::vector<std::vector<bool>>& visited);
	static void loadLevel(Room& room, const std::string& fileName);
	static void loadRiddles(const std::string& fileName, std::vector<Riddle>& outRiddles);
};