#include <cstdlib>
#include <iostream>
#include <conio.h>
#include "Player.h"
#include "Key.h"
#include "Switch.h" 
#include "Torch.h"
#include "Bomb.h"
#include "Game.h"

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

void Player::transferMomentum(int _force, Point dir, int time)
{
    spring.force = _force;
    spring.launchDir = dir;
    spring.flightTime = time;
    spring.compressionCount = 0;
}

void Player::takeDamage(int amount)
{
    hp -= amount;
    gotHit = true;
    if (hp <= DEAD_HP) {
        hp = DEAD_HP;
        alive = false; // Mark as dead
    }
}

bool Player::increaseHP(int amount)
{
    if (hp >= MAX_HP && amount > 0) {
        hp = MAX_HP;
        return false;
    }

    hp += amount;
    if (hp > MAX_HP) hp = MAX_HP;
    return true;
}

void Player::setHP(int amount) {
    if (amount <= DEAD_HP) {
        hp = DEAD_HP;
        alive = false;
    }
    else if (amount > MAX_HP) {
        hp = MAX_HP;
    }
    else {
        hp = amount;
    }
}

//Movement logic: collisions, interactions, and updates
int Player::move(Room& room, Player* otherPlayer) {


    if (dirx == 0 && diry == 0) return 0; // No movement input
    
    Point nextPoint = { pos.getx() + dirx, pos.gety() + diry };
    char tileOnMap = room.getObjectAt(nextPoint);

    // Dynamic lighting
    if (this->itemInHand.type == ItemType::TORCH) {
        room.CompleteLineOfSight(Torch(pos.getx() + dirx, pos.gety() + diry));
    }
    
    //Static collisions
	if (tileOnMap == WALL_TILE || tileOnMap == UNKNOWN_TILE || tileOnMap == GLASS_TILE) {
        setDirection(Directions::STAY); 
        return 0; 
    }

    //Player collision 
    if (otherPlayer != nullptr && !otherPlayer->isFinished() && nextPoint == otherPlayer->getPos()) { 
        if (spring.flightTime > 0) {
            otherPlayer->transferMomentum(spring.force, { dirx,diry }, spring.flightTime);
            spring.flightTime = 0;
            spring.force = 1;
        }

        setDirection(Directions::STAY);
        return 0;
    }
    
    
    //door collision
    if (isDoorTile(tileOnMap)) 
    {
        room.checkDoor(nextPoint, itemInHand);
        setDirection(Directions::STAY);
        if (!Game::s_silentMode) {
            setColor(itemInHand.color);
            pos.draw();
        }
        return 0;
    }

    //Pickups
    if (tileOnMap == KEY_TILE || tileOnMap == TORCH_TILE || tileOnMap == BOMB_TILE) { 
     
        if (!handlePickups(room, nextPoint))
        {
            setDirection(Directions::STAY);
            return 0;
        }
        
    }

    //obstacle collision (pushing)
    if (tileOnMap == OBSTACLE_TILE) { 
     
        //try to push an obstacle. return true if possible
        if (!obstacleHandling(room, nextPoint, otherPlayer)) return 0;
    }

    if (tileOnMap == SPRING_TILE) {
        if (!handleSprings(room, nextPoint)) return 0;
    }

    else  {
        if (!handleSpringExit(room)) return 0;
    }

    //switch collision
    if (tileOnMap == SWITCH_ON || tileOnMap == SWITCH_OFF) { 
        Switch* switchOnOff = room.isSwitchThere(nextPoint);
        if (switchOnOff != nullptr) {
            switchOnOff->toggleState();      

            room.checkSwitch(switchOnOff->getPos()); 
            if (!Game::s_silentMode)
                room.drawTopLayer();                 
            setDirection(Directions::STAY);                  
            return 0;
        }
    }

    if (tileOnMap == POTION_TILE) {
        Potion* potion = room.isPotionThere(nextPoint);
        if (potion) {
            if (increaseHP(HP_INCREASE))
                room.removePotion(nextPoint);
            else {
                setDirection(Directions::STAY);
                return 0;
            }
           
        }
         
    }
    
    if (tileOnMap == RIDDLE_TILE) {
        int id = room.getRiddleID(nextPoint);

        setDirection(Directions::STAY);
        return id;
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
    return 0;
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


void Player::inputManager(char input, Room& room) {

    if (finishedLevel) return; //when a player finishes the level it freezes

    if (input == 0) return;

    input = toupper(input); // normalize input to uppercase


    if (isCommand(input, CommandKeys::UP)) setDirection(Directions::UP);

    else if (isCommand(input, CommandKeys::DOWN)) setDirection(Directions::DOWN);

    else if (isCommand(input, CommandKeys::LEFT)) setDirection(Directions::LEFT);

    else if (isCommand(input, CommandKeys::RIGHT)) setDirection(Directions::RIGHT);

    else if (isCommand(input, CommandKeys::STAY)) setDirection(Directions::STAY);
        

    else if (isCommand(input,CommandKeys::DISPOSE))
        dropItem(room);
    
    else return;
    
    
    
}