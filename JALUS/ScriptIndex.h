#pragma once
#include "Common.h"

class ScriptIndex
{
public:
	bool flag_0 = false;

	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};