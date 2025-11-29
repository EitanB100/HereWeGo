#include <cstdlib>
#include <iostream>
#include <conio.h>
#include "Player.h"
#include "Key.h"



void Player::move(Room& room) {

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
        // Try to unlock. If successful, key is used.
        if (!room.checkDoor(nextPoint, itemInHand))
        {
            setDirection(0, 0); // Locked. Stop.
            return;
        }
        setDirection(0, 0); // Opened. Stop to admire the amazing work.
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

    // --- RENDERING LOGIC ---

    // We print ' ' at the OLD position to clear the player's trail.
    // This is safe now because strictly controlled collision above guarantees
    // we never walked on top of a vital object.
    pos.draw(' ');
    // Update and draw at new position
    pos.set(nextPoint.x, nextPoint.y, symbol);
    draw(); //draw with a color
};


void Player::pickItem(Point& position, Room& room, char symbol)
{
	if (itemInHand.type != NONE) return; // already holding an item

    Key* key = room.isKeyThere(position);

    if (key != nullptr && key->getIsActive()) {

        key->takeKey();
        itemInHand = { KEY, key->getKeyID(), key->getColor()};
      
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