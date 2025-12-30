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
#include "spring.h"
#include "Bomb.h"


class Player; // forward decloraion

class Room {

    char map[MAX_Y][MAX_X] = {}; // leave a room for HUD at top row
    std::vector<Door> doors;
    std::vector<Key> keys;
    std::vector<Switch*> switches;
    std::vector<Obstacle> obstacles;
    std::vector<Torch> torches;
    std::vector<Spring> springs;
	std::vector<Bomb> bombs;


public:

    Room();
    ~Room() {
        for (Switch* sw : switches) {
            delete sw;
        }
        switches.clear();
    }
    const char (*getMap() const)[MAX_X] {
        return map;
    }
    bool checkDoor(Point p, heldItem& item);
    void checkSwitch(Point p); // 3. Added Switch Check

    void drawTopLayer();
    void drawRoom(Screen& screen);
    void loadFromScreen(Screen& screen);
    void clearTile(Point& p) { map[p.y][p.x] = ' '; }

    void addWall(const Point& p);
    void addDoor(const Door& door);
    void addKey(const Key& key);
    void addTorch(const Torch& torch);
    void addSpring(const Spring& spring);
	void addBomb(const Bomb& bomb);
    void addSwitch(Switch* s);
    void addObstacle(const Obstacle& obs);

    void removeKey(const Point& p);
    void removeTorch(const Point& p);
    void removeObstacle(const Point& p);
	void removeSpring(const Point& p);
	void removeSwitch(const Point& p);
	void removeBomb(const Point& p);


    char getObjectAt(Point& p);
    char getObjectAt(Point& p, Color& color);
  
    bool isWallThere(Point p);
	Switch* getSwitchByID(int id);

    Door* isDoorThere(Point p);
    Key* isKeyThere(Point p);
    Switch* isSwitchThere(Point p);
    Torch* isTorchThere(Point p);
    Obstacle* isObstacleThere(Point p);
    Spring* isSpringThere(Point p);
	Bomb* isBombThere(Point p);

    
	bool PointhasLineOfSight(int TorchPointX, int TorchPointY , int pointX , int PointY);
	void CompleteLineOfSight(Torch torch);
	void getTorchesLineOfSight();


    bool moveObstacle(Point p, int dirx, int diry, int force);
    void resetObstacles() {
        for (auto& o : obstacles) {
            o.resetMove();
        }
    }

	void bombExplode(Bomb* bomb, Player* players, int playerCount , Screen& screen);
    void updateBombs(Player* players, int playerCount, Screen& screen);
    void clearExplosions();
    bool hasExplosions();

    
   //bool moveObstacle(Obstacle* obstacle, int dirx, int diry, int playerForce);
};
