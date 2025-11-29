#include <cstdlib>
#include <iostream>
#include <conio.h>
#include "Player.h"
#include "Key.h"



void Player::move(Room& room) {

    if (dirx == 0 && diry == 0) return;

    
    Point nextPoint = { pos.getx() + dirx, pos.gety() + diry };

    char tileOnMap = room.getObjectAt(nextPoint);

    if (tileOnMap == WALL_TILE) {
        setDirection(0, 0);
        return; 
    }

    if (isDoor(tileOnMap))
    {
        if (!room.checkDoor(nextPoint, itemInHand))
        {
            setDirection(0, 0);
            return;
        }
        setDirection(0, 0);
        return;
    }

    if (tileOnMap == KEY_TILE) {
        if (itemInHand.type == NONE)
            pickItem(nextPoint, room, KEY);
        else {
            setDirection(0, 0);
            return;
        }
    }


    pos.draw(' ');
    pos.set(nextPoint.x, nextPoint.y, symbol);
    pos.draw();
};


void Player::pickItem(Point& position, Room& room, char symbol)
{
	if (itemInHand.type != NONE) return; // already holding an item

    Key* key = room.isKeyThere(position);

    if (key != nullptr && key->getIsActive()) {

        key->takeKey();
        itemInHand = { KEY, key->getKeyID()};
        room.clearTile(position);
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