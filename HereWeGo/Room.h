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
        // Iterate through the entire map grid
        for (int y = 0; y < MAX_Y; y++)
        {
            for (int x = 0; x < MAX_X; x++)
            {
                // Initialize Map Data
                // I set the boundaries to WALL_TILE and the inside to Empty Space.
                // PREVIOUS BUG: The map was initialized to all ' ', so the walls
                // existed in the physics check but were invisible on screen.
                if (y == 0 || y == MAX_Y - 1 || x == 0 || x == MAX_X - 1)
                    map[y][x] = WALL_TILE;
                else
                    map[y][x] = ' ';
            }
        }
    }

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