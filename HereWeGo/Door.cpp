#include "Door.h"

bool Door::tryUnlock(int playerKeyID)
{
	for (size_t i = 0; i < requiredKeyIDs.size(); i++)
	{
		if (playerKeyID == requiredKeyIDs[i])
		{
			requiredKeyIDs.erase(requiredKeyIDs.begin() + i);

			// OLD LINE: if (requiredKeyIDs.empty()) open();

			// NEW LINE: Check switches before opening!
			if (requiredKeyIDs.empty()) UpdatedFromSwitch();

			return true;
		}
	}
	return false;
}

void Door::draw()
{
	if (isOpen) pos.draw(' ');
	
	if (!isOpen) {
		setColor(color);
		pos.draw();
		setColor(Color::WHITE);
	}
}

//PLACEHOLDER	
void Door::UpdatedFromSwitch()
{
	// 1. If we still need keys, the door stays locked.
	if (!requiredKeyIDs.empty()) {
		return;
	}

	// 2. Check all switch requirements
	bool conditionsMet = true;
	for (const auto& req : requiredSwitchIDs)
	{
		// req.SW is the pointer, req.requiredState is the boolean (ON/OFF)
		if (req.SW && req.SW->getIsState() != req.requiredState)
		{
			conditionsMet = false;
			break;
		}
	}

	// 3. Open or Close based on the check
	if (conditionsMet) {
		open();
	}
	else {
		close();
	}
}