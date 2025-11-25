#include <cstdlib>
#include <iostream>
#include <conio.h>
#include "Player.h"
#include "Key.h"



void Player::move(Room& room) {

    if (dirx == 0 && diry == 0) return; // no movement

	Placement nextMove = pos; // copy current position
	nextMove.set(pos.getx() + dirx, pos.gety() + diry, symbol); // calculate next position

    /*if (!room.inBounds(nextMove.getPosition()))
    {
		setDirection(0, 0); // stop movement
        return; // out of bounds
    }*/
	Point nextPos = nextMove.getPosition();

	switch (room.getObjectAt(nextPos)) //will be implemented in a more readable way later
    {
        case 'W':
			setDirection(0, 0); // stop movement
            return;
            break;

		case 'D': // door
            if (room.checkDoor(nextPos, itemInHand))
            {
				itemInHand = { NONE, 0 }; // the key has been used 
                break;
            }
            else {
				setDirection(0, 0); // stop movement
				return; // can't move through closed door
            }
            
            break;

        case 'K': // key
            if (itemInHand.type == NONE)
            {
                pickItem(nextPos, room, 'K');
				break;
            }
                
            else
                setDirection(0, 0);
			return;// can't pick up another item so it collides with it
            break;

        case ' ':
			break;
    }
   
	pos.draw(' '); // erase previous position
	pos = nextMove; // update to new position
    pos.draw(symbol);
};


void Player::pickItem(Point& position, Room& room, char symbol)
{
	if (itemInHand.type != NONE) return; // already holding an item

    switch (symbol)
    {
    case 'K':
		Key * keyAtPos = room.isKeyThere(position);
        if (keyAtPos != nullptr) {
            itemInHand.type = KEY;
            itemInHand.id = keyAtPos->getKeyID();
            keyAtPos->takeKey(); // mark key as picked up
		}
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