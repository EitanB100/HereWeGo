#include <cstdlib>
#include <iostream>
#include <conio.h>
#include "Player.h"
#include "Placement.h"
#include "CommandKeys.h"
#include "Screen.h"

void Player::move() {
    pos.draw(' '); // erase old position
    Placement preMove = pos;
    int targetX = pos.getx() + dirx;
    int targetY = pos.gety() + diry;

    pos.set(targetX, targetY, symbol);
    if (area.isWall(pos)) {
        pos = preMove;
    }
    pos.draw(symbol);
};

void Player::changeDirection(char input) {
    if (input == 0) return;
    input = toupper(input); // normalize input
    if (input == keys[UP] && getdirctY() <= 0)
        setDirection(0, -1);
    else if (input == keys[DOWN] && getdirctY() >= 0)
        setDirection(0, 1);
    else if (input == keys[LEFT] && getdirctX() <= 0)
        setDirection(-1, 0);
    else if (input == keys[RIGHT] && getdirctX() >= 0)
        setDirection(1, 0);
    else if (input == keys[STAY]) setDirection(0, 0);
    
}