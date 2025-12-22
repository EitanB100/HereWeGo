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
    char keys[NUM_KEYS];
    heldItem itemInHand = { NONE, 0, Color::WHITE };

    struct SpringState {
        int compressionCount = 0;
        int flightTime = 0;
        int force = 1;
        Point launchDir = { 0,0 };
    } spring;

    // Helpers
    void synchronizePartner(Player* otherPlayer, Room& room);
    bool handleSprings(Room& room, Point nextPoint); 
    bool handlePickups(Room& room, Point nextPoint);
    bool handleSpringExit(Room& room);
public:
    Player(const Placement& p, char c, int directx, int directy, const char keyArray[NUM_KEYS])
    {
        symbol = c;
        pos.set(p.getx(), p.gety(), symbol);
        dirx = directx;
        diry = directy;
        for (int i = 0; i < NUM_KEYS; ++i)
            keys[i] = keyArray[i];
    }

    void draw();

    // Getters / Setters
    Point getPos() const { return { pos.getx(),pos.gety() }; }
    void setPos(Point p) { pos.set(p.x, p.y, symbol); }
    char getSymbol() { return symbol; }
    bool isFinished() { return finishedLevel; }
    void setFinished(bool state) { finishedLevel = state; }
    heldItem getItemInHand() { return itemInHand; }
    void setDirection(int dx, int dy) { dirx = dx; diry = dy; }

    // Core Logic
    void move(Room& room, Player* otherPlayer);
    bool obstacleHandling(Room& room, Point& nextPoint, Player* otherPlayer);
    void updateSpringPhysics(Room& room, Player* otherPlayer);
    void dropItem(Room& room);
    void inputManager(char tav, Room& room);
};