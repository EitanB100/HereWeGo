#pragma once
#include <vector>
#include "Placement.h"
#include "Tile_Chars.h" 

class Spring {
    std::vector<Placement> parts;
    Point direction; // The direction the spring pushes (e.g., 1,0 for Right)

public:
    Spring(Point dir) : direction(dir) {}

    // Add a tile to the spring (springs can be multiple tiles long)
    void addPart(int x, int y) {
        parts.push_back(Placement(x, y, SPRING_TILE));
    }

    // Check if a specific point is part of this spring
    bool isSpringPart(const Point& p) {
        for (const auto& part : parts) {
            if (part.getx() == p.x && part.gety() == p.y) return true;
        }
        return false;
    }

    Point getDirection() const { return direction; }
    const std::vector<Placement>& getParts() const { return parts; }

    void draw() {
        for (auto& part : parts) {
            part.draw();
        }
    }
};