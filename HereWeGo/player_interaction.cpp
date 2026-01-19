#include "Player.h"
#include "Room.h"
#include "Obstacle.h"


//Obstacle pushing logic
bool Player::obstacleHandling(Room& room, Point& nextPoint, Player* otherPlayer, bool isSilent)
{
    Obstacle* obstacleToPush = room.isObstacleThere(nextPoint);
    //if it already been moved this frame
    if (obstacleToPush != nullptr && obstacleToPush->getHasMoved()) return false;
    
    if (otherPlayer) {
        std::vector<Point> futureParts = obstacleToPush->getFutureParts(dirx,diry);
        for (const auto& part : futureParts) {
            if (part == otherPlayer->getPos()) {
                if (spring.flightTime > 0) {
                    Point ogDir = { otherPlayer->getDirX(),otherPlayer->getDirY() };
                    int ogForce = otherPlayer->getForce();

                    otherPlayer->setDirection({ dirx,diry });
                    otherPlayer->setForce(force);
                    otherPlayer->move(room, this, isSilent);
                    otherPlayer->setDirection({ ogDir.x,ogDir.y });
                    otherPlayer->setForce(ogForce);

                    if (otherPlayer->getPos() == part) return false;
                }
                else return false;
            }
        }
    }

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
        bool hasMoved = room.moveObstacle(nextPoint, dirx, diry, currentForce, isSilent);

        //if moved, move the partner too
        if (hasMoved) {
            if (combinedPush) synchronizePartner(otherPlayer, room, isSilent);

            if (spring.flightTime > 0) obstacleToPush->resetMove();
        }
        return hasMoved;
    }
    return false;
}


//Helper function that ensures that when two playes push an obstacle, 
//both will move in sync with each other
//Suggested by Gemini!
void Player::synchronizePartner(Player* otherPlayer, Room& room, bool isSilent) {
    if (otherPlayer == nullptr) return;

    //redraw the tile (similar to move function)
    Point p = otherPlayer->getPos();
    Color c = Color::WHITE;
    char tileBelow = room.getObjectAt(p, c); //the tile that became invisible when disposed!

    setColor(c);
    if (!isSilent) otherPlayer->pos.draw(tileBelow);
    setColor(Color::WHITE);

    //move the partner in the same direction
    otherPlayer->pos.move(dirx, diry, otherPlayer->symbol);

    if (!isSilent) otherPlayer->draw();
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
    if (bomb != nullptr && !bomb->isActivated()) {
        itemInHand = { ItemType::BOMB, bomb->getBombID(), bomb->getColor() };
        room.removeBomb(nextPoint);
        return true;
    }
    return false;
}