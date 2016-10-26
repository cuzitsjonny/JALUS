#pragma once
#include "Common.h"
#include "InventoryItems.h"

class InventoryIndex
{
public:
	InventoryIndex(bool isPlayer, long long objectID);

	bool isPlayer;
	long long objectID;

	bool flag_1 = true;
	unsigned long data_1_0 = 0;

	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};