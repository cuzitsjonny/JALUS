#pragma once
#include "Common.h"

class BehaviorEffect
{
public:
	string string_effect_id;
	unsigned long effect_id;
	wstring effect_type;
	float unknown_0;
	long long unknown_1;
};

class RenderIndex
{
public:
	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};