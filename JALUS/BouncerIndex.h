#pragma once
#include "Common.h"

class BouncerIndex
{
public:
	bool flag_0 = false; // TOGGLE MANUALLY!
	bool flag_1 = false;


	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};
