#pragma once

constexpr int MAX_DOORS = 10;

enum itemType { NONE, KEY, TORCH, BOMB }; // possible item types - last two for exercise 2

struct heldItem {
    itemType type = NONE;
    int id = 0; // key ID or bomb timer
};

struct Point { // simple struct for position getting
    int x, y;
};




void gotoxy(int x, int y);

void hideCursor();

void cls();