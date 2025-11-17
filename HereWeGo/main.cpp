#include <windows.h>
#include <conio.h>
#include "Player.h"
#include "Placement.h"
#include "Room.h"
#include "Utils.h"
#include "CommandKeys.h"

int main() {

    constexpr char ESC = 27;
    hideCursor();

    Placement start(10, 10);
    Placement startR(7, 6);

    char player1Keys[NUM_KEYS] = { 'W','S','A','D','E','Q'};
    char player2Keys[NUM_KEYS] = { 'I','K','J','L','U' };    //no logic implemented yet!

    Room areaA(startR, 'W', 12, 21);
    
    Player player(start, 'H', 1, 0, player1Keys);
    
    areaA.MakeWalls();

    while (true) {
        player.changeDirection();  // instant input reaction
        player.move();             // move in that direction

        if (_kbhit()) {
            char ch = _getch();
            if (ch == ESC) break;  // escape to exit
        }

        Sleep(100); // adjust to control speed (lower = faster)
    }

    cls();
    return 0;
}