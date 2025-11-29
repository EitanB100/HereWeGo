#pragma once
#include "Utils.h"
#include "Placement.h"

// Door class representing a door in the game
class Door {
    int requiredKeyID;
    int nextRoomID; // For later exercises
public:

    Door();
    Door(int keyID, int nextRoom) : requiredKeyID(keyID), nextRoomID(nextRoom) {}

    int getRequiredKeyID() const { return requiredKeyID; }
};