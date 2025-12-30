#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include "Placement.h"
#include "Tile_Chars.h"
#include "Door.h"
#include "Key.h"
#include "Torch.h"
#include "Obstacle.h"
#include "spring.h"
#include "Bomb.h"


class Player; // forward declaration
class Switch;
class Screen;

class Room {

    char map[MAX_Y][MAX_X] = {}; // leave a room for HUD at top row
    std::vector<Door> doors;
    std::vector<Key> keys;
    std::vector<std::unique_ptr<Switch>> switches;
    std::vector<Obstacle> obstacles;
    std::vector<Torch> torches;
    std::vector<Spring> springs;
	std::vector<Bomb> bombs;


public:

    Room();
    Room(const Room&) = delete;
    Room& operator=(const Room&) = delete;
    ~Room() = default;

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
    void addSwitch(std::unique_ptr<Switch> s);
    void addObstacle(const Obstacle& obs);

    void removeKey(const Point& p);
    void removeTorch(const Point& p);
    void removeObstacle(const Point& p);
	void removeSpring(const Point& p);
	void removeSwitch(const Point& p);
	void removeBomb(const Point& p);


    char getObjectAt(const Point& p) const;
    char getObjectAt(const Point& p, Color& color) const;
  
    bool isWallThere(Point p);
	Switch* getSwitchByID(int id);

    const Door* isDoorThere(const Point& p) const;
    const Key* isKeyThere(const Point& p) const;
    const Switch* isSwitchThere(const Point& p) const;
    const Torch* isTorchThere(const Point& p) const;
    const Obstacle* isObstacleThere(const Point& p) const;
    const Spring* isSpringThere(const Point& p) const;
	const Bomb* isBombThere(const Point& p) const;

    Door* isDoorThere(const Point& p);
    Key* isKeyThere(const Point& p);
    Switch* isSwitchThere(const Point& p);
    Torch* isTorchThere(const Point& p);
    Obstacle* isObstacleThere(const Point& p);
    Spring* isSpringThere(const Point& p);
    Bomb* isBombThere(const Point& p);
    
	bool PointhasLineOfSight(int TorchPointX, int TorchPointY , int pointX , int PointY);
	void CompleteLineOfSight(const Torch& torch);
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

};
