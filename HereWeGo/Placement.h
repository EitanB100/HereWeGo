#pragma once

#include <utility>
#include "Utils.h"


class Placement {
	int x = 0, y = 0; //position coordinates
    char tileChar = ' ';


public:


    Placement() {}
    Placement(int _x, int _y, char tile = ' ') {
        x = _x;
        y = _y;
        tileChar = tile;
        inLimit();
    }

    void set(int _x, int _y, char tile = ' ') {
        x = _x;
        y = _y;
        inLimit();
    }
    void move(int xSteps, int ySteps, char tile) {
        set(x + xSteps, y + ySteps, tile);
    }

    void setTileChar(char tile) {
        tileChar = tile;
    }
    int getx() const{
        return x;
    }
    int gety() const{
        return y;
    }
    Point getPosition() const {
        return { x,y };
    }

    char getTileChar() {
        return tileChar;
    }

    void draw() {
        draw(tileChar);
    }
    void draw(char c);

    /* bool onBlock() { //next is where the player is heading
         int x = getx();
         int y = gety();

        if (x < 0 || y < 0 || x > MAX_X || y > MAX_Y)
            return false; // out of bounds means not blocked (or could be true if you prefer)

        if (isBlockingTile(getTileChar(x, y))) {
            return true;
        }
        return false;
    }
      */

private:
    void inLimit();

    static bool isBlockingTile(char t) {
        return (t == 'W' || t == 'w');
    }

};




