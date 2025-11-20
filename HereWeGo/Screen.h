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

    void clearScreen() {
        system("cls");
    }


    void draw() const;
};