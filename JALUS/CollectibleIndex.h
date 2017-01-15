#pragma once
#include "Common.h"

class CollectibleIndex
{
public:
	unsigned short collectible_id = 0;

	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};