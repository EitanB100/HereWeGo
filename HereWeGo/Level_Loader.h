#pragma once
#include <string>
#include "Room.h"

class Level_Loader {
public:

	static void loadLevel(Room& room, const std::string& fileName);
};