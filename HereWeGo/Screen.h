#pragma once
#include <cstdlib>
#include "utils.h"
#include "Placement.h"

class Screen {

private:
    char screen[MAX_Y][MAX_X + 1];
    bool silent = false;

public:
    Screen();

    void setSilent(bool _silent) { silent = _silent; }
    void loadMapFromRoom(const char mapData[MAX_Y][MAX_X]);
    void createGameScreen();


    inline void clearScreen() {
        if (silent) return;
        system("cls");
    }
    void draw() const;
};