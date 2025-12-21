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

    heldItem itemInHand = { NONE, 0, Color::WHITE};
    
    struct SpringState {
        int compressionCount = 0; // How deep we stepped in
        int flightTime = 0;       // How long we fly
        int force = 1;            // Speed
        Point launchDir = { 0,0 };  // Direction locked during flight
    } spring;

    //Suggested and written by gemini!
    // added as a fix to the following bug - when player 1 moved first and pushed an obstacle, 
    // player 2 got left behind one tile despite applying force on the obstacle as well
    // also when player 2 disposed an item while moving an obstacle, it turned invisible (yet still interactable)
    // this function takes care of both bugs!
 
    void synchronizePartner(Player* otherPlayer, Room& room);

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


    void draw() {
        if (itemInHand.type != NONE)
            setColor(itemInHand.color);
        pos.draw();
        setColor(Color::WHITE);
    }

    Point getPos() const {
        return { pos.getx(),pos.gety() };
    }
    
    void setPos(Point p) {
        pos.set(p.x, p.y, symbol);
    }

    int getdirctX() {
        return dirx;
    }

    int getdirctY() {
        return diry;
    }

    char getSymbol() { return symbol; }

    bool isFinished() { return finishedLevel; }
    void setFinished(bool state) { finishedLevel = state; }

    heldItem getItemInHand() {
        return itemInHand;
    }

    void setDirection(int dx, int dy) {
        dirx = dx;
        diry = dy;
    }


    void move(Room& room, Player* otherPlayer);
    bool obstacleHandling(Room& room, Point& nextPoint, Player* otherPlayer);

    void updateSpringPhysics(Room& room, Player* otherPlayer);
    

    bool pickItem(Point& position,Room& room);
    void dropItem(Room& room);

    void inputManager(char tav, Room& room);
};
