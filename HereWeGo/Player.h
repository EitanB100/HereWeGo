#pragma once
#include "CommandKeys.h"
#include "Placement.h"

class Player {
    Placement pos = Placement();
    char symbol = ' ';
    int dirx = 0, diry = 0;
    char keys[NUM_KEYS];            // fixed-size array

public:
    Player(Placement p, char c, int directx, int directy, const char keyArray[NUM_KEYS]) {
        symbol = c;
        pos.set(p.getx(), p.gety(), symbol);
        dirx = directx;
        diry = directy;
        for (int i = 0; i < NUM_KEYS; ++i)
            keys[i] = keyArray[i];
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
    void move();
    void changeDirection();
};
