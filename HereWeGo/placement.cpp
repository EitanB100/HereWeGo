#include <iostream>
#include "Placement.h"
#include "utils.h"



void Placement::draw(char c) {
    gotoxy(x, y);
    std::cout << c;
}

void Placement::inLimit() {
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (getx() > MAX_X - 1)
        x = MAX_X - 1;
    if (gety() > MAX_Y - 1)
        y = MAX_Y - 1;
}


