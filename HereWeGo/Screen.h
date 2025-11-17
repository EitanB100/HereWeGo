#pragma once
#include <cstdlib>
#include "utils.h"
#include "Placement.h"

class Screen {

public:

	void setTile(int x, int y, char symbol) const;
	void inLimit(Placement& p);

	void clearScreen() { 
		system("cls"); 
	}
		
private:

	void createGameScreen();
	void draw() const;
	char* screen[MAX_Y + 1];

	

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