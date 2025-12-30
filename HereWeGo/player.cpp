#include <cstdlib>
#include <iostream>
#include <conio.h>
#include "Player.h"
#include "Key.h"
#include "Switch.h" 
#include "Torch.h"
#include "Bomb.h"

Player::Player(const Placement& p, char c, int directx, int directy, const char keyArray [keyAmount])
{
    symbol = c;
    pos.set(p.getx(), p.gety(), symbol);
    dirx = directx;
    diry = directy;
    for (int i = 0; i < keyAmount; ++i)
        keys[i] = keyArray[i];
}

void Player::draw() {
    if (itemInHand.type != ItemType::NONE) {
        setColor(itemInHand.color);
    }
    pos.draw();
    setColor(Color::WHITE);
}
//Movement logic: collisions, interactions, and updates
void Player::move(Room& room, Player* otherPlayer) {

    if (dirx == 0 && diry == 0) return; // No movement input
    
    Point nextPoint = { pos.getx() + dirx, pos.gety() + diry };
    char tileOnMap = room.getObjectAt(nextPoint);

    // Dynamic lighting
    if (this->itemInHand.type == ItemType::TORCH) {
        room.CompleteLineOfSight(Torch(pos.getx() + dirx, pos.gety() + diry));
    }
    
    //Static collisions
	if (tileOnMap == WALL_TILE || tileOnMap == UNKNOWN_TILE || tileOnMap == GLASS_TILE) {
        setDirection(0, 0); 
        return; 
    }

    //Player collision 
    if (otherPlayer != nullptr && !otherPlayer->isFinished() && nextPoint == otherPlayer->getPos()) { 
        setDirection(0, 0);
        return;
    }
    
    
    //door collision
    if (isDoorTile(tileOnMap)) 
    {
        room.checkDoor(nextPoint, itemInHand);
        setDirection(0, 0);
        setColor(itemInHand.color);
        pos.draw();
        return;
    }

    //Pickups
    if (tileOnMap == KEY_TILE || tileOnMap == TORCH_TILE || tileOnMap == BOMB_TILE) { 
     
        if (!handlePickups(room, nextPoint))
        {
            setDirection(0, 0);
            return;
        }
        
    }

    //obstacle collision (pushing)
    if (tileOnMap == OBSTACLE_TILE) { 
     
        //try to push an obstacle. return true if possible
        if (!obstacleHandling(room, nextPoint, otherPlayer)) return;
    }

    if (tileOnMap == SPRING_TILE) {
        if (!handleSprings(room, nextPoint)) return;
    }

    else  {
        if (!handleSpringExit(room)) return;
    }

    //switch collision
    if (tileOnMap == SWITCH_ON || tileOnMap == SWITCH_OFF) { 
        Switch* switchOnOff = room.isSwitchThere(nextPoint);
        if (switchOnOff != nullptr) {
            switchOnOff->toggleState();      

            room.checkSwitch(switchOnOff->getPos()); 
            room.drawTopLayer();                 
            setDirection(0, 0);                  
            return;
        }
    }

    // execution - happens after checked if can proceed
    Point currentPos = getPos();
    Color objectColor = Color::WHITE;
    //what the player is on so we redraw it when it moves
    char objectChar = room.getObjectAt(currentPos, objectColor);

    if (objectChar == SPRING_TILE && spring.compressionCount > 0) {
        objectChar = ' ';
    }

    setColor(objectColor);
    pos.draw(objectChar); 
    
    setColor(Color::WHITE);
    pos.set(nextPoint.x, nextPoint.y, symbol); 
    draw(); 
}

bool Player::handlePickups(Room& room, Point nextPoint) {
    if (itemInHand.type != ItemType::NONE) return false; // Hands full

    Key* key = room.isKeyThere(nextPoint);
    if (key != nullptr) {
        itemInHand = { ItemType::KEY, key->getKeyID(), key->getColor() };
        room.removeKey(nextPoint);
        return true;
    }

    Torch* torch = room.isTorchThere(nextPoint);
    if (torch != nullptr) {
        itemInHand = { ItemType::TORCH, 0, torch->getColor() };
        room.removeTorch(nextPoint);
        return true;
    }
    Bomb* bomb = room.isBombThere(nextPoint);
    if (bomb != nullptr) {
        itemInHand = { ItemType::BOMB, bomb->getBombID(), bomb->getColor()};
        room.removeBomb(nextPoint);
        return true;
    }
    return false;
}

