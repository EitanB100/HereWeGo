#pragma once
#include <cstdlib>
#include "utils.h"
#include "Placement.h"

class Screen {
private: //was that what u meant? 
	int xSize, ySize;

public:
	Screen(int x_Size, int y_Size) //random ass naming for now
	{
		xSize = x_Size;
		ySize = y_Size;
	}

	void draw() const;
	void inLimit(Placement& p);

	void clearScreen() { 
		system("cls"); 
	}
		
private:


	char* screen[MAX_Y + 1];

	void createGameScreen();

	char charAt(Placement& p) const {
		return screen[p.gety()][p.getx()];
	}


public:

	bool isWall(Placement& p) const {
		return charAt(p) == 'W';
	}
	bool isWonChar(Placement& p) const {
		return charAt(p) == '%';
	}


};