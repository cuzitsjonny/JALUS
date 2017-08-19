#pragma once
#include "Common.h"

class ScriptedActivityIndex
{
public:
	
	bool flag;
	unsigned long count;
	unsigned long long playerObjectID;

	float float_0;
	float float_1;
	float float_2;
	float float_3;
	float float_4;
	float float_5;
	float float_6;
	float float_7;
	float float_8;
	float float_9;


	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};
