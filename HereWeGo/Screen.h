#pragma once
#include <cstdlib>
#include "utils.h"
#include "Placement.h"

class Screen {

private:
    char screen[MAX_Y][MAX_X + 1];
    void loadMapData(const char* mapData[MAX_Y]);

public:
    Screen();


    void loadMapFromRoom(const char mapData[MAX_Y][MAX_X]);
    void createGameScreen();

    void setTile(int x, int y, char symbol) ;

    char getTile(int x, int y) const {
        if (x >= 0 && x < MAX_X && y >= 0 && y < MAX_Y) {
            return screen[y][x]; 
        }
        return ' ';
    }

    inline void clearScreen() {
        system("cls");
    }
    void draw() const;
};