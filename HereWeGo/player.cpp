#include <cstdlib>
#include <iostream>
#include <conio.h>
#include "Player.h"
#include "Key.h"
#include "Switch.h" 
#include "Torch.h"

void Player::move(Room& room, Player* otherPlayer) {

    if (dirx == 0 && diry == 0) return; // No movement input
    
    // 1. Calculate where we want to go next
    Point nextPoint = { pos.getx() + dirx, pos.gety() + diry };

    // Look ahead at what is currently on the map
    char tileOnMap = room.getObjectAt(nextPoint);

    if (this->itemInHand.type == TORCH) {
        room.CompleteLineOfSight(Torch(pos.getx() + dirx, pos.gety() + diry));
    }
    

    // --- COLLISION LOGIC ---
	if (tileOnMap == WALL_TILE || tileOnMap == UNKNOWN_TILE || tileOnMap == GLASS_TILE) { //wall or unknown tile which might be collideable
        setDirection(0, 0); 
        return; 
    }

    
        if (otherPlayer != nullptr && !otherPlayer->isFinished() && nextPoint.x == otherPlayer->getPos().x && nextPoint.y == otherPlayer->getPos().y ) { //collision with 2nd player
            setDirection(0, 0);
            return;
        }
    


    if (isDoorTile(tileOnMap)) //collision with door, there is no door tile so this is a "workaround"
    {
        room.checkDoor(nextPoint, itemInHand);
        setDirection(0, 0);
        setColor(itemInHand.color);
        pos.draw();
        return;
    }

    if (tileOnMap == KEY_TILE || tileOnMap == TORCH_TILE) { //key collision
     
        if (!pickItem(nextPoint, room))
        {
            setDirection(0, 0);
            return;
        }
        
    }

    if (tileOnMap == OBSTACLE_TILE) { //obstacle collision
       
        if (obstacleHandling(room, nextPoint, otherPlayer)) {
            Point currentPos = getPos();
            Color c = Color::WHITE;
            char objectHeld = room.getObjectAt(currentPos, c);
            setColor(c);
            pos.draw(objectHeld);
            setColor(Color::WHITE);
            
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
    Point currentPos = getPos();
    Color objectColor = Color::WHITE;
    char objectChar = room.getObjectAt(currentPos, objectColor);

    setColor(objectColor);
    pos.draw(objectChar);
    
    setColor(Color::WHITE);
    pos.set(nextPoint.x, nextPoint.y, symbol); // Update and draw at new position
    draw(); 
}

void Player::synchronizePartner(Player* otherPlayer, Room& room) {
    if (otherPlayer == nullptr) return;     

    Point p = otherPlayer->getPos();
    Color c = Color::WHITE;
    char tileBelow = room.getObjectAt(p, c); //the tile that became invisible when disposed!

    setColor(c);
    otherPlayer->pos.draw(tileBelow);
    setColor(Color::WHITE);

    otherPlayer->pos.move(dirx, diry, otherPlayer->symbol);
    otherPlayer->draw();
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
            synchronizePartner(otherPlayer, room);
        }
        return hasMoved;
    }
}



bool Player::pickItem(Point& position, Room& room) //check about instead of getting a symbol, get a dynamic type object (torch/key etc.. maybe in future w/ gameobject)
{
    if (itemInHand.type != NONE) return false; // already holding an item

    Key* key = room.isKeyThere(position);
	
    if (key != nullptr) {
        
        itemInHand = { KEY, key->getKeyID(), key->getColor()};
        room.removeKey(position);
        return true;
    }
    Torch* torch = room.isTorchThere(position);
	if (torch != nullptr) {
		itemInHand = { TORCH, 0, torch->getColor()};
		room.removeTorch(position);
        return true;
	}
    return false;
}

void Player::dropItem(Room& room) //item that isnt a bomb!
{
    switch (itemInHand.type) {
    case NONE:
        return;
        break;

    case KEY: {
        Key key(pos.getx(), pos.gety(), itemInHand.id, itemInHand.color);
        key.setSeen();
        room.addKey(key);
        break;
    }
    case TORCH:
        room.addTorch(Torch(pos.getx(), pos.gety()));
        break;
    }
    itemInHand = { NONE,0,Color::WHITE };
    draw();
}

void Player::inputManager(char input, Room& room) {

    if (finishedLevel) return; 
    switch (input) {
    case 0:
        return;
    
    }

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
    else if (input == keys[STAY])
        setDirection(0, 0);
    else if (input == keys[DISPOSE])
        dropItem(room);
    
}