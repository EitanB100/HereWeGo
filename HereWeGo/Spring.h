#pragma once
#include <vector>
#include "Placement.h"
#include "Tile_Chars.h" 


class Spring {
    std::vector<Placement> parts;
    Point direction; // The direction the spring pushes (e.g., 1,0 for Right)
    int compressedCount = 0;
  
public:
    Spring(Point dir) : direction(dir) {}


    Point getDirection() const { return direction; }

    const std::vector<Placement>& getParts() const { return parts; }

    int getCompressionCount() { return compressedCount; }
    
    // Add a tile to the spring (springs can be multiple tiles long)
    void addPart(int x, int y) {
        parts.push_back(Placement(x, y, SPRING_TILE));
    }

    // Check if a specific point is part of this spring
    bool isSpringPart(const Point& p) const;

 

    void setCompression(int count) { compressedCount = count; }

    void setDirection(Point dir);
    void sortParts();

    void draw();
};