#pragma once
#include "CommandKeys.h"
#include "Placement.h"
#include "Screen.h"
#include "Room.h"

class Player {
    Placement pos = Placement();
    char symbol = ' ';
    int dirx = 0, diry = 0;
    int force = 1;
    bool finishedLevel = false;
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

    // Helpers
    void synchronizePartner(Player* otherPlayer, Room& room);
    bool handleSprings(Room& room, Point nextPoint); 
    bool handlePickups(Room& room, Point nextPoint);
    bool handleSpringExit(Room& room);
public:
    Player(const Placement& p, char c, int directx, int directy, const char keyArray[keyAmount]);

    static constexpr int MAX_HP = 15;

    void draw();

    // Getters / Setters
    Point getPos() const { return { pos.getx(),pos.gety() }; }
    void setPos(Point p) { pos.set(p.x, p.y, symbol); }
    char getSymbol() { return symbol; }
    bool isFinished() { return finishedLevel; }
    void setFinished(bool state) { finishedLevel = state; }
    heldItem getItemInHand() { return itemInHand; }
    void setDirection(int dx, int dy) { dirx = dx; diry = dy; }

    int getHP() const { return hp; }
    void takeDamage(int amount) {
        hp -= amount;
        if (hp <= 0) {
            hp = 0;
            alive = false; // Mark as dead
        }
    }
    bool isDead() const { return !alive; }
    bool isAlive() const { return alive; }

    // Core Logic
    void move(Room& room, Player* otherPlayer);
    bool obstacleHandling(Room& room, Point& nextPoint, Player* otherPlayer);
    void updateSpringPhysics(Room& room, Player* otherPlayer);
    void dropItem(Room& room);

    bool isCommand(char input, CommandKeys key) const {
        int index = static_cast<int>(key);
        return toupper(input) == keys[index];
    }
    void inputManager(char tav, Room& room);

};