#pragma once

struct Point { // simple struct for position getting
    int x, y;
};

static constexpr int MAX_X = 79;
static constexpr int MAX_Y = 24;

void gotoxy(int x, int y);

void hideCursor();

void cls();