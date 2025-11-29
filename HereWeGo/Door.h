#pragma once
#include <vector>
#include "Utils.h"
#include "Placement.h"

// Door class representing a door in the game
class Door {
    
    Placement pos;
    int id;
    std::vector<int> requiredKeyIDs;
	Color color = Color::WHITE;
    bool isOpen = false;

public:

   Door(int x, int y, int _id, Color c) : pos(x, y, id + '1'), id(_id), color(c) {}
   
   void addRequiredKey(int id) {
       requiredKeyIDs.push_back(id);
   }

   int getDoorID() { return id; }
   void open() { isOpen = true; }
   
   bool getIsOpen() const { return isOpen; }

   Point getPos() { return pos.getPosition(); }

   void draw();

   bool tryUnlock(int keyID);
   
   
    
   
};