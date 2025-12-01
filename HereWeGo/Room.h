#pragma once
#include <vector>
#include "Screen.h"
#include "Placement.h"
#include "Tile_Chars.h"
#include "Door.h"
#include "Key.h"
#include "Switch.h" // 1. Added Switch Header
#include "Obstacle.h"


class Room {
        
    char map[MAX_Y][MAX_X] = {};
    std::vector<Door> doors;
    std::vector<Key> keys;
    std::vector<Switch> switches; 
    std::vector<Obstacle> obstacles;
    

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
    void checkSwitch(Point p); // 3. Added Switch Check

    void drawTopLayer();
    void drawRoom(Screen& screen);
    void clearTile(Point& p) { map[p.y][p.x] = ' '; }

    void addWall(Point p);
    void addDoor(Door door);
    void addKey(Key key);
    void addSwitch(const Switch& s); // 4. Added Switch Adder
    void addObstacle(Obstacle obs);

    char getObjectAt(Point& p);

    Door* isDoorThere(Point& p);
    Key* isKeyThere(Point& p);
    Switch* isSwitchThere(Point& p); // 5. Added Switch Getter

    
    bool moveObstacle(Point p, int dirx, int diry, int force);

    
   //bool moveObstacle(Obstacle* obstacle, int dirx, int diry, int playerForce);
};