#include "BouncerIndex.h"

void BouncerIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{

	bitStream->Write(flag_0);
	bitStream->Write(flag_1);

}
