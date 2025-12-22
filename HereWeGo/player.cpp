#include <cstdlib>
#include <iostream>
#include <conio.h>
#include "Player.h"
#include "Key.h"
#include "Switch.h" 
#include "Torch.h"

//Movement logic: collisions, interactions, and updates
void Player::move(Room& room, Player* otherPlayer) {

    if (dirx == 0 && diry == 0) return; // No movement input
    
    // Calculate where we want to go next
    Point nextPoint = { pos.getx() + dirx, pos.gety() + diry };

    // whats at the target coordinate
    char tileOnMap = room.getObjectAt(nextPoint);

    // Dynamic lighting mechanic for torch (lights up area around new position)
    if (this->itemInHand.type == TORCH) {
        room.CompleteLineOfSight(Torch(pos.getx() + dirx, pos.gety() + diry));
    }
    

    // Collision logic
    //static objects:

	if (tileOnMap == WALL_TILE || tileOnMap == UNKNOWN_TILE || tileOnMap == GLASS_TILE) {
        setDirection(0, 0); 
        return; 
    }

    //between players (if one finishes, it waits at the door
    // so to not block the other, collision checks won't happen in that case) 
    if (otherPlayer != nullptr && !otherPlayer->isFinished() && nextPoint == otherPlayer->getPos()) { 
        setDirection(0, 0);
        return;
    }
    
    
    //door collision (checking if the player has a fitting key)
    if (isDoorTile(tileOnMap)) 
    {
        room.checkDoor(nextPoint, itemInHand);
        setDirection(0, 0);
        setColor(itemInHand.color);
        pos.draw();
        return;
    }

    //pickup items collision
    if (tileOnMap == KEY_TILE || tileOnMap == TORCH_TILE) { 
     
        if (!pickItem(nextPoint, room))
        {
            setDirection(0, 0);
            return;
        }
        
    }

    //obstacle collision (pushing)
    if (tileOnMap == OBSTACLE_TILE) { 
     
        //try to push an obstacle. return true if possible
        if (!obstacleHandling(room,nextPoint,otherPlayer)) return;
    }

    if (tileOnMap == SPRING_TILE && spring.flightTime > 0) {
        Spring* nextSpring = room.isSpringThere(nextPoint);
        Spring* currentSpring = room.isSpringThere(pos.getPosition());

        if (nextSpring != nullptr && nextSpring != currentSpring) {

            // get the tip of the target spring
            Point tip = nextSpring->getParts()[0].getPosition();
            bool isTip = (nextPoint == tip);

            
            
            if (isTip) { //if colliding with tip of another spring, chain momentum in new spring's direction
          
                int conservedMomentum = spring.force;

       
                if (conservedMomentum > nextSpring->getParts().size()) {
                    conservedMomentum = (int)nextSpring->getParts().size();
                }

                nextSpring->setCompression(conservedMomentum);
                spring.compressionCount = conservedMomentum; // Update player state so they launch next frame

                
                // this ensures we land on the spring tip and don't fly past it
                spring.flightTime = 0;
                spring.force = 1;

                
                
            }
            else { //not a tip - treat as a wall.
       
                setDirection(0, 0);
                return;
            }
        }
    }

    if (tileOnMap == SPRING_TILE && spring.flightTime == 0) {

        Spring* s = room.isSpringThere(nextPoint);
        if (s) {
            bool isOpposing = (dirx == -s->getDirection().x && diry == -s->getDirection().y);
            bool alreadyOnSpring = s->isSpringPart(pos.getPosition());

            Point p = s->getParts()[0].getPosition();
            bool isTip = (p == nextPoint);

            if (!alreadyOnSpring && !isTip) {
                setDirection(0, 0);
                return;
            }

            if (spring.compressionCount >= s->getParts().size() && isOpposing) {
                setDirection(0, 0);
                return;
            }
            spring.compressionCount++;
            s->setCompression(spring.compressionCount);
        }
    }

    else if (spring.flightTime == 0 && spring.compressionCount > 0) {
        Spring* s = room.isSpringThere(pos.getPosition());
        if (s) {
            Point sDir = s->getDirection();
            if (dirx == -sDir.x && diry == -sDir.y) {
                setDirection(0, 0);
                return;
            }

            s->setCompression(0);
        }
        spring.compressionCount = 0;
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
    pos.set(nextPoint.x, nextPoint.y, symbol); // Update player position
    draw(); //drawing it on screen
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



//Picks up an item if hands are empty
bool Player::pickItem(Point& position, Room& room) 
{
    if (itemInHand.type != NONE) return false; // already holding an item

    Key* key = room.isKeyThere(position);
	
    if (key != nullptr) {
        //store data in player inventory
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
        //create a new key on the floor at the player's position
        Key key(pos.getx(), pos.gety(), itemInHand.id, itemInHand.color);
        key.setSeen();
        room.addKey(key);
        break;
    }
    case TORCH:
        room.addTorch(Torch(pos.getx(), pos.gety()));
        break;
    }
    //reset inventory
    itemInHand = { NONE,0,Color::WHITE };
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

    if (input == keys[UP]) {
        requestedDiry = -1;
        requestedDirx = 0;
    }

    else if (input == keys[DOWN]) {

        requestedDirx = 0;
        requestedDiry = 1;
    }

    else if (input == keys[LEFT]) {
        requestedDirx = -1;
        requestedDiry = 0;
    }
    else if (input == keys[RIGHT]) {
        requestedDirx = 1;
        requestedDiry = 0;
    }

    else if (input == keys[STAY])
        setDirection(0, 0);

    else if (input == keys[DISPOSE])
        dropItem(room);
    
    else return;
    
    setDirection(requestedDirx, requestedDiry);
    
}