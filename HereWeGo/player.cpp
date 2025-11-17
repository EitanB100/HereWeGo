#include <cstdlib>
#include <iostream>
#include <conio.h>
#include "Player.h"
#include "Placement.h"
#include "CommandKeys.h"


void Player::move() {
    pos.draw(' '); // erase old position

    int targetX = pos.getx() + dirx;
    int targetY = pos.gety() + diry;

    pos.set(targetX, targetY, symbol);

    /* if (pos.onBlock()) {
         // Step back if we moved into a wall
         pos.set(targetX - dirx, targetY - diry, symbol);
     }*/

    pos.draw(symbol);
};

void Player::changeDirection() {

    if (_kbhit()) {
        char input = _getch();
        input = toupper(input); // your keys might be undercase

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
}