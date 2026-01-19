#include "Door.h"

bool Door::tryUnlock(int playerKeyID)
{
	for (size_t i = 0; i < requiredKeyIDs.size(); i++)
	{
		if (playerKeyID == requiredKeyIDs[i])
		{
			requiredKeyIDs.erase(requiredKeyIDs.begin() + i);

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

bool Door::UpdatedFromSwitch()
{
	// 1. If we still need keys, the door stays locked.
	if (!requiredKeyIDs.empty()) {
		return false;
	}

	// 2. Check all switch requirements
	bool conditionsMet = true;
	for (const auto& req : requiredSwitchIDs)
	{
		// req.SW is the pointer, req.requiredState is the boolean (ON/OFF)
		if (!req.SW || req.SW->getState() != req.requiredState)
		{
			conditionsMet = false;
			break;
		}
	}

	bool stateChanged = false; 

	if (conditionsMet) {
		if (!isOpen) {
			open();
			stateChanged = true;
		}
	}

	else {
		if (isOpen) {
			close();
			stateChanged = true;
		}
	}

	return stateChanged;
}

void Door::invalidateSwitch(const Switch* sw)
{
	for (auto& requiredSwitch : requiredSwitchIDs) {
		if (requiredSwitch.SW == sw) requiredSwitch.SW = nullptr;
	}
	UpdatedFromSwitch();
}
