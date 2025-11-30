#pragma once

#include <utility>
#include "Utils.h"


class Placement {
<<<<<<< Updated upstream
	int x = 0, y = 0; //position coordinates
=======
    int x = 0, y = 0; //position coordinates
>>>>>>> Stashed changes
    char tileChar = ' ';


public:


    Placement() {}
    Placement(int _x, int _y, char tile = ' ') {
        x = _x;
        y = _y;
        tileChar = tile;
        inLimit();
    }

<<<<<<< Updated upstream
    void set(int _x, int _y, char tile = ' ') {
        x = _x;
        y = _y;
=======
    // Added 'tileChar = tile;'
    // Previously, this function updated X and Y but ignored the 'tile' char.
    // This caused the Player to turn invisible (become ' ') as soon as they moved.
    void set(int _x, int _y, char tile = ' ') {
        x = _x;
        y = _y;
        tileChar = tile;
>>>>>>> Stashed changes
        inLimit();
    }
    void move(int xSteps, int ySteps, char tile) {
        set(x + xSteps, y + ySteps, tile);
    }

    void setTileChar(char tile) {
        tileChar = tile;
    }
<<<<<<< Updated upstream
    int getx() const{
        return x;
    }
    int gety() const{
=======
    int getx() const {
        return x;
    }
    int gety() const {
>>>>>>> Stashed changes
        return y;
    }
    Point getPosition() const {
        return { x,y };
    }

<<<<<<< Updated upstream
    char getTileChar() {
=======
    char getTileChar() const {
>>>>>>> Stashed changes
        return tileChar;
    }

    void draw() {
        draw(tileChar);
    }
    void draw(char c);


private:
    void inLimit();

    static bool isBlockingTile(char t) {
        return (t == 'W' || t == 'w');
    }

};




};