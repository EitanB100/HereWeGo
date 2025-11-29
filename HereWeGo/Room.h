#pragma once
#include "Screen.h"
#include "Placement.h"
#include "Tile_Chars.h"

// Forward declarations
class Door; 
class Key;
class Wall;
class Obstacle;

class Room {
    

	char symbol = 'W'; // Wall symbol
    
    char map[25][80] = {};
    
    Door doors[MAX_DOORS];
    int numDoors = 0;

public:

    

    bool checkDoor(Point p, heldItem& item);

    void setupPerimiter();

    void addWall(int x, int y);

    void addDoor(int x, int y, int id);

    void drawDoors(Screen& screen);

    void drawKeys(Screen& screen);

    char getObjectAt(Point& p) const;

    void initializeArrays(int doorCount, int keyCount, int wallCount, int obstacleCount); // Initialize doors and keys arrays
    
    void drawRoom(Screen& screen);

    


    void addDoor(int x,int y, int doorNumber);

    void addKey(int index, const Key& key);

	void addWall(int index, const Wall& wall);

    void addObstacle(int index, int x, int y);


    Door* isDoorThere(Point& p) const;

    Key* isKeyThere(Point& p) const;

	Obstacle* isObstacleThere(Point& p) const;

    bool moveObstacle(Obstacle* obstacle, int dirx, int diry, int playerForce);
};