#pragma once
#include "Common.h"

class CollectibleIndex
{
public:
	unsigned short collectible_id; // SET MANUALLY!

	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};