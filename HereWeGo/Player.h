#pragma once
#include "CommandKeys.h"
#include "Placement.h"
#include "Screen.h"
#include "Room.h"

class Player {
    Placement pos = Placement();

    char symbol = ' ';
    int dirx = 0, diry = 0;
    int force = 1; // for moving obstacles, will be changed by spring later

    char keys[NUM_KEYS];            // fixed-size array

     //Color will be used as an indicator for a picked up item, mainly key
	heldItem itemInHand = { NONE, 0, Color::WHITE};

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

    
    int getdirctX() {
        return dirx;
    }

    int getdirctY() {
        return diry;
    }

    void setDirection(int dx, int dy) {
        dirx = dx;
        diry = dy;
    }
    

    void move(Room& room);
    void pickItem(Point& position,Room& room, char symbol);
    void changeDirection(char tav);
};
