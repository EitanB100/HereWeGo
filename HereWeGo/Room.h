#pragma once
#include <vector>
#include <iostream>
#include "Screen.h"
#include "Placement.h"
#include "Tile_Chars.h"
#include "Door.h"
#include "Key.h"
#include "Torch.h"
#include "Switch.h" 
#include "Obstacle.h"


class Room {
        
    char map[MAX_Y][MAX_X] = {}; // leave a room for HUD at top row
    std::vector<Door> doors;
    std::vector<Key> keys;
    std::vector<Switch> switches; 
    std::vector<Obstacle> obstacles;
    std::vector<Torch> torches;


public:

    Room();

    bool checkDoor(Point p, heldItem& item);
    void checkSwitch(Point p); // 3. Added Switch Check

    void drawTopLayer();
    void drawRoom(Screen& screen);
    void clearTile(Point& p) { map[p.y][p.x] = ' '; }

    void addWall(Point p);
    void addDoor(Door door);
    void addKey(Key key);
    void addTorch(Torch torch);

    void removeKey(const Point& p);
    void removeTorch(const Point& p);
    void removeObstacle(const Point& p);

    void addSwitch(const Switch& s); // 4. Added Switch Adder
    void addObstacle(Obstacle obs);

    char getObjectAt(Point& p);
    char getObjectAt(Point& p, Color& color);
  
    bool isWallThere(Point p);
    

    Door* isDoorThere(Point p);
    Key* isKeyThere(Point p);
    Switch* isSwitchThere(Point p); // 5. Added Switch Getter
    Torch* isTorchThere(Point p);
    Obstacle* isObstacleThere(Point p);
    
    
	bool PointhasLineOfSight(int TorchPointX, int TorchPointY , int pointX , int PointY);
	void CompleteLineOfSight(Torch torch);
	void getTorchesLineOfSight();


    bool moveObstacle(Point p, int dirx, int diry, int force);
    void resetObstacles() {
        for (auto& o : obstacles) {
            o.resetMove();
        }
    }
    
   //bool moveObstacle(Obstacle* obstacle, int dirx, int diry, int playerForce);
};
