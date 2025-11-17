#pragma once
#include "Screen.h"
#include "Placement.h"
class Room {
    Placement pos = Placement();
    char symbol = 'W';
    int height = 1, width = 1;
    //Doors*
    //bool isLit
    friend Screen;
public:
    Room(Placement p, char c, int h, int w) {
        symbol = c;
        pos.set(p.getx(), p.gety(), symbol);
        height = h;
        width = w;
    }
    int getHeight() {
        return height;
    }
    int getWidth() {
        return width;
    }
    //void MakeWalls();
    void drawRoom(Screen& screen);

};