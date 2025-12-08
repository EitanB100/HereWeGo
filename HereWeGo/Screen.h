#pragma once
#include <cstdlib>
#include "utils.h"
#include "Placement.h"

class Screen {

private:
    char* screen[MAX_Y];

    

public:
    Screen();
    ~Screen();

    void createGameScreen();

    void setTile(int x, int y, char symbol) ;
    //void inLimit(Placement& p);

    char getTile(int x, int y) const {
        if (x >= 0 && x < MAX_X && y >= 0 && y < MAX_Y) {
            return screen[y][x]; 
        }
        return ' ';
    }

    void clearScreen() {
        system("cls");
    }
    void Lvl1Screen();
    void Lvl2Screen();
    void Lvl3Screen();

    void draw() const;
};