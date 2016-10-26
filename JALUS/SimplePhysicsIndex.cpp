#include "SimplePhysicsIndex.h"

void SimplePhysicsIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{
	if (isConstruction)
	{
		bitStream->Write(data_0);
		bitStream->Write(data_1);
	}

	bitStream->Write(flag_2);
	if (flag_2)
	{
		bitStream->Write(data_2_0);
		bitStream->Write(data_2_1);
		bitStream->Write(data_2_2);
		bitStream->Write(data_2_3);
		bitStream->Write(data_2_4);
		bitStream->Write(data_2_5);
	}

	bitStream->Write(flag_3);
	if (flag_3)
	{
		bitStream->Write(data_3_0);
	}

	bitStream->Write(flag_4);
	if (flag_4)
	{
		bitStream->Write(pos_x);
		bitStream->Write(pos_y);
		bitStream->Write(pos_z);
		bitStream->Write(rot_x);
		bitStream->Write(rot_y);
		bitStream->Write(rot_z);
		bitStream->Write(rot_w);
	}
}
