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

    Room() {
        for (int y = 0; y < MAX_Y; y++)
        {
            for (int x = 0; x < MAX_X; x++)
            {
                map[y][x] = ' ';
            }
        }
    } // array initialized to zeroes, vectors self initialize

    bool checkDoor(Point p, heldItem& item);

    void drawTopLayer();
    void drawRoom(Screen& screen);
    void clearTile(Point p) { map[p.y][p.x] = ' '; }

    void addWall(Point p);
    void addDoor(Door door);
    void addKey(Key key);

    char getObjectAt(Point& p);

    Door* isDoorThere(Point& p);
    Key* isKeyThere(Point& p);
};