#include "PhantomPhysicsIndex.h"

void PhantomPhysicsIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{
	bitStream->Write(flag_0);
	if (flag_0)
	{
		bitStream->Write(pos_x);
		bitStream->Write(pos_y);
		bitStream->Write(pos_z);
		bitStream->Write(rot_x);
		bitStream->Write(rot_y);
		bitStream->Write(rot_z);
		bitStream->Write(rot_w);
	}

	bitStream->Write(flag_1);
	if (flag_1)
	{
		bitStream->Write(data_1_0);
	}
}
