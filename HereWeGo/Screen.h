#pragma once
#include <cstdlib>
#include "utils.h"
#include "Placement.h"

class Screen {

private:
    char* screen[MAX_Y];

    char charAt(Placement& p) const {
        return screen[p.gety()][p.getx()];
    }

public:
    Screen();
    void createGameScreen();

    void setTile(int x, int y, char symbol) const;
    void inLimit(Placement& p);

    void clearScreen() {
        system("cls");
    }

    bool isWall(Placement& p) const {
        return charAt(p) == 'W';
    }
    bool isWonChar(Placement& p) const {
        return charAt(p) == '%';
    }

    void draw() const;
};