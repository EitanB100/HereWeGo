#include <cstdlib>
#include <iostream>
#include <conio.h>
#include "Player.h"
#include "Key.h"



void Player::move(Room& room) {

<<<<<<< Updated upstream
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
=======
    if (dirx == 0 && diry == 0) return; // No movement input

    // 1. Calculate where we want to go next
    Point nextPoint = { pos.getx() + dirx, pos.gety() + diry };

    // Look ahead at what is currently on the map
    char tileOnMap = room.getObjectAt(nextPoint);


    // --- COLLISION LOGIC ---
    if (tileOnMap == WALL_TILE) {
        setDirection(0, 0); // Hit a wall, stop.
        return;
    }

    if (isDoor(tileOnMap))
    {
        bool isOpened = room.checkDoor(nextPoint, itemInHand);

        setDirection(0, 0);

        setColor(itemInHand.color);
        pos.draw();
        std::cout << std::flush;
        return;
    }

    if (tileOnMap == KEY_TILE) {
        if (itemInHand.type == NONE)
        {
            pickItem(nextPoint, room, KEY);
        }
        else {
            // Hands are full. STOP!
            // This is preventing the drawing code below to print ' ' over the key, 
            // deleting it from the game forever.
            setDirection(0, 0);
            return;
        }
    }

    if (isSwitch(tileOnMap)) {
        Switch* switchOnOff = room.isSwitchThere(nextPoint);
        if (switchOnOff != nullptr) {
            switchOnOff->toggleState();          // toggle the switch state
            room.checkSwitch(switchOnOff->getPos()); // update doors that depend on this switch
            room.drawTopLayer();                 // redraw after doors update
            setDirection(0, 0);                  // stop player
            return;
        }
    }

    // --- RENDERING LOGIC ---

    // We print ' ' at the OLD position to clear the player's trail.
    // This is safe now because strictly controlled collision above guarantees
    // we never walked on top of a vital object.
    pos.draw(' ');
    // Update and draw at new position
    pos.set(nextPoint.x, nextPoint.y, symbol);
    draw(); //draw with a color
>>>>>>> Stashed changes
};


void Player::pickItem(Point& position, Room& room, char symbol)
{
<<<<<<< Updated upstream
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
=======
    if (itemInHand.type != NONE) return; // already holding an item

    Key* key = room.isKeyThere(position);

    if (key != nullptr && key->getIsActive()) {

        key->takeKey();
        itemInHand = { KEY, key->getKeyID(), key->getColor() };

        room.clearTile(position);
>>>>>>> Stashed changes
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