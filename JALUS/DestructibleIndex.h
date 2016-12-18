#pragma once
#include "Common.h"

class DestructibleIndex
{
public:
	bool data_0 = false;
	bool data_1 = false;

	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};