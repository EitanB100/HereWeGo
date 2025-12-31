#include "Player.h"
#include "Room.h"
#include "Obstacle.h"


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
        itemInHand = { ItemType::BOMB, bomb->getBombID(), bomb->getColor() };
        room.removeBomb(nextPoint);
        return true;
    }
    return false;
}