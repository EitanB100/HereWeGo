#pragma once
#include <vector>
#include "Utils.h"
#include "Placement.h"

// Door class representing a door in the game
class Door {
    
    Placement pos;
    std::vector<int> requiredKeyIDs;
	Color color = Color::WHITE;
    bool isOpen = false;

public:

   Door(int x, int y, Color c) : pos(x, y, 'D'), color(c) {}
   
   void addRequiredKey(int id) {
       requiredKeyIDs.push_back(id);
   }

   void open() { isOpen = true; }
   
   bool getIsOpen() const { return isOpen; }

   void draw();

   bool tryUnlock(int keyID);
   
   
    
   
};