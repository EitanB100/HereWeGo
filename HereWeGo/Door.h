#pragma once
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
};