#pragma once
#include <Windows.h>
#include <string>
#include <conio.h>

constexpr char ESC = 27;
constexpr char ENTER = 13;

constexpr int MAX_X = 80;
constexpr int MAX_Y = 25;

enum itemType { NONE, KEY, TORCH, BOMB }; // possible item types - 

enum class Color {
    BLUE = 1, GREEN, CYAN, RED, MAGENTA,YELLOW,WHITE = 15//white is default
};

struct heldItem {
    itemType type = NONE;
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


void printCentered(std::string text, int y); //to print the menu in the center
void printInstructions();

void setColor(Color c);
 

void gotoxy(int x, int y);

void hideCursor();

