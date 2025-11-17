#include <iostream>
#include "Placement.h"
#include "utils.h"



void Placement::draw(char c) {
    gotoxy(pos[0], pos[1]);
    std::cout << c;
}

void Placement::inLimit() {
    if (getx() < 0)
        pos[0] = 0;
    if (gety() < 0)
        pos[1] = 0;
    if (getx() > MAX_X)
        pos[0] = MAX_X;
    if (gety() > MAX_Y)
        pos[1] = MAX_Y;
}


