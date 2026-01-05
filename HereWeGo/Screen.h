#pragma once
#include <cstdlib>
#include "utils.h"
#include "Placement.h"

class Screen {

private:
    char screen[MAX_Y][MAX_X + 1];

public:
    Screen();


    void loadMapFromRoom(const char mapData[MAX_Y][MAX_X]);
    void createGameScreen();


    inline void clearScreen() {
        system("cls");
    }
    void draw() const;
};