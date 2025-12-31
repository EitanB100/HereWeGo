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

class Level_Loader {
public:

	static void loadLevel(Room& room, const std::string& fileName);
};