#include "DestructibleIndex.h"

void DestructibleIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{
	if (isConstruction)
	{
		bitStream->Write(flag_0);
		bitStream->Write(flag_1);
	}
}
