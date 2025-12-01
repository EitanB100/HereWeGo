#include "Game.h"

const char p1Keys[NUM_KEYS] = { 'W','S','A','D','E','Q' };
const char p2Keys[NUM_KEYS] = { 'I','K','J','L','U','O' };

Game::Game() : players{
	Player(Placement(10,20),'&',1,0,p1Keys),
	Player(Placement(9,15),'@',0,1,p2Keys)
}
{
	init();
}
void Game::init()
{

}

void Game::run()
{

}
