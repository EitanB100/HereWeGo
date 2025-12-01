#include <cstdlib>
#include <iostream>
#include <conio.h>
#include "Player.h"
#include "Key.h"
// Ensure Switch.h is included (either here or in Room.h)
#include "Switch.h" 

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
        doorHandling(room, nextPoint, itemInHand);
        return;
    }

    if (tileOnMap == KEY_TILE) {
     
        keyHandling(room, nextPoint);
        return;
    }

    if (isSwitch(tileOnMap)) {
        switchHandling(room, nextPoint);
        return;
    }

    // --- RENDERING LOGIC ---
    
    pos.draw(' ');
    
    pos.set(nextPoint.x, nextPoint.y, symbol); // Update and draw at new position
    draw(); 
}

void Player::doorHandling(Room& room, Point& nextPoint, heldItem& itemInHand )
{
    bool isOpened = room.checkDoor(nextPoint, itemInHand);

    setDirection(0, 0);

    setColor(itemInHand.color);
    pos.draw();
    std::cout << std::flush;
}

void Player::keyHandling(Room& room, Point& nextPoint)
{
    if (itemInHand.type == NONE)
    {
        pickItem(nextPoint, room, KEY);
    }
    else {
        // Hands are full. STOP!
        setDirection(0, 0);
    }
}

void Player::switchHandling(Room& room, Point& nextPoint)
{
    Switch* switchOnOff = room.isSwitchThere(nextPoint);
    if (switchOnOff != nullptr) {
        switchOnOff->toggleState();          // toggle the switch state
        room.checkSwitch(switchOnOff->getPos()); // update doors
        room.drawTopLayer();                 // redraw
        setDirection(0, 0);                  // stop player
        
    }
}

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