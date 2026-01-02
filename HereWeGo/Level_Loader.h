#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Room.h"
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
	static void loadLevel(Room& room, const std::string& fileName);
	static void loadRiddles(const std::string& fileName, std::vector<Riddle>& outRiddles);
};