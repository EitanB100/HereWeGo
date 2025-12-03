#include <cstdlib>
#include <iostream>
#include <conio.h>
#include "Player.h"
#include "Key.h"
// Ensure Switch.h is included (either here or in Room.h)
#include "Switch.h" 

void Player::move(Room& room, Player* otherPlayer) {

    if (dirx == 0 && diry == 0) return; // No movement input
    
    // 1. Calculate where we want to go next
    Point nextPoint = { pos.getx() + dirx, pos.gety() + diry };

    // Look ahead at what is currently on the map
    char tileOnMap = room.getObjectAt(nextPoint);

    // --- COLLISION LOGIC ---
    if (tileOnMap == WALL_TILE) { //wall
        setDirection(0, 0); 
        return; 
    }

    if (otherPlayer != nullptr) { //collision with 2nd player
        Point otherPos = otherPlayer->getPos();
        if (nextPoint.x == otherPos.x && nextPoint.y == otherPos.y)
        {
            setDirection(0, 0);
            return;
        }
    }

    if (isDoorTile(tileOnMap)) //collision with door, there is no door tile so this is a "workaround"
    {
        room.checkDoor(nextPoint, itemInHand);
        doorHandling(room, itemInHand);
        return;
    }

    if (tileOnMap == KEY_TILE) { //key collision
     
        if (!keyHandling(room, nextPoint))
        return;
    }

    if (tileOnMap == OBSTACLE_TILE) { //obstacle collision
       
        if (obstacleHandling(room, nextPoint, otherPlayer)) {
            pos.draw(' ');
            pos.set(nextPoint.x, nextPoint.y, symbol);
            draw();
        }
        
        return;
    }

    if (tileOnMap == SWITCH_ON || tileOnMap == SWITCH_OFF) { //switch collision
        switchHandling(room, nextPoint);
        return;
    }

    // --- RENDERING LOGIC ---
    
    pos.draw(' ');
    
    pos.set(nextPoint.x, nextPoint.y, symbol); // Update and draw at new position
    draw(); 
}

void Player::doorHandling(Room& room, heldItem& _itemInHand ) //room might be used in future, when we add more rooms to project!
{
    setDirection(0, 0);

    setColor(_itemInHand.color);
    pos.draw();
    std::cout << std::flush;
}

bool Player::keyHandling(Room& room, Point& nextPoint)
{
    if (itemInHand.type == NONE)
    {
        pickItem(nextPoint, room, KEY);
        return true;
    }
    else {
        // Hands are full. STOP!
        setDirection(0, 0);
        return false;
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
            return;
        }
    
}

bool Player::obstacleHandling(Room& room, Point& nextPoint, Player* otherPlayer)
{
    Obstacle* obstacleToPush = room.isObstacleThere(nextPoint);
    if (obstacleToPush != nullptr && obstacleToPush->getHasMoved()) return false;

    int currentForce = this->force;
    bool combinedPush = false;

    if (otherPlayer != nullptr && obstacleToPush != nullptr)
    {
        Point otherPos = otherPlayer->getPos();

        Point otherTarget = { otherPos.x + otherPlayer->dirx, otherPos.y + otherPlayer->diry };
        Obstacle* otherObToPush = room.isObstacleThere(otherTarget);


        if (obstacleToPush == otherObToPush && dirx == otherPlayer->dirx && diry == otherPlayer->diry)
        {
            currentForce += otherPlayer->force;
            combinedPush = true;
        }
        bool hasMoved = room.moveObstacle(nextPoint, dirx, diry, currentForce);

        if (hasMoved && combinedPush) {
            synchronizePartner(otherPlayer);
        }
        return hasMoved;
    }
}



void Player::pickItem(Point& position, Room& room, char _symbol) //check about instead of getting a symbol, get a dynamic type object (torch/key etc.. maybe in future w/ gameobject)
{
    if (itemInHand.type != NONE) return; // already holding an item

    Key* key = room.isKeyThere(position);

    if (key != nullptr && key->getIsActive()) {

        key->takeKey();
        
        
        itemInHand = { KEY, key->getKeyID(), key->getColor()};
       
        room.clearTile(position);
    }
}

void Player::dropItem(Room& room) //item that isnt a bomb!
{
    switch (itemInHand.type) {
    case NONE:
        return;
        break;
    
    case KEY:
        room.addKey(Key(pos.getx(), pos.gety(), itemInHand.id, itemInHand.color));
        break;
    //case TORCH:
    //case BOMB:

        itemInHand = { NONE,0,Color::WHITE };
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