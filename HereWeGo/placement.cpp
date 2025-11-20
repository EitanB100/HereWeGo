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
    if (getx() > MAX_X)
        x = MAX_X;
    if (gety() > MAX_Y)
        y = MAX_Y;
}


