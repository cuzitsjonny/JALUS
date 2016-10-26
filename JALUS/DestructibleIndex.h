#pragma once
#include "Common.h"

class DestructibleIndex
{
public:
	bool flag_0 = false;
	bool flag_1 = false;

	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};