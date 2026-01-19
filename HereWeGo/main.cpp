#include <windows.h>
#include <conio.h>
#include <iostream>
#include "Game.h"
#include "Recording_Game.h"
#include "ReplayGame.h"
//removed all the inclusions!


int main(int argc, char* argv[]) {


	bool isSaveMode = false;
	bool isLoadMode = false;
	bool isSilent = false;

	if (argc > 1) {
		std::string mode = argv[1];
		if (mode == "-save") isSaveMode = true;
		else if (mode == "-load") isLoadMode = true;
	}

	if (argc > 2) {
		std::string s = argv[2];
		if (s == "-silent") {
			isSilent = true;
		}
	}
	
	if (isLoadMode) {
		ReplayGame game(isSilent);
		game.startInLevel(Level::ONE);
		game.run();
		if (!isSilent) {
			printCentered("Replay Finished. Press Any Key To Exit...",2);
			while (_kbhit())
				_getch();
		}

		return 0;
	}

	auto runGameLevel = [&](Level level) {
		if (isSaveMode) {
			RecordingGame game;
			game.startInLevel(level);
			game.run();
		}
		else { //load or silent
			Game game;
			game.startInLevel(level);
			game.run();
		}
	};

	bool exitProgram = false;
	hideCursor();
	constexpr char MENU_LVL1 = '1';
	constexpr char MENU_LVL2 = '2';
	constexpr char MENU_LVL3 = '3';
	constexpr char MENU_ENDING = '4';
	constexpr char MENU_LOAD_GAME = '5';
	constexpr char MENU_SETTINGS = '7';
	constexpr char MENU_INSTRUCTIONS = '8';
	constexpr char MENU_EXIT = '9';
	while (!exitProgram)
	{
		system("cls");

		printCentered("Welcome to the game!", 4);
		printCentered("(1) Start new game", 7);
		printCentered("(5) Load saved game", 9);
		printCentered("(7) Settings", 11);
		printCentered("(8) Instructions", 13);
		printCentered("(9) EXIT", 15);
		gotoxy(0, 16);
		std::cout << "for easy playtesting:" << std::endl;
		std::cout << "(2) Skip to level 2" << std::endl;
		std::cout << "(3) Skip to level 3" << std::endl;
		std::cout << "(4) Ending Screen" << std::endl;

		char selection = _getch();

		switch (selection)
		{
			
		case MENU_LVL1:	
		{
			runGameLevel(Level::ONE);
			break;
		}
		case MENU_LVL2:
		{
			runGameLevel(Level::TWO);
			break;
		}
		case MENU_LVL3:
		{
			runGameLevel(Level::THREE);
			break;
		}
		case MENU_ENDING:
		{
			Game game;
			game.showEndingScreen();
			break;
		}
		case MENU_LOAD_GAME:
		{

			Game* gamePtr = nullptr; // temp pointer to game obeject which can either be Game or RecordingGame
			Game tempGame;
			int selectedSlot = tempGame.getSlotFromLoadMenu(); // You'll need this helper

			if (selectedSlot != -1) {
				// 2. CHECK: Is this a recorded game?
				if (tempGame.isSlotRecorded(selectedSlot)) {
					gamePtr = new RecordingGame();
					// Force the -save behavior logic even if -save wasn't in argv
					isSaveMode = true;
				}
				else {
					gamePtr = new Game();
				}

				// 3. Load and Run the specific object
				if (gamePtr->loadGame(selectedSlot)) {
					// Manually trigger the redraw we discussed
					Room& currRoom = gamePtr->getLevel(gamePtr->getCurrentLevelIdx());
					gamePtr->redrawScreen(currRoom, isSilent);
					gamePtr->run();
				}
				delete gamePtr;
			}
			break;
		}
		case MENU_SETTINGS:
		{
			Game game;
			game.settingsMenu();
			break;
		}
		case MENU_INSTRUCTIONS: 
		{
			Game game;
			gotoxy(MAX_X / 2, MAX_Y / 2);
			printInstructions();
			break;
		}
		case MENU_EXIT:
		{
			system("cls");
			printCentered("Goodbye :3", 10);
			exitProgram = true;
			break;
		}

		}

	}

	return 0;
}