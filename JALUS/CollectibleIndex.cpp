#include "CollectibleIndex.h"

void CollectibleIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{
	bitStream->Write(collectible_id);
}
