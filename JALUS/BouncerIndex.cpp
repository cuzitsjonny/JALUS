#include "BouncerIndex.h"

void BouncerIndex::writeToBitStream(BitStream* bitStream, bool isConstruction)
{

	bitStream->Write(flag_0);
	bitStream->Write(data_0_1);

}
