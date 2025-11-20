#include <cstdlib>
#include <iostream>
#include <conio.h>
#include "Player.h"
#include "Placement.h"
#include "CommandKeys.h"
#include "Screen.h"

void Player::move() {

    if (dirx == 0 && diry == 0) return; // no movement

	Placement nextMove = pos; // copy current position
	nextMove.set(pos.getx() + dirx, pos.gety() + diry, symbol); // calculate next position

    if (area.isWall(nextMove)) {
        setDirection(0, 0); // stop movement if hit wall
        return;
    }
	pos.draw(' '); // erase previous position
	pos.set(pos.getx() + dirx, pos.gety() + diry, symbol);
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