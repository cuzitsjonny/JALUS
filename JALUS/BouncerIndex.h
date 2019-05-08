#pragma once
#include "Common.h"

class BouncerIndex
{
public:
	bool flag_0 = false;
	bool data_0_1 = false;


	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};
