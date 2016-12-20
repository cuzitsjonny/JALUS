#pragma once
#include "Common.h"

class SkillIndex
{
public:
	bool data_0 = false;

	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};