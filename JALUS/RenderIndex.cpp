#include "RenderIndex.h"

void RenderIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{
	if (isConstruction)
	{
		bitStream->Write((unsigned long)0);
	}
}
