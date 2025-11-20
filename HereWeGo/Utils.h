#pragma once

enum itemType { NONE, KEY, TORCH, BOMB }; // possible item types - last two for exercise 2

struct heldItem {
    itemType type = NONE;
    int id = 0; // key ID or bomb timer
};

struct Point { // simple struct for position getting
    int x, y;
};



static constexpr int MAX_X = 79;
static constexpr int MAX_Y = 24;

void gotoxy(int x, int y);

void hideCursor();

void cls();