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

    // Added 'tileChar = tile;'
    // Previously, this function updated X and Y but ignored the 'tile' char.
    // This caused the Player to turn invisible (become ' ') as soon as they moved.
    void set(int _x, int _y, char tile = ' ') {
        x = _x;
        y = _y;
        tileChar = tile;
 
        inLimit();
    }

    void setx(int _x) {
        x = _x;
        inLimit();
	}

    void sety(int _y) {
        y = _y;
        inLimit();
	}   

    void move(int xSteps, int ySteps, char tile) {
        set(x + xSteps, y + ySteps, tile);
    }

    void setTileChar(char tile) {
        tileChar = tile;
    }
    int getx() const {
        return x;
    }
    int gety() const {
        return y;
    }
    Point getPosition() const {
        return { x,y };
    }

    char getTileChar() const {
        return tileChar;
    }

    void draw() {
        draw(tileChar);
    }
    void draw(char c);


private:
    void inLimit();
};




