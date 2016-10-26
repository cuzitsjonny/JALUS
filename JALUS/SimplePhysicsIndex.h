#pragma once
#include "Common.h"

class SimplePhysicsIndex
{
public:
	bool data_0 = false;
	float data_1 = 0.0F;

	bool flag_2 = false;
	float data_2_0;
	float data_2_1;
	float data_2_2;
	float data_2_3;
	float data_2_4;
	float data_2_5;

	bool flag_3 = true;
	unsigned long data_3_0 = 5;

	bool flag_4 = false;
	float pos_x;
	float pos_y;
	float pos_z;
	float rot_x;
	float rot_y;
	float rot_z;
	float rot_w;

	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};