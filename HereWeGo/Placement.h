#pragma once

#include <utility>
#include "Utils.h"

class Placement {
    int pos[2] = { 0,0 };        //pos[0] = x, pos[1] = ya
    char tileChar = ' ';




public:


    Placement() {}
    Placement(int x, int y, char tile = ' ') {
        pos[0] = x;
        pos[1] = y;
        tileChar = tile;
        inLimit();
    }

    void set(int x, int y, char tile = ' ') {
        pos[0] = x;
        pos[1] = y;
        inLimit();
    }
    void move(int x, int y, char tile) {
        set(pos[0] + x, pos[1] + y, tile);
    }

    void setTileChar(char tile) {
        tileChar = tile;
    }
    int getx() {
        return pos[0];
    }
    int gety() {
        return pos[1];
    }
    int* getPos() {
        return pos;
    }

    char getTileChar(const int x, const int y) {
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




