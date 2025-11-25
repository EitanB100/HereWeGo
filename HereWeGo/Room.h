#pragma once
#include "Screen.h"
#include "Placement.h"

// Forward declarations
class Door; 
class Key;
class Wall;
class Obstacle;

class Room {
    
    Placement pos = Placement();

	char symbol = 'W'; // Wall symbol
	int height = MAX_Y, width = MAX_X; // Room dimensions

	Door* doors = nullptr;
	int numDoors = 0;
	Key* keys = nullptr;
	int numKeys = 0;
	Wall* walls = nullptr;
	int numWalls = 0;
	Obstacle* obstacles = nullptr;
	int numObstacles = 0;

public:

	Room() {} // Default constructor

	Room(const Placement& p, char c, const int h, const int w) { // Constructor to set up room walls
        symbol = c;
        pos.set(p.getx(), p.gety(), symbol);
        height = h;
        width = w;
    }
    int getHeight() {
        return height;
    }
    int getWidth() {
        return width;
    }

    bool inBounds(const Point& p) const {
    int startX = pos.getx();
    int startY = pos.gety();

    return (p.x > startX && p.x < startX + width - 1 &&
            p.y > startY && p.y < startY + height - 1);

	}

    void drawDoors(Screen& screen);

    void drawKeys(Screen& screen);

    char getObjectAt(Point& p) const;

    void initializeArrays(int doorCount, int keyCount, int wallCount, int obstacleCount); // Initialize doors and keys arrays
    
    void drawRoom(Screen& screen);

    void drawPerimiterWalls(Screen& screen);

    void drawInternalWalls(Screen& screen);

    void addDoor(int index, const Door& door);

    void addKey(int index, const Key& key);

	void addWall(int index, const Wall& wall);

    void addObstacle(int index, int x, int y);


    Door* isDoorThere(Point& p) const;
	bool checkDoor(Point p, const heldItem& item);

    Key* isKeyThere(Point& p) const;

    Wall* isWallThere(Point& p) const;

	Obstacle* isObstacleThere(Point& p) const;

    bool moveObstacle(Obstacle* obstacle, int dirx, int diry, int playerForce);
};