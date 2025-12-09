#pragma once

#include <vector>
#include "Utils.h"
#include "Placement.h"
#include "Switch.h"

// Door class representing a door in the game
class Door {

    struct SwitchRequirement {
        const Switch* SW;
        bool requiredState;   // false = OFF, true = ON
    };

    int id;
    Placement pos;
    std::vector<int> requiredKeyIDs;
    std::vector<SwitchRequirement> requiredSwitchIDs;
    Color color = Color::WHITE;
    bool isOpen = false;

public:

    Door(int x, int y, int _id, Color c) : pos(x, y, _id + '0'), id(_id), color(c) {}
   
    void addRequiredKey(int _id) {
        requiredKeyIDs.push_back(_id);
    }

    void addRequiredSwitch(Switch* switchOnOff, bool state) {
        requiredSwitchIDs.push_back({ switchOnOff, state });
    }

    int getDoorID() const { return id; }

    Color getColor() const {
        return color;
    }
    
    
    // Open/Close logic
    void open() { isOpen = true; }
    void close() { isOpen = false; }
    bool getIsOpen() const { return isOpen; }

    Placement getPos() const { return pos; }

    void draw();

    bool tryUnlock(int keyID);
    void UpdatedFromSwitch();

    bool isAtPosition(const Point& p) const {
        return (pos.getx() == p.x && pos.gety() == p.y);
    }
};  