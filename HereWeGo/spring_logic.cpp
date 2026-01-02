#include "Player.h"
#include "Room.h"
#include "Spring.h"


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
                    setDirection(Directions::STAY);
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
                setDirection(Directions::STAY);
                return false;
            }

            // Stop if fully compressed
            if (spring.compressionCount >= (int)s->getParts().size() && isOpposing) {
                setDirection(Directions::STAY);
                return false;
            }
            spring.compressionCount++;
            s->setCompression(spring.compressionCount);
        }
    }
    return true;
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
        setDirection(Directions::STAY);
        return false;
    }
    s->setCompression(0);
    s->draw();
    spring.compressionCount = 0;
    return true;
}