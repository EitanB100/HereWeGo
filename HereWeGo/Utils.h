#pragma once
#include <Windows.h>
#include <string>
#include <conio.h>


constexpr char ESC = 27;
constexpr char ENTER = 13;
constexpr int MIN_X = 0;
constexpr int MIN_Y = 4;
constexpr int MAX_X = 80;
constexpr int MAX_Y = 25;

static constexpr int HP_INCREASE = 5;

enum class ItemType { NONE, KEY, TORCH, BOMB }; // possible item types - 

enum class Level {ONE = 0, TWO, THREE, COUNT};
inline int operator*(Level level) { return static_cast<int>(level); }
inline bool operator<(Level level, int i) { return static_cast<int>(level) < i; }

inline Level& operator++ (Level& level) {
    level = static_cast<Level>(static_cast<int>(level) + 1);
    return level;
}

enum class Color {
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,      
    LIGHT_GRAY = 7,
    DARK_GRAY = 8,
    SKY = 9,
    LIME = 10,
    AQUA = 11,
    ORANGE = 12,      
    YELLOW = 14,      
    WHITE = 15       
};

struct heldItem {
    ItemType type = ItemType::NONE;
    int id = 0; // key ID or bomb timer
    

    Color color = Color::WHITE;
 
};

struct Point { // simple struct for position getting
    int x, y;

    bool operator== (const Point& other) const{
        return x == other.x && y == other.y;
    }
    
    bool operator!= (const Point& other) const {
        return x != other.x || y != other.y;
    }
};

constexpr Point DEFAULT_POINT = { 1,1 };

struct Directions {
    static constexpr Point UP = { 0,-1 };
    static constexpr Point RIGHT = { 1,0 };
    static constexpr Point DOWN = { 0,1 };
    static constexpr Point LEFT = { -1,0 };
    static constexpr Point STAY = { 0,0 };
};

void printCentered(std::string text, int y); //to print the menu in the center
void printInstructions();

void setColorMode(bool useColor);
bool getColorMode();
void setColor(Color c);
void setColor(Color c, bool allowColor);

void gotoxy(int x, int y);

void hideCursor();