bool Player::handleSpringExit(Room& room)
{

    if (spring.compressionCount == 0) return true;

    Spring* s = room.isSpringThere(pos.getPosition());
    if (!s) {
        spring.compressionCount = 0;
        return true;
    }

    Point springDir = s->getDirection();
    bool isOpposing = (dirx == -springDir.x && diry == -springDir.y);
    
    if (isOpposing && spring.compressionCount >= s->getParts().size()) {
        setDirection(0, 0);
        return false;
    }
    s->setCompression(0);
    s->draw();
    spring.compressionCount = 0;
    return true;
}

bool Player::handleSprings(Room& room, Point nextPoint) {
    // A. Flight Logic (Chaining)
    if (spring.flightTime > 0) {
        Spring* nextSpring = room.isSpringThere(nextPoint);
        Spring* currentSpring = room.isSpringThere(pos.getPosition());

        if (nextSpring != nullptr && nextSpring != currentSpring) {
            Point tip = nextSpring->getParts()[0].getPosition();

            // Hit Tip -> Chain
            if (nextPoint == tip) {
                int conservedMomentum = spring.force;
                if (conservedMomentum > (int)nextSpring->getParts().size()) {
                    conservedMomentum = (int)nextSpring->getParts().size();
                }
                nextSpring->setCompression(conservedMomentum);
                spring.compressionCount = conservedMomentum;
                spring.flightTime = 0; // Stop flight, land on spring
                spring.force = 1;
                return true; // Continue move logic (step onto spring)
            }
            else {
                if (spring.flightTime == 0) {
                    setDirection(0, 0);
                    return false;
                }
                return true;
            }
        }
    }

    // compression Logic (Walking onto Spring)
    if (spring.flightTime == 0) {
        Spring* s = room.isSpringThere(nextPoint);
        if (s) {
            bool isOpposing = (dirx == -s->getDirection().x && diry == -s->getDirection().y);
            bool alreadyOnSpring = s->isSpringPart(pos.getPosition());
            Point tip = s->getParts()[0].getPosition();

            // Only allow entry from the tip or if already on it
            if (!alreadyOnSpring && nextPoint != tip) {
                setDirection(0, 0);
                return false;
            }

            // Stop if fully compressed
            if (spring.compressionCount >= (int)s->getParts().size() && isOpposing) {
                setDirection(0, 0);
                return false;
            }
            spring.compressionCount++;
            s->setCompression(spring.compressionCount);
        }
    }
    return true;
}

//Helper function that ensures that when two playes push an obstacle, 
//both will move in sync with each other
//Suggested by Gemini!
void Player::synchronizePartner(Player* otherPlayer, Room& room) {
    if (otherPlayer == nullptr) return;     
    
    //redraw the tile (similar to move function)
    Point p = otherPlayer->getPos();
    Color c = Color::WHITE;
    char tileBelow = room.getObjectAt(p, c); //the tile that became invisible when disposed!

    setColor(c);
    otherPlayer->pos.draw(tileBelow);
    setColor(Color::WHITE);
    
    //move the partner in the same direction
    otherPlayer->pos.move(dirx, diry, otherPlayer->symbol);
    otherPlayer->draw();
}

