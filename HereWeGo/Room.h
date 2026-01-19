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
#include "Potion.h"
#include "Riddle.h"


class Player; // forward declaration
class Switch;
class Screen;

class Room {
    friend class Level_Loader;
    
    struct RiddlePos {
        Point p;
        int id;
    };

    char map[MAX_Y][MAX_X] = {}; // leave a room for HUD at top row
    std::vector<Door> doors;
    std::vector<Key> keys;
    std::vector<std::unique_ptr<Switch>> switches;
    std::vector<Obstacle> obstacles;
    std::vector<Torch> torches;
    std::vector<Spring> springs;
	std::vector<Bomb> bombs;
    std::vector<Potion> potions;
    std::vector<RiddlePos> riddleLocations;

    template <typename T, typename Predicate>
    T* findBy(std::vector<T>& obj, Predicate pred) {
        for (auto& item : obj) {
            if (pred(item)) return &item;
        }
        return nullptr;
    }

    template <typename T, typename Predicate>
    const T* findBy(const std::vector<T>& obj, Predicate pred) const {
        for (const auto& item : obj) {
            if (pred(item)) return &item;
        }
        return nullptr;
    }

    Point p1Start = { 1,1 };
    Point p2Start = { 2,1 };
    Point exitPos = { -1,-1 };
    Point legendLocation = { 0,0 };
public:

    Room();
    //changed switches to be unique_ptrs, but just in case did those anyway
    Room(const Room&) = delete;
    Room& operator=(const Room&) = delete;

    Room(Room&&) = default;
    ~Room() = default;

    const char (*getMap() const)[MAX_X] {
        return map;
    }

    Point getLegendLoc() const { return legendLocation; }

    void resetRoom();

    bool checkDoor(Point p, heldItem& item, bool isSilent);
    void checkSwitch(Point p); // 3. Added Switch Check

    Point getP1Start() const { return p1Start; }
    void setP1Start(const Point& p) { p1Start = p; }
    Point getP2Start() const { return p2Start; }
    void setP2Start(const Point& p) { p2Start = p; }
    
    Point getExitPos() const { return exitPos; }
    void setExitPos(const Point& p) { exitPos = p; }

    void drawTopLayer();
    void drawRoom(Screen& screen);
    void clearTile(Point& p) { map[p.y][p.x] = ' '; }

    void addDoor(const Door& door);
    void addKey(const Key& key);
    void addTorch(const Torch& torch);
    void addSpring(const Spring& spring);
	void addBomb(const Bomb& bomb);
    void addSwitch(std::unique_ptr<Switch> s);
    void addObstacle(const Obstacle& obs);
    void addPotion(const Potion& potion);
    void addRiddle(int x, int y, int id);

    void removeKey(const Point& p);
    void removeTorch(const Point& p);
    void removeObstacle(const Point& p);
	void removeSpring(const Point& p);
	void removeSwitch(const Point& p);
	void removeBomb(const Point& p);
    void removePotion(const Point& p);
    void removeRiddle(const Point& p);

    char getObjectAt(const Point& p) const;
    char getObjectAt(const Point& p, Color& color) const;
    
    Point getRiddlePos(int id) const;
  
    bool isWallThere(Point p);
	Switch* getSwitchByID(int id);

    const Door* isDoorThere(const Point& p) const;
    const Key* isKeyThere(const Point& p) const;
    const Switch* isSwitchThere(const Point& p) const;
    const Torch* isTorchThere(const Point& p) const;
    const Obstacle* isObstacleThere(const Point& p) const;
    const Spring* isSpringThere(const Point& p) const;
	const Bomb* isBombThere(const Point& p) const;
    const Potion* isPotionThere(const Point& p) const;
    const RiddlePos* isRiddleThere(const Point& p) const;

    Door* isDoorThere(const Point& p);
    Key* isKeyThere(const Point& p);
    Switch* isSwitchThere(const Point& p);
    Torch* isTorchThere(const Point& p);
    Obstacle* isObstacleThere(const Point& p);
    Spring* isSpringThere(const Point& p);
    Bomb* isBombThere(const Point& p);
    Potion* isPotionThere(const Point& p);
    RiddlePos* isRiddleThere(const Point& p);
    
	bool PointhasLineOfSight(int TorchPointX, int TorchPointY , int pointX , int PointY);
	void CompleteLineOfSight(const Torch& torch);
	void getTorchesLineOfSight();


    bool moveObstacle(Point p, int dirx, int diry, int force, bool isSilent);
    void resetObstacles() {
        for (auto& o : obstacles) {
            o.resetMove();
        }
    }

	void bombExplode(Bomb* bomb, Player* players, int playerCount , Screen& screen);
    void updateBombs(Player* players, int playerCount, Screen& screen, bool isSilent);
    void clearExplosions();
    bool hasExplosions();

    int getRiddleID(const Point& p) const;
};
