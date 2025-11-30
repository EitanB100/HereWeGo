#pragma once
<<<<<<< Updated upstream
#include "Utils.h"
#include "Placement.h"

// Door class representing a door in the game
class Door {
	Placement pos;
	int DestinationID;
	int requiredKeyID;
	bool isOpen;
	
public:

	Door() : pos(0, 0, 'D'), DestinationID(-1), requiredKeyID(-1), isOpen(false) {} // Default constructor for array initialization
	

	Door(int doorNum, int _x, int _y, int destID, int keyID) // Constructor to initialize door properties
		: pos(_x, _y, doorNum + '1'), DestinationID(destID), requiredKeyID(keyID), isOpen(false) {}


	int getDestinationID() const { 
		return DestinationID;
	}

	int getRequiredKeyID() const {
			return requiredKeyID;
	}

	Placement getPosition() const {
		return pos;
	}

	bool getIsOpen() const { 
			return isOpen;
		}

	void openDoor(bool open) {
			isOpen = open;
		}
	
	void draw() { // Draw the door on the screen based on its state
		pos.draw(isOpen ? ' ' : pos.getTileChar());
	}

	bool isAtPosition(const Point& p) const {
		return (pos.getx() == p.x && pos.gety() == p.y);
	}
=======
#include <vector>
#include "Utils.h"
#include "Placement.h"
#include "Switch.h"

// Door class representing a door in the game
class Door {

    struct SwitchRequirement {
        Switch* SW;
        bool requiredState;   // false = OFF, true = ON
    };

    int id;
    Placement pos;
    std::vector<int> requiredKeyIDs;
    std::vector<SwitchRequirement> requiredSwitchIDs;
	Color color = Color::WHITE;
    bool isOpen = false;
	bool keysSatisfied = false; //all keys are used to open the door

public:

   Door(int x, int y, int _id, Color c) : pos(x, y, _id + '0'), id(_id), color(c) {}
   
   void addRequiredKey(int id) { // doubleling dynmaic size
       requiredKeyIDs.push_back(id);
   }

   void addRequiredSwitch(Switch* switchOnOff, bool state) {
       requiredSwitchIDs.push_back({ switchOnOff, state });

   }

   int getDoorID() { return id; }
   bool setKeySatisfied(bool val) { return keysSatisfied = val; }
   void open() { isOpen = true; }
   void close() { isOpen = false; }
   
   bool getIsOpen() const { return isOpen; }

   Placement getPos() { return pos; }

   void draw();

   bool tryUnlock(int keyID);
   void UpdatedFromSwitch();


>>>>>>> Stashed changes
};