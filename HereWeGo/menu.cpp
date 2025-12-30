#include "Game.h"
#include <conio.h>
#include <iostream>
#include <string>


void Game::settingsMenu() {
	bool inSettings = true;
	while (inSettings) {
		system("cls");
		printCentered("Change Players Keys", 3);
		printCentered("(1) Change P1 keys", 5);
		printCentered("(2) Change P2 keys", 7);
		printCentered("(9) Back to menu", 9);

		char selection = _getch();

		switch (selection)
		{
		case '1':
		{
			updatePlayerKeys(p1Keys, 1);
			break;
		}
		case '2':
		{
			updatePlayerKeys(p2Keys, 2);
			break;
		}

		case '9':
		{
			inSettings = false;
			break;
		}
		}
	}
}


void Game::updatePlayerKeys(char keys[], int playerNum) {
	const char* commandNames[] = { "UP", "DOWN", "LEFT", "RIGHT", "STAY", "DISPOSE" , "" };
	bool finishing = false;

	while (!finishing) {
		system("cls");
		printCentered("PLAYER " + std::to_string(playerNum) + " CONTROLS", 2);

		// Display current mapping
		for (int i = 0; i < KEY_COUNT; i++) {
			std::string row = "(" + std::to_string(i + 1) + ") " + commandNames[i] + ": [" + keys[i] + "]";
			printCentered(row, 4 + i);
		}

		printCentered("(ESC or Enter) Save and Exit", 12);
		printCentered("Select a number to rebind:", 14);

		char choice = _getch();
		bool isConflict = false;

		if (choice == ESC || choice == ENTER) {
			finishing = true;
		}
		else {


			int index = -1;
			// Check if choice is a number 1-6
			if (choice >= '1' && choice <= '6') {
				index = choice - '1';
			}
			// Check if choice is the actual command key
			else {
				for (int i = 0; i < KEY_COUNT; i++) {
					if (toupper(choice) == keys[i]) {
						index = i;
						break;
					}
				}
			}
			if (index != -1) {
				system("cls");
				printCentered("Changing " + std::string(commandNames[index]), 10);
				printCentered("Press new key...", 12);

				char newKey = _getch();
				newKey = toupper(newKey);

				for (int i = 0; i < KEY_COUNT; i++) {
					if (newKey == p1Keys[i]) {
						printCentered("Key is already used as " + std::string(commandNames[i]) + " for Player 1...", 14);
						isConflict = true;
						break;
					}
				}

				if (!isConflict) {
					for (int i = 0; i < KEY_COUNT; i++) {
						if (newKey == p2Keys[i]) {
							printCentered("Key is already used as " + std::string(commandNames[i]) + " for Player 2...", 14);
							isConflict = true;
							break;
						}
					}
				}

				// Validation: Check if newKey is ESC or ENTER
				if (newKey == ESC || newKey == ENTER) {
					printCentered("ERROR: ESC and ENTER are reserved!", 14);

				}
				else if (!isConflict) {
					keys[index] = toupper(newKey);
					printCentered("SAVED!", 14);

				}
			}
			Sleep(600);
		}
	}
}