//Obstacle pushing logic
bool Player::obstacleHandling(Room& room, Point& nextPoint, Player* otherPlayer)
{
    Obstacle* obstacleToPush = room.isObstacleThere(nextPoint);
    //if it already been moved this frame
    if (obstacleToPush != nullptr && obstacleToPush->getHasMoved()) return false;

    int currentForce = this->force; //default is 1
    bool combinedPush = false;

    //if the other player is pushing same obstacle in same direction
    if (otherPlayer != nullptr && obstacleToPush != nullptr)
    {
        Point otherPos = otherPlayer->getPos();

        Point otherTarget = { otherPos.x + otherPlayer->dirx, otherPos.y + otherPlayer->diry };
        Obstacle* otherObToPush = room.isObstacleThere(otherTarget);

        //if obstacles pushed and directions matched, combine the forces
        if (obstacleToPush == otherObToPush && dirx == otherPlayer->dirx && diry == otherPlayer->diry)
        {
            currentForce += otherPlayer->force;
            combinedPush = true;
        }
        
        //move the obstacle with the calculated force
        bool hasMoved = room.moveObstacle(nextPoint, dirx, diry, currentForce);
        
        //if moved, move the partner too
        if (hasMoved) {
            if (combinedPush) synchronizePartner(otherPlayer, room);
            
            if (spring.flightTime > 0) obstacleToPush->resetMove();
        }
        return hasMoved;
    }
    return false; 
}

void Player::dropItem(Room& room) //item that isnt a bomb!
{
    switch (itemInHand.type) {
    case ItemType::NONE:
        return;
        break;

    case ItemType::KEY: {
        //create a new key on the floor at the player's position
        Key key(pos.getx(), pos.gety(), itemInHand.id, itemInHand.color);
        key.setSeen();
        room.addKey(key);
        break;
    }
    case ItemType::TORCH:
        room.addTorch(Torch(pos.getx(), pos.gety()));
        break;

    case ItemType::BOMB:
        Bomb newBomb(pos.getx(), pos.gety(), itemInHand.id, 5);
        newBomb.setSeen();
        newBomb.activate();
        room.addBomb(newBomb); // This sends it "down" to the room's vector
        break;
}

    //reset inventory
    itemInHand = { ItemType::NONE,0,Color::WHITE };
    draw();
}

void Player::updateSpringPhysics(Room& room, Player* otherPlayer)
{
    if (spring.compressionCount > 0) {    //is a spring compressed?
        Spring* s = room.isSpringThere(pos.getPosition());
        if (s) {
            Point springDir = s->getDirection();

            bool isCompressing = (dirx == -springDir.x && diry == -springDir.y);

            if (!isCompressing) {
                spring.force = spring.compressionCount;
                spring.flightTime = spring.force * spring.force;
                spring.launchDir = springDir;
                
                spring.compressionCount = 0;
                s->setCompression(0);
            }
        }
    }
    //launch loop
    if (spring.flightTime > 0) {
        int userDirx = dirx; //store before changing values
        int userDiry = diry;

        int originalForce = force;
        force = spring.force;

        dirx = spring.launchDir.x;
        diry = spring.launchDir.y;

        for (int i = 0; i < spring.force; i++) {
            
            Point startPos = getPos();
            move(room, otherPlayer);
            
            if (startPos == getPos()) {
                spring.flightTime = 0;
                spring.force = 1;
                break;
            }
        }
        force = originalForce;
        dirx = userDirx;
        diry = userDiry;
        
        if (spring.flightTime > 0) spring.flightTime--;
        else spring.force = 1;
    }
}

void Player::inputManager(char input, Room& room) {

    if (finishedLevel) return; //when a player finishes the level it freezes

    if (input == 0) return;

    input = toupper(input); // normalize input to uppercase

    int requestedDirx = 0;
    int requestedDiry = 0;

    if (isCommand(input,CommandKeys::UP)) {
        requestedDiry = -1;
        requestedDirx = 0;
    }

    else if (isCommand(input,CommandKeys::DOWN)) {

        requestedDirx = 0;
        requestedDiry = 1;
    }

    else if (isCommand(input,CommandKeys::LEFT)) {
        requestedDirx = -1;
        requestedDiry = 0;
    }
    else if (isCommand(input,CommandKeys::RIGHT)) {
        requestedDirx = 1;
        requestedDiry = 0;
    }

    else if (isCommand(input,CommandKeys::STAY))
        setDirection(0, 0);

    else if (isCommand(input,CommandKeys::DISPOSE))
        dropItem(room);
    
    else return;
    
    setDirection(requestedDirx, requestedDiry);
    
}