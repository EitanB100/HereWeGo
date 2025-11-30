#include "Door.h"

bool Door::tryUnlock(int playerKeyID)
{
	for (size_t i = 0; i < requiredKeyIDs.size(); i++)
	{
		if (playerKeyID == requiredKeyIDs[i])
		{
			requiredKeyIDs.erase(requiredKeyIDs.begin() + i);
			
			if (requiredKeyIDs.empty()) open();

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
