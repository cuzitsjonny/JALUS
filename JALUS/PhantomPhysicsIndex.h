#pragma once
#include "Common.h"

class PhantomPhysicsIndex
{
public:
	bool flag_0 = false; // TOGGLE MANUALLY!
	float pos_x; // SET MANUALLY!
	float pos_y; // SET MANUALLY!
	float pos_z; // SET MANUALLY!
	float rot_x; // SET MANUALLY!
	float rot_y; // SET MANUALLY!
	float rot_z; // SET MANUALLY!
	float rot_w; // SET MANUALLY!

	bool flag_1 = true;
	bool data_1_0 = false;

	void writeToBitStream(BitStream* bitStream, bool isConstruction);
};