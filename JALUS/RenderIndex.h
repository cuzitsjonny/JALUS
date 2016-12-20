#pragma once
#include "Common.h"

class FXEffect
{
public:
	string name = "";
	long id = -1;
	wstring type = L"";
	float scale = 1.0F;
	long long secondary = 0;
};

class RenderIndex
{
public:
	vector<FXEffect> effects = vector<FXEffect>();

	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};