#include "DestructibleIndex.h"

void DestructibleIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{
	if (isConstruction)
	{
		bitStream->Write(data_0);
		bitStream->Write(data_1);
	}
}
