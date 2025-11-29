#include <cstdlib>
#include <iostream>
#include <conio.h>
#include "Player.h"
#include "Key.h"



void Player::move(Room& room) {

    Placement nextPos = pos;
    nextPos.set(pos.getx() + dirx, pos.gety() + diry, symbol);

    if (isDoor(nextPos.getTileChar()))
    {
        if (room.checkDoor(pos.getPosition(), itemInHand))
            pos = nextPos;
    
        return;
    }

    if (nextPos.getTileChar() == WALL_TILE) return;
    
    if (nextPos.getTileChar() == KEY_TILE) {
        if (itemInHand.type == NONE) {
            
            Point temp = nextPos.getPosition();
            pickItem(temp, room, KEY_TILE);
        
        }
    }
};


void Player::pickItem(Point& position, Room& room, char symbol)
{
	if (itemInHand.type != NONE) return; // already holding an item

    Key* key = room.isKeyThere(position);

    if (key != nullptr && key->getIsActive()) {

        key->takeKey();
        itemInHand = { KEY, key->getKeyID()};
    }

}

void Player::changeDirection(char input) {
    if (input == 0) return;
    input = toupper(input); // normalize input
    if (input == keys[UP])
        setDirection(0, -1);
    else if (input == keys[DOWN])
        setDirection(0, 1);
    else if (input == keys[LEFT])
        setDirection(-1, 0);
    else if (input == keys[RIGHT])
        setDirection(1, 0);
    else if (input == keys[STAY]) setDirection(0, 0);
    
}