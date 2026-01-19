#pragma once
#include "CommandKeys.h"
#include "Placement.h"
#include "Screen.h"
#include "Room.h"
#include "Utils.h"

class Player {
    Placement pos = Placement();
    char symbol = ' ';
    Directions direction;
    Color playerColor = Color::WHITE;
    int dirx = 0, diry = 0;
    int force = 1;
    bool finishedLevel = false;
    bool gotHit = false;
    static constexpr int keyAmount = static_cast<int>(CommandKeys::NUM_KEYS);
   

    char keys[keyAmount];
    heldItem itemInHand = { ItemType::NONE, 0, Color::WHITE };

    struct SpringState {
        int compressionCount = 0;
        int flightTime = 0;
        int force = 1;
        Point launchDir = { 0,0 };
    } spring;

	int hp = MAX_HP;
    bool alive = true;
    bool isSilent = false;

    // Helpers
    void setSilent(bool s) { isSilent = s; }
    void synchronizePartner(Player* otherPlayer, Room& room);
    bool handleSprings(Room& room, Point nextPoint); 
    bool handlePickups(Room& room, Point nextPoint);
    bool handleSpringExit(Room& room);
public:
    Player(const Placement& p, char c, int directx, int directy, const char keyArray[keyAmount]);
    Player() : pos(Placement()), symbol(' '), dirx(0), diry(0) {
        for (int i = 0; i < keyAmount; ++i) keys[i] = 0;
    }
    
    static constexpr int MAX_HP = 15;
    static constexpr int DEAD_HP = 0;
    void draw();


    // Getters / Setters
    Point getPos() const { return { pos.getx(),pos.gety() }; }
    heldItem getItemInHand() { return itemInHand; }
    void setItemInHand(heldItem newItem) { itemInHand = newItem; }
    char getSymbol() { return symbol; }
    bool isFinished() { return finishedLevel; }
    int getDirX() { return dirx; }
    int getDirY() { return diry; }
    bool hasGotHit() { return gotHit; }
    void resetGotHit() { gotHit = false; }

   

    void setPos(Point p) { pos.set(p.x, p.y, symbol); }
    void setFinished(bool state) { finishedLevel = state; setDirection(Directions::STAY); }
    void setDirection(const Point& direction) { dirx = direction.x; diry = direction.y; }

    void setForce(int _force) { force = _force; }
    int getForce() const { return force; }
    void transferMomentum(int _force, Point dir, int time);

    int getHP() const { return hp; }
    void setHP(int amount);
    void takeDamage(int amount);
    bool increaseHP(int amount);
       
    bool isDead() const { return !alive; }
    bool isAlive() const { return alive; }

    // Core Logic
    // Core Logic
    int move(Room& room, Player* otherPlayer);
    bool obstacleHandling(Room& room, Point& nextPoint, Player* otherPlayer);
    void updateSpringPhysics(Room& room, Player* otherPlayer);
    void dropItem(Room& room);

    bool isCommand(char input, CommandKeys key) const {
        int index = static_cast<int>(key);
        return toupper(input) == keys[index];
    }
    void inputManager(char input, Room& room);

};