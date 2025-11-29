#pragma once
#include <vector>
#include "Screen.h"
#include "Placement.h"
#include "Tile_Chars.h"
#include "Door.h"
#include "Key.h"


class Obstacle;

class Room {
        
    char map[MAX_Y][MAX_X] = {};
    std::vector<Door> doors;
    std::vector<Key> keys;
    //obstacles vector later

public:

    Room() {} // array initialized to zeroes, vectors self initialize

    bool checkDoor(Point p, heldItem& item);

    void drawRoom(Screen& screen);

    void addWall(Point p);
    void addDoor(Door door);
    void addKey(Key key);

    char getObjectAt(Point& p) const;

    Door* isDoorThere(Point& p);
    Key* isKeyThere(Point& p);
};